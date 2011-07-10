#include <errno.h>
#include "Midi.h"
#include "mapm.h"
#include <iostream>

MIDI::MIDI(void)
{
	openPorts();

	current_midi_port = 1;

//	uartSpeedTest();
}

MIDI::~MIDI(void)
{
	closePorts();
}

void MIDI::closePorts(void)
{
	close(synth_fd);
	close(midi_fd);
}

void MIDI::openPorts(void)
{
	std::cout << "Initialising MIDI... ";

	//ttyS1 = synth, ttyS2 = midi
	if(initialise(&synth_fd, &midi_fd) != 0) //ttyS0 is the terminal connection
		std::cout << "FAILED." << std::endl << std::flush;
	else
		std::cout << "success." << std::endl;
}

char MIDI::initialise(int *synthfd, int *midioutfd)
{
	struct termios synth_options, midi_options;
	
	*synthfd = open("/dev/ttyS1", O_RDWR | O_NOCTTY | O_NDELAY);
	if(*synthfd == -1)
		return 1;

	*midioutfd = open("/dev/ttyS2", O_RDWR | O_NOCTTY | O_NDELAY);
	if(*midioutfd == -1)
		return 1;

	tcgetattr(*synthfd, &synth_options);	//get port options
	cfsetispeed(&synth_options, B115200);	//set input baud rate
	cfsetospeed(&synth_options, B115200);	//set output baud rate
	cfmakeraw(&synth_options);
	tcsetattr(*synthfd, TCSANOW, &synth_options);	//set new port options

	tcgetattr(*midioutfd, &midi_options);	//get port options
	cfsetispeed(&midi_options, B115200);//B38400);//115200);	//set input baud rate
	cfsetospeed(&midi_options, B115200);//B38400);//115200);	//set output baud rate
	cfmakeraw(&midi_options);
	tcsetattr(*midioutfd, TCSANOW, &midi_options);	//set new port options

//	system("mm w 4 0x48001030 0x84");	//set UART2 SYNTH (/dev/ttyS1) to 31250 bps
//	system("mm w 4 0x48002030 0x84");	//set UART3 MIDI (/dev/ttyS2) to 31250 bps

	//set UART3 MIDI (/dev/ttyS2) to 31250bps
	volatile unsigned int *uart;
	uart = (unsigned int *) mapm(0x48002000, 0x1000);
	*(uart + (0x30>>2)) = (60000000/16/31250); // PERCLK1=60000000Hz Baudrate=31250
	*(uart + (0x34>>2)) = 0x00000000;
	unmapm((unsigned int *)uart,0x1000);

	return 0;
}

void MIDI::safeWrite(int fd, void *buf, size_t nbytes)
{
	int total_bytes = 0;

	while(total_bytes != nbytes)
	{
//if(total_bytes != 0) std::cout << "UART: " << strerror(errno) << std::endl << std::flush;
		int b = write(fd, (void *) ((char *)buf+total_bytes), nbytes-total_bytes);
		if(b > 0)
			total_bytes += b;
	}
}

void MIDI::uartSpeedTest(void)
{
	unsigned char midi_buffer[6] = {0x90, 60, 127, 0x80, 60, 0};
	void *buf = &midi_buffer;

	int us = 10;

	std::cout << "Starting UART speed test... " << std::flush;

//	while(us >= 0)
	{
//		std::cout << us << " " << std::flush;
		for(int i = 0; i < 10000; i++)
		{
			int total_bytes = 0;

			while(total_bytes != 3)
			{
				int b = write(synth_fd, (void *) ((char *)buf+total_bytes), 3-total_bytes);
				if(b > 0)
					total_bytes += b;
				else
				{
					std::cout << "UART: " << strerror(errno) << std::endl << std::flush;
					std::cout << "Failed on message " << i << "." << " Wrote " << b << " bytes." << std::endl << std::flush;
					return;
				}
			}

//			if(us > 0) usleep(us);
		}

		us -= 1;
	}

	std::cout << "done." << std::endl << std::flush;
}

void MIDI::sendNoteOn(unsigned char output, unsigned char channel, unsigned char note, unsigned char velocity)
{
	int chan = channel;
	if(output == SYNTH)
		chan = setMidiPort(channel);

	unsigned char midi_on_buffer[3] = {0x90+chan, note, velocity};

	if(output == SYNTH)
		safeWrite(synth_fd, &midi_on_buffer, 3);
	else
		safeWrite(midi_fd, &midi_on_buffer, 3);

//std::cout << std::endl << "note on " << (unsigned int) output << ": " << (unsigned int) note << " channel: " << (unsigned int) channel << " port channel: " << (unsigned int) chan << " velocity: " << (unsigned int) velocity << "." << std::flush;
}

void MIDI::sendNoteOff(unsigned char output, unsigned char channel, unsigned char note, unsigned char velocity)
{
	int chan = channel;
	if(output == SYNTH)
		chan = setMidiPort(channel);

	unsigned char midi_off_buffer[3] = {0x80+chan, note, velocity};

	if(output == SYNTH)
		safeWrite(synth_fd, &midi_off_buffer, 3);
	else
		safeWrite(midi_fd, &midi_off_buffer, 3);

//std::cout << std::endl << "note off " << (unsigned int) output << ": " << (unsigned int) note << "." << std::flush;
}

void MIDI::sendSoundOff(unsigned char output, unsigned char channel)
{
	if(output == MIDI_OUT)
	{
		unsigned char sound_off_buffer[3] = {0xB0+channel, 0x78, 0};

		safeWrite(midi_fd, &sound_off_buffer, 3);
	}
	else
//	if(output == SYNTH)
	{
		int chan = setMidiPort(channel);
		unsigned char sound_off_buffer[3] = {0xB0+chan, 0x78, 0};

		safeWrite(synth_fd, &sound_off_buffer, 3);
	}
//std::cout << std::endl << "send sound off " << (unsigned int) output << std::flush;
}

void MIDI::sendPitch(unsigned char output, unsigned char channel, unsigned int p)
{
	int chan = channel;
	if(output == SYNTH)
		chan = setMidiPort(channel);

	unsigned char ph = (unsigned char) p & 127;
	unsigned char pl = (unsigned int) p >> 7;
	unsigned char pitch_wheel[3] = {0xE0+chan, ph, pl};

	if(output == SYNTH)
		safeWrite(synth_fd, &pitch_wheel, 3);
	else
		safeWrite(midi_fd, &pitch_wheel, 3);

//std::cout << std::endl << "pitch " << (unsigned int) output << ": " << (unsigned int) channel << " " << (unsigned int) p << std::flush;
}

void MIDI::sendCC(unsigned char output, unsigned char channel, unsigned char control, unsigned char value)
{
	int chan = channel;
	if(output == SYNTH)
		chan = setMidiPort(channel);

	unsigned char midi_off_buffer[3] = {0xB0+chan, control, value};

	if(output == SYNTH)
		safeWrite(synth_fd, &midi_off_buffer, 3);
	else
		safeWrite(midi_fd, &midi_off_buffer, 3);

//std::cout << "control change " << (unsigned int) output << ": " << (unsigned int) control << " value: " << (unsigned int) value << "." << std::flush;
}

void MIDI::sendNRPN(unsigned char output, unsigned char channel, unsigned char nrpn_high, unsigned char nrpn_low, unsigned char val)
{
	int chan = channel;
	if(output == SYNTH)
		chan = setMidiPort(channel);

	char nrpn[7] = {0xB0+chan, 0x63, nrpn_high, 0x62, nrpn_low, 0x06, val};
//std::cout << "chan: " << chan << " high: " << (unsigned int) nrpn_high << " low: " << (unsigned int) nrpn_low << " val: " << (unsigned int) val << std::endl << std::flush;
	if(output == SYNTH)
		safeWrite(synth_fd, nrpn, 7);
	else
		safeWrite(midi_fd, nrpn, 7);
}

void MIDI::sendRPN(unsigned char output, unsigned char channel, unsigned char rpn_high, unsigned char rpn_low, unsigned char val)
{
	int chan = channel;
	if(output == SYNTH)
		chan = setMidiPort(channel);

	char rpn[7] = {0xB0+chan, 0x65, rpn_high, 0x64, rpn_low, 0x06, val};

	if(output == SYNTH)
		safeWrite(synth_fd, rpn, 7);
	else
		safeWrite(midi_fd, rpn, 7);

//std::cout << std::endl << "rpn" << (unsigned int) output << " val: " << (unsigned int) val << "." << std::flush;
}

void MIDI::sendSysex(unsigned char output, unsigned char *sysex, unsigned int size)
{
//std::cout << std::endl << "sysex." << std::flush;

	if(output == SYNTH)
		safeWrite(synth_fd, sysex, size);
	else
		safeWrite(midi_fd, sysex, size);
}

void MIDI::sendSysexToChannel(unsigned char output, unsigned char *sysex, unsigned int size, unsigned char channel)
{
//std::cout << std::endl << "sysextochan." << std::flush;
	int chan = channel;
	if(output == SYNTH)
		chan = setMidiPort(channel);

/*	if(channel >= 16)
		sysex[5] = 0x50;
	else
		sysex[5] = 0x40;
*/
	if(output == SYNTH)
		safeWrite(synth_fd, sysex, size);
	else
		safeWrite(midi_fd, sysex, size);
}

void MIDI::sendGSReset(void)
{
	char sysex[11] = {0xF0, 0x41, 0x00, 0x42, 0x12, 0x40, 0x00, 0x7F, 0x00, 0x00, 0xF7};
	safeWrite(synth_fd, sysex, 11);
}

void MIDI::sendProgramChange(unsigned char channel, unsigned char bank, unsigned char prog)
{
	int chan = setMidiPort(channel);

	unsigned char bc[3] = {0xB0+chan, 00, bank};
	unsigned char pc[2] = {0xC0+chan, prog};

//std::cout << std::endl << "program_change_" << (unsigned int) channel << "port channel: " << chan << "_" << (unsigned int) bank << "_" << (unsigned int) prog << "." << std::flush;
	safeWrite(synth_fd, bc, 3);
	safeWrite(synth_fd, pc, 2);
}

int MIDI::setMidiPort(unsigned char channel)
{
	unsigned char midi_port_to_1[2] = {0xF5, 0x01};
	unsigned char midi_port_to_2[2] = {0xF5, 0x02};

	if(current_midi_port == 1)
	{
		if(channel >= 16)
		{
			current_midi_port = 2;
			safeWrite(synth_fd, &midi_port_to_2, 2);
//std::cout << std::endl << "Set MIDI port 2." << std::flush;
		}
	}
	else
	{
		if(channel < 16)
		{
			current_midi_port = 1;
			safeWrite(synth_fd, &midi_port_to_1, 2);
//std::cout << std::endl << "Set MIDI port 1." << std::flush;
		}
	}

	if(channel >= 16)
		return channel - 16;
	else
		return channel;
}

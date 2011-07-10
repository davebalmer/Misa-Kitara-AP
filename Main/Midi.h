#ifndef __MIDI_H__
#define __MIDI_H__

#include <string>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <vector>
#include "Touchpanel.h"
#include "Neck.h"

#define SYNTH		0
#define MIDI_OUT	1

class MIDI
{
	private:
		int synth_fd, midi_fd;
		char current_midi_port;

	public:
		MIDI(void);
		~MIDI(void);
		void openPorts(void);
		void closePorts(void);
		char initialise(int *synthfd, int *midioutfd);
		void midiTest(void);
		void sendNoteOn(unsigned char output, unsigned char channel, unsigned char note, unsigned char velocity);
		void sendNoteOff(unsigned char output, unsigned char channel, unsigned char note, unsigned char velocity);
		void sendSoundOff(unsigned char output, unsigned char channel);
		void sendCC(unsigned char output, unsigned char channel, unsigned char control, unsigned char value);
		void sendNRPN(unsigned char output, unsigned char channel, unsigned char nrpn_high, unsigned char nrpn_low, unsigned char val);
		void sendRPN(unsigned char output, unsigned char channel, unsigned char rpn_high, unsigned char rpn_low, unsigned char val);
		void sendSysex(unsigned char output, unsigned char *sysex, unsigned int size);
		void sendSysexToChannel(unsigned char output, unsigned char *sysex, unsigned int size, unsigned char channel);
		void sendGSReset(void);
		void sendProgramChange(unsigned char channel, unsigned char bank, unsigned char prog);
		void sendPitch(unsigned char output, unsigned char channel, unsigned int p);
		int setMidiPort(unsigned char channel);
		void safeWrite(int fd, void *buf, size_t nbytes);
		void uartSpeedTest(void);
};

#endif

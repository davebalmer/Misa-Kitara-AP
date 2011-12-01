#include "Neck.h"
#include <iostream>
#include <errno.h>
#include <fcntl.h>

#define KBDRV_DEVICENAME	"/dev/event3"
#define KB_DEVICENAME	"/dev/keyboard144"
#define NK_STRESS_TEST	false

void *keyboard_thread(void *n)
{
	Neck *neck = (Neck *) n;

	fd_set rds;
	int ret;

	std::cout << "Neck started." << std::endl << std::flush;

#if NK_STRESS_TEST
	struct neckstate new_key_event;
	int iterations = 0;
	std::cout << "Starting neck stress test..." << std::endl << std::flush;
	while(1)
	{
//		iterations++;
//		if(iterations >= 10000) {std::cout << "Neck stress test completed." << std::endl << std::flush; break; }

		for(int s = 0; s < 6; s++)
		{
			int r = rand() % 5;
			if(r < 3) r = 0; else r = rand() % 25;
			new_key_event.string_button[s] = r;
		}

		neck->addToEventQueue(new_key_event);
		usleep(1000*1000);
	}
	for(int s = 0; s < 6; s++)
		new_key_event.string_button[s] = 0;
	neck->addToEventQueue(new_key_event);
#endif

	while(1)
	{
		FD_ZERO(&rds);
		FD_SET(neck->neck_fd, &rds);

		ret = select(neck->neck_fd + 1, &rds, NULL, NULL, NULL);

		if(ret < 0)
			std::cerr << "Neck error." << std::endl;

		if(ret == 0)
			std::cerr << "Neck time out." << std::endl;
		else
		if(FD_ISSET(neck->neck_fd, &rds))
		{
			struct input_event new_event;
			if(read(neck->neck_fd, &new_event, sizeof(struct input_event)) == -1)
			{
				if(errno != EAGAIN)
					std::cerr << "Neck read error." << std::endl;
			}
			else
			if(new_event.type == EV_KEY) //ignore EV_SYN
			{
				unsigned char key_buffer[18];
//				std::cout << "Key detected. " << std::flush;
				read(neck->drv_fd, key_buffer, 18);

				struct neckstate new_key_event;
//std::cout << std::endl << "Event detected." << std::flush;
				//cycle each bit
				for(int s = 0; s < 6; s++)
				{
					unsigned char byte[3] = {key_buffer[s], key_buffer[s+6], key_buffer[s+12]};
					new_key_event.string_button[s] = 0;

					for(unsigned int b = 0; b < 3; b++)
					{
						for(int i = 0; i < 8; i++)
							if(byte[b] & (1<<i))
							{
//								std::cout << std::endl << "string: " << (unsigned int) s << " button: " << (unsigned int) (b*8)+i+1 << "." << std::flush;
								new_key_event.string_button[s] = (i+1)+(b*8);
							}
					}
				}
				neck->addToEventQueue(new_key_event);
			}
		}
	}
	pthread_exit(NULL);
}

Neck::Neck(void)
{
	pthread_t tid;

	void *n = this;

	neck_fd = open(KBDRV_DEVICENAME, 0);
	if(neck_fd < 0)
	{
		std::cerr << "Neck could not be opened." << std::endl;
	}

	drv_fd = open(KB_DEVICENAME, 0);
	if(drv_fd < 0)
	{
		std::cerr << "Neck could not be opened." << std::endl;
	}

	for(int i = 0; i < 6; i++)
		setStringButton(i, 0);	//0 is low E, followed by ADGBE


	if(pthread_mutex_init(&neck_mutex, NULL))
		std::cerr << "Unable to initialise neck mutex." << std::endl;

	pthread_create(&tid, NULL, keyboard_thread, n);
}

Neck::~Neck(void)
{
	close(neck_fd);
	close(drv_fd);
	pthread_exit(NULL);
	pthread_mutex_destroy(&neck_mutex);
}

// OR  : Scene memories
void Neck::clear()
{
	while (neck_events.size() > 0)
		neck_events.pop();
}

//this will pop off the queue
void Neck::getNeckState(struct neckstate *n_state, bool left_handed)
{
	if(neck_events.size() > 0)
	{
		pthread_mutex_lock(&neck_mutex);
		struct neckstate neck_event = neck_events.front();
		pthread_mutex_unlock(&neck_mutex);

		for(int i = 0; i < 6; i++)
		{
			setStringButton(i, neck_event.string_button[i]);
		}

		pthread_mutex_lock(&neck_mutex);
		neck_events.pop();
		pthread_mutex_unlock(&neck_mutex);
	}

	if(left_handed)
	{
		for(int i = 0; i < 6; i++)
			n_state->string_button[5-i] = getStringButton(i);
	}
	else
	{
		for(int i = 0; i < 6; i++)
			n_state->string_button[i] = getStringButton(i);
	}
}

unsigned int Neck::getStringButton(unsigned int str)
{
	return neck_state.string_button[str];
}

void Neck::setStringButton(unsigned int str, unsigned int button)
{
	if(button <= 24)
		neck_state.string_button[str] = button;
}

void Neck::addToEventQueue(struct neckstate n)
{
	pthread_mutex_lock(&neck_mutex);
	neck_events.push(n);
	pthread_mutex_unlock(&neck_mutex);
}

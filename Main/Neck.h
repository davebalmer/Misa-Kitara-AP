#ifndef __NECK_H__
#define __NECK_H__

#include <linux/input.h>
#include <pthread.h>
#include <queue>

struct neckstate
{
	unsigned char string_button[6]; //1 to 24, and 0 means off
};

class Neck
{
	private:
		struct neckstate neck_state;
		std::queue<neckstate> neck_events;
		pthread_mutex_t neck_mutex;

		unsigned int getStringButton(unsigned int str);
		void setStringButton(unsigned int str, unsigned int button);

	public:
		Neck(void);
		~Neck(void);

		void addToEventQueue(struct neckstate n);	//only the thread should use this

		void getNeckState(struct neckstate *n_state, bool left_handed);
		void clear();		// OR  : Scene memories
		int neck_fd, drv_fd;
};

#endif

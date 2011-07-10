#ifndef __TOUCHPANEL_H__
#define __TOUCHPANEL_H__

#include <linux/input.h>
#include <pthread.h>
#include <queue>

struct touchstate
{
	unsigned int x[5];
	unsigned int y[5];
	unsigned char num_touches;
	unsigned char old_num_touches;
};

struct touchpanel_event
{
	unsigned char data[22];
	unsigned short *p_data;
};

class Touchpanel
{
	private:
		struct touchstate touch_state;
		std::queue<touchpanel_event> touch_events;
		pthread_mutex_t tp_mutex;

		unsigned int getX(unsigned int id);
		unsigned int getY(unsigned int id);
		unsigned char getNumTouches(void);
		unsigned char getOldNumTouches(void);
		void setX(unsigned int id, unsigned int new_x);	//sets X to value (0 to 255)
		void setY(unsigned int id, unsigned int new_y);	//sets Y to value (0 to 255)
		void setNumTouches(unsigned char num);
		void setOldNumTouches(unsigned char num);

	public:
		Touchpanel(void);
		~Touchpanel(void);

		void addToEventQueue(struct touchpanel_event e);	//only the thread should use this

		void getTouchState(struct touchstate *t_state);
		void getConfigMenuTouchState(struct touchstate *t_state);
		int touchpanel_fd, drv_fd;
};

#endif

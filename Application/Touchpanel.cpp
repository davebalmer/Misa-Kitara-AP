
#ifndef WIN32

#include <iostream>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#include "LCDFrameBuffer.h"

#include "GUI.h"

#include "Touchpanel.h"

#define TP_DEVICENAME	"/dev/event2"
#define NS_DEVICENAME	"/dev/nastech_ts"

void *touchscreen_thread(void *t)
{
	Touchpanel *ts = (Touchpanel *) t;

	fd_set rds;
	int ret;
	struct touchstate tsState;
	static int lastX,lastY;

	lastX = lastY = 0;

	std::queue<touchpanel_event> events;

	std::cout << "Touch panel started." << std::endl << std::flush;

	while(ts->threadRun)
	{
		FD_ZERO(&rds);
		FD_SET(ts->touchpanel_fd, &rds);

		ret = select(ts->touchpanel_fd + 1, &rds, NULL, NULL, NULL);

		if(ret < 0)
			std::cerr << "Touch panel error." << std::endl;

		if(ret == 0)
			std::cerr << "Touch panel time out." << std::endl;
		else
		if(FD_ISSET(ts->touchpanel_fd, &rds))
		{
			struct input_event new_event;
			if(read(ts->touchpanel_fd, &new_event, sizeof(struct input_event)) == -1)
			{
				if(errno != EAGAIN)
					std::cerr << "Touch panel read error." << std::endl;
			}
			else
			{
				struct touchpanel_event tpe;
				GUI_PID_STATE pid_state;
				read(ts->drv_fd, &tpe.data, 22);
				tpe.p_data = (unsigned short *) tpe.data;
				ts->addToEventQueue(tpe);
#if 0
				ts->getTouchState(&tsState);
				if(tsState.num_touches > tsState.old_num_touches)
				{
					pid_state.Pressed = 1;
					if(GetLcdFlip())
					{
						
						lastX = pid_state.x = 800-tsState.x[0];
						lastY = pid_state.y = 600-tsState.y[0];
					}
					else
					{
						lastX = pid_state.x = tsState.x[0];
						lastY = pid_state.y = tsState.y[0];
					}
					GUI_TOUCH_StoreStateEx(&pid_state);
				}
				else if(0 == tsState.num_touches)
				{
					pid_state.Pressed = 0;
					pid_state.x = lastX;
					pid_state.y = lastY;
					GUI_TOUCH_StoreStateEx(&pid_state);
				}
				else
				{
					pid_state.Pressed = 1;
					if(GetLcdFlip())
					{
						
						lastX = pid_state.x = 800-tsState.x[0];
						lastY = pid_state.y = 600-tsState.y[0];
					}
					else
					{
						lastX = pid_state.x = tsState.x[0];
						lastY = pid_state.y = tsState.y[0];
					}
					GUI_TOUCH_StoreStateEx(&pid_state);
				}
#endif
			}
		}
	}
	pthread_exit(NULL);
}

Touchpanel::Touchpanel(void)
{
	void *t = this;

	touchpanel_fd = open(TP_DEVICENAME, 0);
	if(touchpanel_fd < 0)
	{
		std::cerr << "Touch panel could not be opened." << std::endl;
	}

	drv_fd = open(NS_DEVICENAME, 0);
	if(drv_fd < 0)
	{
		std::cerr << "Touch panel could not be opened." << std::endl;
	}

	for(int i = 0; i < 5; i++)
	{
		setX(i, 0);
		setY(i, 0);
	}

	setNumTouches(0);
	setOldNumTouches(0);

	if(pthread_mutex_init(&tp_mutex, NULL))
		std::cerr << "Unable to initialise touch panel mutex." << std::endl;
	threadRun = 1;
	pthread_create(&tid, NULL, touchscreen_thread, t);
}

Touchpanel::~Touchpanel(void)
{
	std::cout << "~Touchpanel!"<<std::endl<<"Thread exited!"<<std::endl<<std::flush;
	close(touchpanel_fd);
	close(drv_fd);
	pthread_exit(NULL);
	pthread_mutex_destroy(&tp_mutex);
}

int Touchpanel::WaitForExit()
{
	int ret;
	threadRun = 0;
	ret = pthread_join(tid,NULL);
	close(touchpanel_fd);
	close(drv_fd);
	std::cout << "WaitForExit!"<<std::endl<<std::flush;
	return ret;
}

//this will pop off the queue
void Touchpanel::getTouchState(struct touchstate *t_state)
{
	if(touch_events.size() > 0)
	{
		pthread_mutex_lock(&tp_mutex);
		struct touchpanel_event touch_event = touch_events.front();
		pthread_mutex_unlock(&tp_mutex);

		if(touch_event.p_data[1] != 65535)
		{
			setX(0, touch_event.p_data[1]+1);
			setY(0, touch_event.p_data[2]+1);
		}
		else
		{
			setX(0, 0);
			setY(0, 0);
		}

		if(touch_event.p_data[3] != 65535)
		{
			setX(1, touch_event.p_data[3]+1);
			setY(1, touch_event.p_data[4]+1);
		}
		else
		{
			setX(1, 0);
			setY(1, 0);
		}

		if(touch_event.p_data[5] != 65535)
		{
			setX(2, touch_event.p_data[5]+1);
			setY(2, touch_event.p_data[6]+1);
		}
		else
		{
			setX(2, 0);
			setY(2, 0);
		}

		if(touch_event.p_data[7] != 65535)
		{
			setX(3, touch_event.p_data[7]+1);
			setY(3, touch_event.p_data[8]+1);
		}
		else
		{
			setX(3, 0);
			setY(3, 0);
		}

		if(touch_event.p_data[9] != 65535)
		{
			setX(4, touch_event.p_data[9]+1);
			setY(4, touch_event.p_data[10]+1);
		}
		else
		{
			setX(4, 0);
			setY(4, 0);
		}

/*		if(touch_event.data[1] == 255)
			setOldNumTouches(0);
		else
			setOldNumTouches(touch_event.data[1]);
*/
		setOldNumTouches(getNumTouches());
/*
		if(touch_event.data[0] == 255)
			setNumTouches(0);
		else
			setNumTouches(touch_event.data[0]);
*/
		int touch_count = 0;
		for(int i = 0; i < 5; i++)
			if(!((getX(i) == 0) && (getY(i) == 0)))
				touch_count++;

		setNumTouches(touch_count);

		pthread_mutex_lock(&tp_mutex);
//		delete(touch_event);
		touch_events.pop();
		pthread_mutex_unlock(&tp_mutex);
	}

	for(int i = 0; i < 5; i++)
	{
		t_state->x[i] = getX(i);
		t_state->y[i] = getY(i);
	}

	t_state->old_num_touches = getOldNumTouches();
	t_state->num_touches = getNumTouches();
}

unsigned int Touchpanel::getX(unsigned int id)
{
	return touch_state.x[id];
}

unsigned int Touchpanel::getY(unsigned int id)
{
	return touch_state.y[id];
}

unsigned char Touchpanel::getNumTouches(void)
{
	return touch_state.num_touches;
}

unsigned char Touchpanel::getOldNumTouches(void)
{
	return touch_state.old_num_touches;
}

void Touchpanel::setX(unsigned int id, unsigned int new_x)
{
	if(new_x == 0)
		touch_state.x[id] = 0;
	else
		touch_state.x[id] = 800-new_x;
}

void Touchpanel::setY(unsigned int id, unsigned int new_y)
{
	if(new_y == 0)
		touch_state.y[id] = 0;
	else
		touch_state.y[id] = 600-new_y;
}

void Touchpanel::setNumTouches(unsigned char num)
{
	touch_state.num_touches = num;
}

void Touchpanel::setOldNumTouches(unsigned char num)
{
	touch_state.old_num_touches = num;
}

void Touchpanel::addToEventQueue(struct touchpanel_event e)
{
	pthread_mutex_lock(&tp_mutex);
	touch_events.push(e);
	pthread_mutex_unlock(&tp_mutex);
}

#else // WIN32

#include "Touchpanel.h"

Touchpanel::Touchpanel(void)
{
}

Touchpanel::~Touchpanel(void)
{
}

int Touchpanel::WaitForExit()
{
	return 0;
}

int LCD_L0_Exit(void)
{
	return 0;
}

#endif // WIN32

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <linux/input.h>

#define PWK_DEVICENAME      "/dev/event4"

struct input_event ev;
typedef unsigned short u16;


int main(int argc, char **argv)
{
    fd_set rds;
    int ret;

    int kfd;
    int maxfd = 0;
		printf("Start powerkey app.\n");
    
	//////////////////////////
	kfd = open(PWK_DEVICENAME, 0);
    if (kfd < 0)
    {
        perror("can not open /dev/event4");
        exit(-1);
    }
	
	FD_ZERO(&rds);
	FD_SET(kfd, &rds);
    for (; ;)
    {  
		
		ret = select(kfd + 1, &rds, NULL, NULL, NULL);
        if (ret < 0)
        {
            perror("select");
            exit(-1);
        }
        if (ret == 0)
        {
            printf("Timeout.\n");
        }
        else 	
		if (FD_ISSET(kfd, &rds))
        {
            if (read(kfd, &ev, sizeof(struct input_event)) == -1)
            {
                if (errno != EAGAIN)
                {
                    perror("read power_fd\n");
                }
                continue;
            }
            else  if ((ev.type==EV_KEY) && (ev.code==KEY_POWER) && (ev.value==1))
            {
				//
				//To do: you can add some code here before system halt
				//
				printf("KEY_POWER PRESSED, run system(halt) now !\n");
				system("halt");
				break;
            }
        }
        
    }

    close(kfd);
    printf("exit power key app.\n");
    return (0);            
}

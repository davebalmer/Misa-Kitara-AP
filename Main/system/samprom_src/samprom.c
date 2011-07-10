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
#include <linux/delay.h>

#define SAM_DEVICENAME      "/dev/samdev"

#define MAXBYTES	16384	// only MAXBYTES+11 bytes in driver
#define MAXWORDS	8192

#define msleep(n) usleep(n*1000)

typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned long  u32;

char msgbuf[128];
char databuf[MAXBYTES+12];
struct timeval tmstart;
struct timeval tmend;
float time_use=0;
// usage: samprom [filename]
//
// get id:  samprom
// program: samprom /media/usrdata/soundbank/misa_101230.bin 
int main(int argc, char **argv)
{
    int i,ret;

    int sndfile = 0;
	int	samdrv = 0;
	char buff[64];
	char filename[128];
	u32 count,startaddr;
	
	//printf("Start samtest app.\n");
    if (argc == 1)	// get id
    {
		samdrv = open(SAM_DEVICENAME, O_RDWR);
		if (samdrv<0)
		{
			printf("can not open "SAM_DEVICENAME"\n");
	        return(1);
		}
		printf("Identity sam3716:");
		//-----------------------------
		// read identity cmd
		//-----------------------------
		buff[0]='I';
		ret = read(samdrv,buff,17);
		if(ret>=17)
		{	//<-:f0 7e 7f 06 02 00 20 00 00 07 11 7f 31 2e 30 34 f7
			for(i=0; i<17; i++)
			{
				sprintf(msgbuf+i*3+0,"%02x ",buff[i]);
			}
			printf("%s \n",msgbuf);
		} else {
			printf("\nIdentity error!\n");
		}
    }
	else if (argc == 2)	//program soundbank
    {
		samdrv = open(SAM_DEVICENAME, O_RDWR);
		if (samdrv<0)
		{
			printf("can not open "SAM_DEVICENAME"\n");
	        return(2);
		}
		strcpy(filename,argv[1]);
		sndfile = open(filename,O_RDONLY);
		if (sndfile<0)
		{
			printf("can not open %s \n",filename);
        	return(3);
		}
		// get soundbank file information
		struct stat st;
		stat(filename, &st);
		u32 wordsleft = (st.st_size & 1) ? ((st.st_size+1)>>1):(st.st_size>>1);
		//u32 wordsleft = (st.st_size & 1) ? ((st.st_size+1)):(st.st_size);			
		lseek(sndfile,0L,0);
		printf("soundbank file=%s size=%ld \n",filename,st.st_size);
		u32 nsector = st.st_size / 65536;
		nsector += (st.st_size % 65536)? 1:0;
		u16 delaytime = nsector * 180;	// for nsector erase timeout
		printf("waiting for erase flash,total sectors = %ld.\n",nsector);
		gettimeofday(&tmstart,NULL); //
		//-----------------------------
		// send start update cmd
		//-----------------------------
		startaddr = 0x00010000;
		buff[0]='S';
		buff[1]=(u8)(startaddr>>0);	//start address = 0x00010000
		buff[2]=(u8)(startaddr>>8);
		buff[3]=(u8)(startaddr>>16);
		buff[4]=(u8)(startaddr>>24);
		buff[5]=(u8)(wordsleft>>0);	//total words
		buff[6]=(u8)(wordsleft>>8);
		buff[7]=(u8)(wordsleft>>16);
		buff[8]=(u8)(wordsleft>>24);
		buff[9] =(u8)(delaytime );	//erase sectors timeout (ms)
		buff[10]=(u8)(delaytime>>8);
		ret = write(samdrv,buff,11);
		
		if (ret == 0x05)	// Ack OK
		{
			printf("start program flash.\n");
			while(wordsleft)
			{
				printf("addr:%08x left:%8ld\r\n",startaddr,wordsleft*2);
				
				// read data to buffer
				if (wordsleft>MAXWORDS)
				{
					count = MAXWORDS;
					wordsleft -= count;
				}
				else
				{
					count = wordsleft;
					wordsleft = 0;
				}
				read(sndfile,databuf+11,count*2);	//read data to buffer
				delaytime = 0;
				databuf[0]='W';
				databuf[1]=0xff & (startaddr    );
				databuf[2]=0xff & (startaddr>> 8);
				databuf[3]=0xff & (startaddr>>16);
				databuf[4]=0xff & (startaddr>>24);
				databuf[5]=0xff & (count    );
				databuf[6]=0xff & (count>> 8);
				databuf[7]=0xff & (count>>16);
				databuf[8]=0xff & (count>>24);
				databuf[9]=0xff  & (delaytime);		// packet wait ack timeout (ms)
				databuf[10]=0xff & (delaytime>>8);
				ret = write(samdrv,databuf,count*2+11);	//write count words to sam3716
				if ((ret<=0)||(ret!=5)) break;
				startaddr+=count;
				//msleep(10);
			}
			if (ret==5)
				printf("\nprogram finish!\n");		
			else
			{	// error
				printf("\nprogram error! address=%08x\n",startaddr);
			}
		}
		else 
		{	//start update cmd error
			printf("send start update cmd error! ret=%d\n",ret);
			if (samdrv) close(samdrv);
			if (sndfile) close(sndfile);
			return(4);
		}
		gettimeofday(&tmend,NULL); 
		//time_use=(tmend.tv_sec-tmstart.tv_sec)*1000000+ (tmend.tv_usec-tmstart.tv_usec);//us
		time_use=0.000001*((tmend.tv_sec-tmstart.tv_sec)*1000000+ (tmend.tv_usec-tmstart.tv_usec));
		printf("time use is %f sec\n",time_use);
    }
	else
	{
		printf("incorrect argc.\n");
	}
		
	if (samdrv) close(samdrv);
	if (sndfile) close(sndfile);
    return (0);            
}

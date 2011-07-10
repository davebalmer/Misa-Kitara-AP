
#ifndef __DELAYSCREEN_H__
#define __DELAYSCREEN_H__

typedef enum __DELAY_ID
{
	DELAY_ID_MONO=GUI_ID_DELAY_BASE,
	DELAY_ID_STEREO,
	DELAY_ID_PROGPRELPF,
	DELAY_ID_PROGTIME,
	DELAY_ID_PROGLEVEL,
	DELAY_ID_PROGFDBK,
	DELAY_ID_PROGFDBKFILTER,
	DELAY_ID_MAX
} DELAY_ID;

typedef struct __DELAY_INFO
{
	U8 Fx;
	U8 On;
	U8 monostereo;
	U8 pre_lpf;
	U8 time;
	U8 level;
	U8 fdbk;
	U8 fdbkfilter;
} DELAY_INFO,*PDELAY_INFO;

U8 CreateDelayScreen(void* pPara);
U8 DeleteDelayScreen();
U8 TopDelayScreen(WM_HWIN hPreWin);
U8 UpdateDelayInfo(PDELAY_INFO pInfo);
U8 GetDelayInfo(PDELAY_INFO pInfo);

#endif // __DELAYSCREEN_H__


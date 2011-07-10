
#ifndef __REVERBSCREEN_H__
#define __REVERBSCREEN_H__

typedef enum __REVERB_ID
{
	REVERB_ID_LEFTARROW=GUI_ID_REVERB_BASE,
	REVERB_ID_RIGHTARROW,
	REVERB_ID_PROGPRELPF,
	REVERB_ID_PROGCHAR,
	REVERB_ID_PROGLEVEL,
	REVERB_ID_PROGTIME,
	REVERB_ID_PROGDELAYFDBK,
	REVERB_ID_PROGDELAYTIME,
	REVERB_ID_MAX
} REVERB_ID;

typedef struct __REVERB_INFO
{
	U8 type;
	U8 pre_lpf;
	U8 character;
	U8 level;
	U8 time;
	U8 delayfdbk;
	U8 delaytime;
} REVERB_INFO,*PREVERB_INFO;

U8 CreateReverbScreen(void* pPara);
U8 DeleteReverbScreen();
U8 TopReverbScreen(WM_HWIN hPreWin);
U8 UpdateReverbInfo(PREVERB_INFO pInfo);
U8 GetReverbInfo(PREVERB_INFO pInfo);

#endif // __REVERBSCREEN_H__


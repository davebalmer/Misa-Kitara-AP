
#ifndef __DISTORTION_H__
#define __DISTORTION_H__

typedef enum __DISTORTION_ID
{
	DISTORTION_ID_LEFTARROW=GUI_ID_DISTORTION_BASE,
	DISTORTION_ID_RIGHTARROW,
	DISTORTION_ID_PROGDRIVE,
	DISTORTION_ID_PROGTONE,
	DISTORTION_ID_PROGLEVEL,
	DISTORTION_ID_PROGNOISEREDUCTION,
	DISTORTION_ID_PROGBOOSTER,
	//DISTORTION_ID_POPMENU,
	DISTORTION_ID_MAX
} DISTORTION_ID;

typedef struct __DISTORTION_INFO
{
	U8 Fx;
	U8 On;
	U8 type;
	U8 drive;
	U8 tone;
	U8 level;
	U8 noisereduction;
	U8 booster;
} DISTORTION_INFO,*PDISTORTION_INFO;

U8 CreateDistortionScreen(void* pPara);
U8 DeleteDistortionScreen();
U8 TopDistortionScreen(WM_HWIN hPreWin);
U8 UpdateDistortionInfo(PDISTORTION_INFO pInfo);
U8 GetDistortionInfo(PDISTORTION_INFO pInfo);

#endif // __DISTORTION_H__


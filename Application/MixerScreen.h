
#ifndef __MIXERSCREEN_H__
#define __MIXERSCREEN_H__

typedef enum __MIXER_ID
{
	MIXER_TOEQSCREEN_ID=GUI_ID_MIXER_BASE,
	MIXER_PROGLOWCUT_ID,
	MIXER_PROGHIGHCUT_ID,
	MIXER_PROGINPUTGAIN_ID,
	MIXER_PROGLEVEL_ID,
	MIXER_PROGREVERBSEND_ID,
	MIXER_PROGPAN_ID,
	MIXER_ID_MAX
} MIXER_ID;

typedef struct __MIXER_INFO
{
	U8 Fx;
	U8 lowcut;
	U8 highcut;
	U8 inputgain;
	U8 level;
	U8 reverbsend;
	U8 pan;
} MIXER_INFO,*PMIXER_INFO;

U8 CreateMixerScreen(void* pPara);
U8 DeleteMixerScreen();
U8 TopMixerScreen(WM_HWIN hPreWin);
U8 UpdateMixerInfo(PMIXER_INFO pInfo);
U8 GetMixerInfo(PMIXER_INFO pInfo);

#endif // __MIXERSCREEN_H__



#ifndef __VOICESCREEN_H__
#define __VOICESCREEN_H__

typedef enum __VOICE_ID
{
	VOICE_ID_DELETE=GUI_ID_VOICE_BASE,
	VOICE_ID_LEFTARROW,
	VOICE_ID_RIGHTARROW,
	VOICE_ID_AMPATTACK,
	VOICE_ID_AMPDECAY,
	VOICE_ID_AMPRELEASE,
	VOICE_ID_FILTFREQ,
	VOICE_ID_FILTRES,
	VOICE_ID_DETUNECOURSE,
	VOICE_ID_DETUNEFINE,
	VOICE_ID_VIBRATERATE,
	VOICE_ID_VIBRATEDEPTH,
	VOICE_ID_VIBRATEDELAY,
	VOICE_ID_PORTMTIME,
	VOICE_ID_PAN,
	VOICE_ID_PITCHWSEMITONES,
	VOICE_ID_PITCHWHEEL,
	VOICE_ID_VELOCITY,
	VOICE_ID_REVERBSEND,
	VOICE_ID_LEVEL,
	VOICE_ID_FILTER_TYPE,
	VOICE_ID_FILTER_ATTACK,
	VOICE_ID_FILTER_DECAY,
	VOICE_ID_FILTER_RELEASE,
	VOICE_ID_MAX
} VOICE_ID;

typedef struct __VOICE_INFO
{
	U8 available;
	U8 Fx; // 0: none; 1:Fx1; 2:Fx2
	U8 wave;
	U8 ampattack;
	U8 ampdecay;
	U8 amprelease;
	U8 filtfreq;
	U8 filtres;
	U8 detunecource;
	U8 detunefine;
	U8 vibrate;
	U8 vibdepth;
	U8 vibdelay;
	U8 portmtime;
	U8 pan;
	U8 pitchWsemitones;
	U8 pitchwheel;
	U8 volecity;
	U8 reverbsend;
	U8 level;
	U8 filter_type;
	U8 filter_attack;
	U8 filter_decay;
	U8 filter_release;
} VOICE_INFO,*PVOICE_INFO;

U8 CreateVoiceScreen(void* pPara);
U8 DeleteVoiceScreen();
U8 TopVoiceScreen(WM_HWIN hPreWin);
U8 UpdateVoiceInfo(PVOICE_INFO pInfo);
U8 GetVoiceInfo(PVOICE_INFO pInfo);

#endif// __VOICESCREEN_H__



#ifndef __SYNTHSCREEN_H__
#define __SYNTHSCREEN_H__

#include "VoiceScreen.h"

typedef struct __SYNTH_INFO
{
	U8 tune[6];
	PVOICE_INFO aVoicept1[5];
	PVOICE_INFO aVoicept2[5];
	PVOICE_INFO aVoicept3[5];
	PVOICE_INFO aVoicept4[5];
	PVOICE_INFO aVoicept5[5];
	PVOICE_INFO aVoicept6[5];
} SYNTH_INFO,*PSYNTH_INFO;

U8 CreateSynthScreen(void* pPara);
U8 DeleteSynthScreen();
U8 TopSynthScreen(WM_HWIN hPreWin);
U8 UpdateSynthInfo(PSYNTH_INFO pInfo);
U8 GetSynthInfo(PSYNTH_INFO pInfo);
U8 SynthEditVoice();
U8 SynthDeleteVoice();
int SynthGetStringIndex();
int SynthGetStringIndex(GUI_HWIN CurSynthStringPanel);	// OR : Drag & Drop
int SynthGetVoiceIndex();
WM_HWIN SynthScreen_GetStringHWin(int &x, int &y);

#endif // __SYNTHSCREEN_H__


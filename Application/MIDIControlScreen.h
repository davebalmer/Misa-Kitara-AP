
#ifndef __MIDICONTROLSCREEN_H__
#define __MIDICONTROLSCREEN_H__

typedef struct __MIDICTRL_INFO
{
	U32 control;
	U8 type;
	U8 channel;
	U8 string;
	U32 cc;
	U8 inv;
} MIDICTRL_INFO,*PMIDICTRL_INFO;

U8 CreateMIDIControlScreen(void* pPara);
U8 DeleteMIDIControlScreen();
U8 TopMIDIControlScreen(WM_HWIN hPreWin);
U8 UpdateMIDIControlInfo(PMIDICTRL_INFO pInfo);
U8 GetMIDIControlInfo(PMIDICTRL_INFO pInfo);

#endif // __MIDICONTROLSCREEN_H__



#include <cstdlib>
#include <iostream>

#include "GUI.h"
#include "WM.h"
#include "BUTTON.h"

#include "MisaWidget.h"

#include "MisaAppInc.h"

#include "MainMenu.h"
#include "ControlAssignments.h"
#include "MIDIControlScreen.h"
#include "VoiceScreen.h"
#include "SynthScreen.h"

#include "MisamenuConfig.h"

#include "Resource.h"

///////////////////////////////////////////////////////////////////////////////
//

#define SYNTH_TYPE_XPOS 80
#define SYNTH_TYPE_YPOS 120

#define SYNTH_XOFFSET 2
#define SYNTH_YOFFSET 2

#define VOICE_CNT_ZERO 0x00
#define VOICE_CNT_ONE 0x01
#define VOICE_CNT_TOW 0x03
#define VOICE_CNT_THREE 0x07
#define VOICE_CNT_FOUR 0x0F
#define VOICE_CNT_FIVE 0x1F

///////////////////////////////////////////////////////////////////////////////
//

typedef enum __SYNTHITEMS
{
	SYNTH_MIDICTRL,
	SYNTH_STRINGPANEL1,
	SYNTH_STRINGPANEL2,
	SYNTH_STRINGPANEL3,
	SYNTH_STRINGPANEL4,
	SYNTH_STRINGPANEL5,
	SYNTH_STRINGPANEL6,
	SYNTH_MAX
} SYNTHITEMS;

typedef enum __SYNTH_ID
{
	SYNTH_ID_MIDICTRL=GUI_ID_SYNTH_BASE,
	SYNTH_ID_STRINGPANEL1,
	SYNTH_ID_STRINGPANEL2,
	SYNTH_ID_STRINGPANEL3,
	SYNTH_ID_STRINGPANEL4,
	SYNTH_ID_STRINGPANEL5,
	SYNTH_ID_STRINGPANEL6,
	SYNTH_ID_MAX
} SYNTH_ID;

///////////////////////////////////////////////////////////////////////////////
//

static WM_HWIN hSynth;
static void SynthProc(WM_MESSAGE* pMsg);
static WM_HWIN hSynthItems[SYNTH_MAX];
static COMMCOMPONENT hCOMPcomm;
static WM_HWIN hCurSynthStringPanel;

static void SynthControlMenuProc(int menuId);

///////////////////////////////////////////////////////////////////////////////
//

U8 CreateSynthScreen(void* pPara)
{
	int x,y;
	x=GUI_GetScreenSizeX();
	y=GUI_GetScreenSizeY();
	hSynth = WM_CreateWindow(0, 0, x, y, /*WM_CF_SHOW | */WM_CF_MEMDEV, SynthProc, 0);
	CreateMIDIControlScreen(0);
	CreateControlAssignmentsScreen(0);
	CreateVoiceScreen(0);
	return 0;
}

static U8 SynthCreateItems(WM_HWIN hParent)
{
	int x,y;
	hCOMPcomm = CreateCommComponentEx(hParent,COMMON_ITEM_CLOSE);
	memset(hSynthItems,0,sizeof(hSynthItems));
	// Panel 1
	x = 4;
	y = bmEMPTYTITLEBAR.YSize;
	//y += SYNTH_YOFFSET;
	y += 1;
	hSynthItems[SYNTH_STRINGPANEL1] = MisaSynthString_Create(x,y,0,0,hParent,SYNTH_ID_STRINGPANEL1);
	y += STRING_PANEL_HEIGHT;
	y += 1;
	hSynthItems[SYNTH_STRINGPANEL2] = MisaSynthString_Create(x,y,0,0,hParent,SYNTH_ID_STRINGPANEL2);
	y += STRING_PANEL_HEIGHT;
	y += 1;
	hSynthItems[SYNTH_STRINGPANEL3] = MisaSynthString_Create(x,y,0,0,hParent,SYNTH_ID_STRINGPANEL3);
	y += STRING_PANEL_HEIGHT;
	y += 1;
	hSynthItems[SYNTH_STRINGPANEL4] = MisaSynthString_Create(x,y,0,0,hParent,SYNTH_ID_STRINGPANEL4);
	y += STRING_PANEL_HEIGHT;
	y += 1;
	hSynthItems[SYNTH_STRINGPANEL5] = MisaSynthString_Create(x,y,0,0,hParent,SYNTH_ID_STRINGPANEL5);
	y += STRING_PANEL_HEIGHT;
	y += 1;
	hSynthItems[SYNTH_STRINGPANEL6] = MisaSynthString_Create(x,y,0,0,hParent,SYNTH_ID_STRINGPANEL6);

	x = SYNTH_XOFFSET;
	y = 502;
	hSynthItems[SYNTH_MIDICTRL] = BUTTON_CreateAsChild(x,y,bmSM_MIDICTL_UN.XSize,bmSM_MIDICTL_UN.YSize,hParent,SYNTH_ID_MIDICTRL,WM_CF_SHOW|WM_CF_MEMDEV);
	BUTTON_SetFocussable(hSynthItems[SYNTH_MIDICTRL],0);
	BUTTON_SetBitmap(hSynthItems[SYNTH_MIDICTRL],BUTTON_CI_UNPRESSED,&bmSM_MIDICTL_UN);
	BUTTON_SetBitmap(hSynthItems[SYNTH_MIDICTRL],BUTTON_CI_PRESSED,&bmSM_MIDICTL_SE);
	return 0;
}

static U8 SynthDeleteItems()
{
	if(hSynthItems[SYNTH_STRINGPANEL1])
	{
		MisaSynthString_Delete(hSynthItems[SYNTH_STRINGPANEL1]);
		hSynthItems[SYNTH_STRINGPANEL1] = 0;
	}
	if(hSynthItems[SYNTH_STRINGPANEL2])
	{
		MisaSynthString_Delete(hSynthItems[SYNTH_STRINGPANEL2]);
		hSynthItems[SYNTH_STRINGPANEL2] = 0;
	}
	if(hSynthItems[SYNTH_STRINGPANEL3])
	{
		MisaSynthString_Delete(hSynthItems[SYNTH_STRINGPANEL3]);
		hSynthItems[SYNTH_STRINGPANEL3] = 0;
	}
	if(hSynthItems[SYNTH_STRINGPANEL4])
	{
		MisaSynthString_Delete(hSynthItems[SYNTH_STRINGPANEL4]);
		hSynthItems[SYNTH_STRINGPANEL4] = 0;
	}
	if(hSynthItems[SYNTH_STRINGPANEL5])
	{
		MisaSynthString_Delete(hSynthItems[SYNTH_STRINGPANEL5]);
		hSynthItems[SYNTH_STRINGPANEL5] = 0;
	}
	if(hSynthItems[SYNTH_STRINGPANEL6])
	{
		MisaSynthString_Delete(hSynthItems[SYNTH_STRINGPANEL6]);
		hSynthItems[SYNTH_STRINGPANEL6] = 0;
	}
	if(hSynthItems[SYNTH_MIDICTRL])
	{
		BUTTON_Delete(hSynthItems[SYNTH_MIDICTRL]);
		hSynthItems[SYNTH_MIDICTRL] = 0;
	}
	DeleteCommComponent(hCOMPcomm);
	return 0;
}

U8 DeleteSynthScreen()
{
	DeleteVoiceScreen();
	RunFramework();
	DeleteControlAssignmentsScreen();
	RunFramework();
	DeleteMIDIControlScreen();
	RunFramework();
	SynthDeleteItems();
	WM_DeleteWindow(hSynth);
	return 0;
}

U8 TopSynthScreen(WM_HWIN hPreWin)
{
	UpdateSynthSettingEx(MISAMODULE_SYNTH); //MZ
	WM_HideWindow(hPreWin);
	WM_ShowWindow(hSynth);
	//WM_BringToTop(hSynth);
	return 0;
}

U8 UpdateSynthInfo(PSYNTH_INFO pInfo)
{
	int i,j,k,voice,mask;
	PVOICE_INFO pVoice;
	for(i=0;i<6;i++)
	{
		MisaSynthString_SetTune(hSynthItems[SYNTH_STRINGPANEL1+i],pInfo->tune[i]);
		MisaSynthString_SwitchSynthMode(hSynthItems[SYNTH_STRINGPANEL1+i],SynthGetMidiOutChannel(i)==-1?0:1,SynthGetMidiOutChannel(i));
		voice = 0;
		k = 0;//MisaSynthString_GetVoiceIndex(hSynthItems[SYNTH_STRINGPANEL1+i]);
		mask = 0x00000003;
		for(j=0;j<5;j++)
		{
			switch(i)
			{
			case 0:
				pVoice = pInfo->aVoicept1[j];
				break;
			case 1:
				pVoice = pInfo->aVoicept2[j];
				break;
			case 2:
				pVoice = pInfo->aVoicept3[j];
				break;
			case 3:
				pVoice = pInfo->aVoicept4[j];
				break;
			case 4:
				pVoice = pInfo->aVoicept5[j];
				break;
			case 5:
			default:
				pVoice = pInfo->aVoicept6[j];
			}
			if(pVoice->available)
			{
				voice |= mask;
				mask <<= 2;
				k++;
			}
		}
		if(k>0)
		{
			--k;
		}
		voice |= k<<16;
		MisaSynthString_SetVoice(hSynthItems[SYNTH_STRINGPANEL1+i],voice);
	}
	return 0;
}

U8 GetSynthInfo(PSYNTH_INFO pInfo)
{
	int i;
	for(i=0;i<5;i++)
	{
		pInfo->aVoicept1[i] = (PVOICE_INFO)MisaSynthString_GetVoice(hSynthItems[SYNTH_STRINGPANEL1],i);
		if(pInfo->aVoicept1[i])
		{
			memset(pInfo->aVoicept1[i],0,sizeof(VOICE_INFO));
		}
	}
	for(i=0;i<5;i++)
	{
		pInfo->aVoicept2[i] = (PVOICE_INFO)MisaSynthString_GetVoice(hSynthItems[SYNTH_STRINGPANEL2],i);
		if(pInfo->aVoicept2[i])
		{
			memset(pInfo->aVoicept2[i],0,sizeof(VOICE_INFO));
		}
	}
	for(i=0;i<5;i++)
	{
		pInfo->aVoicept3[i] = (PVOICE_INFO)MisaSynthString_GetVoice(hSynthItems[SYNTH_STRINGPANEL3],i);
		if(pInfo->aVoicept3[i])
		{
			memset(pInfo->aVoicept3[i],0,sizeof(VOICE_INFO));
		}
	}
	for(i=0;i<5;i++)
	{
		pInfo->aVoicept4[i] = (PVOICE_INFO)MisaSynthString_GetVoice(hSynthItems[SYNTH_STRINGPANEL4],i);
		if(pInfo->aVoicept4[i])
		{
			memset(pInfo->aVoicept4[i],0,sizeof(VOICE_INFO));
		}
	}
	for(i=0;i<5;i++)
	{
		pInfo->aVoicept5[i] = (PVOICE_INFO)MisaSynthString_GetVoice(hSynthItems[SYNTH_STRINGPANEL5],i);
		if(pInfo->aVoicept5[i])
		{
			memset(pInfo->aVoicept5[i],0,sizeof(VOICE_INFO));
		}
	}
	for(i=0;i<5;i++)
	{
		pInfo->aVoicept6[i] = (PVOICE_INFO)MisaSynthString_GetVoice(hSynthItems[SYNTH_STRINGPANEL6],i);
		if(pInfo->aVoicept6[i])
		{
			memset(pInfo->aVoicept6[i],0,sizeof(VOICE_INFO));
		}
	}
	return 0;
}

static void SynthProc(WM_MESSAGE* pMsg)
{
	int x;
	int NCode,Id;
	ProcessCommComponent(pMsg, &hCOMPcomm);
	switch (pMsg->MsgId)
	{
	case WM_CREATE:
		SynthCreateItems(pMsg->hWin);
		hCurSynthStringPanel = 0;
		break;
	case WM_DELETE:
		SynthDeleteItems();
		break;
	case WM_PAINT:
		GUI_SetBkColor(GUI_BLACK);
		GUI_Clear();
		GUI_SetTextMode(GUI_TEXTMODE_TRANS);
		GUI_DrawBitmap(&bmEMPTYTITLEBAR, 0, 0);
		GUI_SetFont(&GUI_Font32B_ASCII);
		x = WM_GetWindowSizeX(pMsg->hWin);
		GUI_DispStringHCenterAt("Synth / MIDI", x / 2, 5);
		//PaintSynthCombo();
		break;
	case WM_NOTIFY_PARENT:
		if(DispatchCommComponent(pMsg,&hCOMPcomm))
		{
			Id = WM_GetId(pMsg->hWinSrc);
			NCode = pMsg->Data.v;
			switch(NCode)
			{
			case WM_NOTIFICATION_ADDVOICE:
				switch(Id)
				{
				case SYNTH_ID_STRINGPANEL1:
					SynthInsertVoice(0,0);
					break;
				case SYNTH_ID_STRINGPANEL2:
					SynthInsertVoice(1,0);
					break;
				case SYNTH_ID_STRINGPANEL3:
					SynthInsertVoice(2,0);
					break;
				case SYNTH_ID_STRINGPANEL4:
					SynthInsertVoice(3,0);
					break;
				case SYNTH_ID_STRINGPANEL5:
					SynthInsertVoice(4,0);
					break;
				case SYNTH_ID_STRINGPANEL6:
					SynthInsertVoice(5,0);
					break;
				}
				hCurSynthStringPanel = pMsg->hWinSrc;
				UpdateSynthSettingEx(MISAMODULE_SYNTH);
				UpdateVoiceInfo((PVOICE_INFO)MisaSynthString_GetVoice(hCurSynthStringPanel,MisaSynthString_GetVoiceIndex(hCurSynthStringPanel)));
				TopVoiceScreen(pMsg->hWin);
				break;
			case WM_NOTIFICATION_CLICKED:
				DEBUG_STDOUT("SYNTH button clicked!");
				break;
			case WM_NOTIFICATION_RELEASED:
				switch(Id)
				{
				case SYNTH_ID_MIDICTRL:
					TopControlAssignmentsScreen(pMsg->hWin);
					break;
				case SYNTH_ID_STRINGPANEL1:
				case SYNTH_ID_STRINGPANEL2:
				case SYNTH_ID_STRINGPANEL3:
				case SYNTH_ID_STRINGPANEL4:
				case SYNTH_ID_STRINGPANEL5:
				case SYNTH_ID_STRINGPANEL6:
					hCurSynthStringPanel = pMsg->hWinSrc;
					//UpdateSynthSettingEx(MISAMODULE_SYNTH);
					UpdateVoiceInfo((PVOICE_INFO)MisaSynthString_GetVoice(hCurSynthStringPanel,MisaSynthString_GetVoiceIndex(hCurSynthStringPanel)));
					TopVoiceScreen(pMsg->hWin);
					break;
				case CTL_NONE_ID:
				case CTL_TOUCH_X_ID:
				case CTL_TOUCH_Y_ID:
				case CTL_DRAG_X_ID:
				case CTL_DRAG_Y_ID:
				case CTL_BALL_X_ID:
				case CTL_BALL_Y_ID:
				case CTL_VARIATION_ID:
					break;
				default:
					;
				}
				DEBUG_STDOUT("SYNTH button released!");
				break;
			case WM_NOTIFICATION_VALUE_CHANGED:
				SynthTurnNotesOff();
				switch(Id)
				{
				case SYNTH_ID_STRINGPANEL1:
					SynthTuning(0,MisaSynthString_GetTune(pMsg->hWinSrc));
					break;
				case SYNTH_ID_STRINGPANEL2:
					SynthTuning(1,MisaSynthString_GetTune(pMsg->hWinSrc));
					break;
				case SYNTH_ID_STRINGPANEL3:
					SynthTuning(2,MisaSynthString_GetTune(pMsg->hWinSrc));
					break;
				case SYNTH_ID_STRINGPANEL4:
					SynthTuning(3,MisaSynthString_GetTune(pMsg->hWinSrc));
					break;
				case SYNTH_ID_STRINGPANEL5:
					SynthTuning(4,MisaSynthString_GetTune(pMsg->hWinSrc));
					break;
				case SYNTH_ID_STRINGPANEL6:
					SynthTuning(5,MisaSynthString_GetTune(pMsg->hWinSrc));
					break;
				default:
					;
				}
				break;
			case WM_NOTIFICATION_SEL_CHANGED:
				switch(Id)
				{
				case SYNTH_ID_STRINGPANEL1:
					SynthSetMidiOutChannel(0,MisaSynthString_GetChannel(pMsg->hWinSrc));
					break;
				case SYNTH_ID_STRINGPANEL2:
					SynthSetMidiOutChannel(1,MisaSynthString_GetChannel(pMsg->hWinSrc));
					break;
				case SYNTH_ID_STRINGPANEL3:
					SynthSetMidiOutChannel(2,MisaSynthString_GetChannel(pMsg->hWinSrc));
					break;
				case SYNTH_ID_STRINGPANEL4:
					SynthSetMidiOutChannel(3,MisaSynthString_GetChannel(pMsg->hWinSrc));
					break;
				case SYNTH_ID_STRINGPANEL5:
					SynthSetMidiOutChannel(4,MisaSynthString_GetChannel(pMsg->hWinSrc));
					break;
				case SYNTH_ID_STRINGPANEL6:
					SynthSetMidiOutChannel(5,MisaSynthString_GetChannel(pMsg->hWinSrc));
					break;
				default:
					;
				}
				break;
			case WM_NOTIFICATION_STRINGPANEL_SYNTH:
				switch(Id)
				{
				case SYNTH_ID_STRINGPANEL1:
					SynthSetMidiOutChannel(0,-1);
					break;
				case SYNTH_ID_STRINGPANEL2:
					SynthSetMidiOutChannel(1,-1);
					break;
				case SYNTH_ID_STRINGPANEL3:
					SynthSetMidiOutChannel(2,-1);
					break;
				case SYNTH_ID_STRINGPANEL4:
					SynthSetMidiOutChannel(3,-1);
					break;
				case SYNTH_ID_STRINGPANEL5:
					SynthSetMidiOutChannel(4,-1);
					break;
				case SYNTH_ID_STRINGPANEL6:
					SynthSetMidiOutChannel(5,-1);
					break;
				default:
					;
				}
				break;
			}
		}
		break;
	case WM_TOUCH:
		break;
	default:
		WM_DefaultProc(pMsg);
	}
}

U8 SynthEditVoice()
{
	if(hCurSynthStringPanel)
	{
		GetVoiceInfo((PVOICE_INFO)MisaSynthString_GetVoice(hCurSynthStringPanel,MisaSynthString_GetVoiceIndex(hCurSynthStringPanel)));
		hCurSynthStringPanel = 0;
	}
	return 0;
}

U8 SynthDeleteVoice()
{
	if(hCurSynthStringPanel)
	{
		SynthTurnNotesOff();
		SynthRemoveVoice(WM_GetId(hCurSynthStringPanel)-SYNTH_ID_STRINGPANEL1,MisaSynthString_GetVoiceIndex(hCurSynthStringPanel));
		MisaSynthString_DelSynthoice(hCurSynthStringPanel);
		hCurSynthStringPanel = 0;
	}
	return 0;
}

int SynthGetStringIndex()
{
	if(hCurSynthStringPanel)
	{
		return WM_GetId(hCurSynthStringPanel)-SYNTH_ID_STRINGPANEL1;
	}
	else
	{
		return -1;
	}
}

// OR : Drag & Drop
int SynthGetStringIndex(GUI_HWIN CurSynthStringPanel)
{
	return WM_GetId(CurSynthStringPanel)-SYNTH_ID_STRINGPANEL1;
}
int SynthGetVoiceIndex()
{
	if(hCurSynthStringPanel)
	{
		return MisaSynthString_GetVoiceIndex(hCurSynthStringPanel);
	}
	else
	{
		return -1;
	}
}

// Find if the point is over a Synth String Panel and, if yes, the function returns the handle on the window found
// and update the coordinates relatively  to this window.
WM_HWIN SynthScreen_GetStringHWin(int &x, int &y)
{
	for(int i = 0; i < 6; i++)
	{
		GUI_RECT rect;
		WM_GetWindowRectEx(hSynthItems[SYNTH_STRINGPANEL1+i], &rect);
		if (x >= rect.x0 && y >= rect.y0 && x <= rect.x1 && y <= rect.y1)
		{
			x -= rect.x0;
			y -= rect.y0;
			return hSynthItems[SYNTH_STRINGPANEL1+i];
		}
	}
	return NULL;
}

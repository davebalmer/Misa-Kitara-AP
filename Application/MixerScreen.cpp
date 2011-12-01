
#include <cstdlib>
#include <iostream>

#include "GUI.h"
#include "WM.h"
#include "BUTTON.h"

#include "MisaWidget.h"

#include "MisaAppInc.h"

#include "MainMenu.h"
#include "EqScreen.h"

#include "MixerScreen.h"

#include "MisamenuConfig.h"

#include "Resource.h"

///////////////////////////////////////////////////////////////////////////////
//

#define MIXER_TYPE_XPOS 80
#define MIXER_TYPE_YPOS 120

#define MIXER_XOFFSET 2
#define MIXER_YOFFSET 2


// No more sliding
static const GUI_RECT leftsidebutton=
{
	0,64,39,498
};
static const GUI_RECT rightsidebutton=
{
	760,64,799,498
};

///////////////////////////////////////////////////////////////////////////////
//

typedef enum __MIXERITEMS
{
	MIXER_INDICATOR,
	MIXER_SLIDEWINDOW,
	MIXER_TOEQSCREEN,
	MIXER_PROGLOWCUT,
	MIXER_PROGHIGHCUT,
	MIXER_PROGINPUTGAIN,
	MIXER_PROGLEVEL,
	MIXER_PROGREVERBSEND,
	MIXER_PROGPAN,
	MIXER_MAX
} MIXERITEMS;

///////////////////////////////////////////////////////////////////////////////
//

static WM_HWIN hMixer;
static void MixerProc(WM_MESSAGE* pMsg);
static WM_HWIN hMixerItems[MIXER_MAX];
static COMMCOMPONENT hCOMPcomm;

static WM_HWIN CreateSlidePanel(int x0, int y0, int width, int height, WM_HWIN hParent, U16 Style);
static U8 DeleteSlidePanel(WM_HWIN hSlidePanel);
static U8 SlidingBorder;

static int MixerGetFX();
static U8 MixerLoadParameter(int index, U8 update);
static void MixerControlMenuProc(int menuId);

///////////////////////////////////////////////////////////////////////////////
//

U8 CreateMixerScreen(void* pPara)
{
	int x,y;
	x=GUI_GetScreenSizeX();
	y=GUI_GetScreenSizeY();
	SlidingBorder = 1;
	hMixer = WM_CreateWindow(0, 0, x, y, /*WM_CF_SHOW | */WM_CF_MEMDEV, MixerProc, 0);
	return 0;
}

U8 DeleteMixerScreen()
{
	WM_DeleteWindow(hMixer);
	return 0;
}

U8 TopMixerScreen(WM_HWIN hPreWin)
{
	WM_HideWindow(hPreWin);
	WM_ShowWindow(hMixer);
	//WM_BringToTop(hMixer);
	SetWindowToUpdateIfPresetModified(hMixer);
	return 0;
}

U8 UpdateMixerInfo(PMIXER_INFO pInfo)
{
	std::vector<int> effect;
	if(pInfo->Fx != MixerGetFX())
	{
		MisaRadio_SetStatus(hCOMPcomm.hFX1,pInfo->Fx?0:1);
		MisaRadio_SetStatus(hCOMPcomm.hFX2,pInfo->Fx?1:0);
	}
	MisaProgressbar_SetPercentEx(hMixerItems[MIXER_PROGLOWCUT],pInfo->lowcut,0);
	MisaProgressbar_SetPercentEx(hMixerItems[MIXER_PROGHIGHCUT],127-pInfo->highcut,0);
	MisaProgressbar_SetPercentEx(hMixerItems[MIXER_PROGINPUTGAIN],pInfo->inputgain,0);
	MisaProgressbar_SetPercentEx(hMixerItems[MIXER_PROGLEVEL],pInfo->level,0);
	MisaProgressbar_SetPercentEx(hMixerItems[MIXER_PROGREVERBSEND],pInfo->reverbsend,0);
	MisaProgressbar_SetPercentEx(hMixerItems[MIXER_PROGPAN],pInfo->pan,0);
	
	effect = SynthFindEffect(SynthTranslateEffect(MIXER_PROGLOWCUT_ID),MixerGetFX());
	MisaProgressbar_SetCheck(hMixerItems[MIXER_PROGLOWCUT], effect.size()?1:0);
	effect = SynthFindEffect(SynthTranslateEffect(MIXER_PROGHIGHCUT_ID),MixerGetFX());
	MisaProgressbar_SetCheck(hMixerItems[MIXER_PROGHIGHCUT], effect.size()?1:0);
	effect = SynthFindEffect(SynthTranslateEffect(MIXER_PROGINPUTGAIN_ID),MixerGetFX());
	MisaProgressbar_SetCheck(hMixerItems[MIXER_PROGINPUTGAIN], effect.size()?1:0);
	effect = SynthFindEffect(SynthTranslateEffect(MIXER_PROGLEVEL_ID),MixerGetFX());
	MisaProgressbar_SetCheck(hMixerItems[MIXER_PROGLEVEL], effect.size()?1:0);
	effect = SynthFindEffect(SynthTranslateEffect(MIXER_PROGREVERBSEND_ID),MixerGetFX());
	MisaProgressbar_SetCheck(hMixerItems[MIXER_PROGREVERBSEND], effect.size()?1:0);
	effect = SynthFindEffect(SynthTranslateEffect(MIXER_PROGPAN_ID),MixerGetFX());
	MisaProgressbar_SetCheck(hMixerItems[MIXER_PROGPAN], effect.size()?1:0);

	SynthSetDragCentre(SynthTranslateEffect(MIXER_PROGLOWCUT_ID),pInfo->Fx,pInfo->lowcut);
	SynthSetDragCentre(SynthTranslateEffect(MIXER_PROGHIGHCUT_ID),pInfo->Fx,pInfo->highcut);
	SynthSetDragCentre(SynthTranslateEffect(MIXER_PROGINPUTGAIN_ID),pInfo->Fx,pInfo->inputgain);
	SynthSetDragCentre(SynthTranslateEffect(MIXER_PROGLEVEL_ID),pInfo->Fx,pInfo->level);
	SynthSetDragCentre(SynthTranslateEffect(MIXER_PROGREVERBSEND_ID),pInfo->Fx,pInfo->reverbsend);
	SynthSetDragCentre(SynthTranslateEffect(MIXER_PROGPAN_ID),pInfo->Fx,pInfo->pan);
	return 0;
}

U8 GetMixerInfo(PMIXER_INFO pInfo)
{
	pInfo->Fx = MixerGetFX();
	pInfo->lowcut= MisaProgressbar_GetPercent(hMixerItems[MIXER_PROGLOWCUT]);
	pInfo->highcut = 127-MisaProgressbar_GetPercent(hMixerItems[MIXER_PROGHIGHCUT]);
	pInfo->inputgain = MisaProgressbar_GetPercent(hMixerItems[MIXER_PROGINPUTGAIN]);
	pInfo->level = MisaProgressbar_GetPercent(hMixerItems[MIXER_PROGLEVEL]);
	pInfo->reverbsend = MisaProgressbar_GetPercent(hMixerItems[MIXER_PROGREVERBSEND]);
	pInfo->pan = MisaProgressbar_GetPercent(hMixerItems[MIXER_PROGPAN]);
	return 0;
}

static U8 MixerCreateItems(WM_HWIN hParent)
{
	int x,y;
	hCOMPcomm = CreateCommComponentEx(hParent,COMMON_ITEM_FX1|COMMON_ITEM_FX2|COMMON_ITEM_CLOSE);
	memset(hMixerItems,0,sizeof(hMixerItems));
	x=2;
	y=500;
	x+=bmFX1_NORMAL.XSize;
	x+=bmFX2_NORMAL.XSize;
	x+=2;
	hMixerItems[MIXER_TOEQSCREEN] = BUTTON_CreateAsChild(x,y,bmTOEQ_NORMAL.XSize,bmTOEQ_NORMAL.YSize,hParent,MIXER_TOEQSCREEN_ID,WM_CF_SHOW|WM_CF_MEMDEV);
	BUTTON_SetFocussable(hMixerItems[MIXER_TOEQSCREEN],0);
	BUTTON_SetBitmap(hMixerItems[MIXER_TOEQSCREEN],BUTTON_CI_UNPRESSED,&bmTOEQ_NORMAL);
	BUTTON_SetBitmap(hMixerItems[MIXER_TOEQSCREEN],BUTTON_CI_PRESSED,&bmTOEQ_PRESSED);
	x = 4;
	y = bmEMPTYTITLEBAR.YSize;

	y += bmDISTYPEAREA.YSize;
	y += MIXER_YOFFSET;
	x = 50;
	hMixerItems[MIXER_INDICATOR] = CreateSlidePanel(x,y,WM_GetWindowSizeX(hParent)-100,bmSLIDER.YSize,hParent,WM_CF_SHOW|WM_CF_MEMDEV);
	return 0;
}

static U8 MixerDeleteItems()
{
	if(hMixerItems[MIXER_TOEQSCREEN])
	{
		BUTTON_Delete(hMixerItems[MIXER_TOEQSCREEN]);
	}
	if(hMixerItems[MIXER_INDICATOR])
	{
		DeleteSlidePanel(hMixerItems[MIXER_INDICATOR]);
	}
	DeleteCommComponent(hCOMPcomm);
	return 0;
}

static int MixerGetFX()
{
	int stFx1,stFx2;
	stFx1 = MisaRadio_GetStatus(hCOMPcomm.hFX1);
	stFx2 = MisaRadio_GetStatus(hCOMPcomm.hFX2);
	if(stFx1 == stFx2)
	{
		MisaRadio_SetStatus(hCOMPcomm.hFX1,1);
		MisaRadio_SetStatus(hCOMPcomm.hFX2,0);
		return 0;
	}
	else
	{
		return stFx2;
	}
}

static void MixerProc(WM_MESSAGE* pMsg)
{
	int x, y;
	int NCode,Id;
	// No more sliding
	GUI_PID_STATE* pPID_State;
	static U8 sidebutton = 0;

	ProcessCommComponent(pMsg, &hCOMPcomm);
	switch (pMsg->MsgId)
	{
	case WM_CREATE:
		MixerCreateItems(pMsg->hWin);
		break;
	case WM_DELETE:
		MixerDeleteItems();
		break;
	case WM_PAINT:
		GUI_SetBkColor(GUI_BLACK);
		GUI_Clear();
		GUI_SetTextMode(GUI_TEXTMODE_TRANS);
		GUI_DrawBitmap(&bmEMPTYTITLEBAR, 0, 0);
		GUI_SetFont(&GUI_Font32B_ASCII);
		x = WM_GetWindowSizeX(pMsg->hWin);
		{
			std::string title("Mixer");
			GUI_DispStringHCenterAt(GetTitleWithPreset(title).c_str(), x / 2, 5);
		}

		// No more sliding
		// Draw button shape
		if(WM_HasCaptured(pMsg->hWin))
		{
			const GUI_RECT* prect;
			switch(sidebutton)
			{
			case 1:
				prect = &leftsidebutton;
				break;
			case 2:
				prect = &rightsidebutton;
				break;
			case 0:
			default:
				prect = 0;
			}
			if(prect)
			{
				GUI_SetColor(GUI_LIGHTBLUE);
				GUI_SetPenSize(5);
				GUI_DrawRectEx(prect);
			}
		}

		// Draw slide indicator
		y = WM_GetWindowOrgY(hMixerItems[MIXER_SLIDEWINDOW]);
		y += 170;
		if(0x02 &SlidingBorder)
		{
			x = 4;
			GUI_DrawBitmap(&bmLARROW,x,y);
		}
		if(0x01 & SlidingBorder)
		{
			x = 750 + 15;
			GUI_DrawBitmap(&bmRARROW,x,y);
		}
		break;
	case WM_NOTIFY_PARENT:
		if(DispatchCommComponent(pMsg,&hCOMPcomm))
		{
			Id = WM_GetId(pMsg->hWinSrc);
			NCode = pMsg->Data.v;
			switch(NCode)
			{
			case WM_NOTIFICATION_CLICKED:
				DEBUG_STDOUT("MIXER button clicked!");
				break;
			case WM_NOTIFICATION_RELEASED:
				switch(Id)
				{
				case MIXER_TOEQSCREEN_ID:
					TopEqScreen(pMsg->hWin);
				case COMMON_ID_FX1:
				case COMMON_ID_FX2:
				case COMMON_ID_CLOSE:
					UpdateSynthSettingEx(MISAMODULE_MIXER);
					break;
				case CTL_NONE_ID:
				case CTL_TOUCH_X_ID:
				case CTL_TOUCH_Y_ID:
				case CTL_DRAG_X_ID:
				case CTL_DRAG_Y_ID:
				case CTL_BALL_X_ID:
				case CTL_BALL_Y_ID:
				case CTL_VARIATION_ID:
					MixerControlMenuProc(Id);
					break;
				default:
					;
				}
				DEBUG_STDOUT("MIXER button released!");
				break;
			}
		}
		break;
	case WM_TOUCH:
		// No more sliding
		if(pMsg->Data.p)
		{
			pPID_State = (GUI_PID_STATE*)pMsg->Data.p;
			if(pPID_State->Pressed)
			{
				if(!WM_HasCaptured(pMsg->hWin))
				{
					GUI_RECT ptRect;
					WM_SetCapture(pMsg->hWin,1);
					ptRect.x0 = pPID_State->x;
					ptRect.y0 = pPID_State->y;
					ptRect.x1 = ptRect.x0+1;
					ptRect.y1 = ptRect.y0+1;
					if(GUI_RectsIntersect(&leftsidebutton,&ptRect))
					{
						sidebutton = 1;
						WM_InvalidateRect(pMsg->hWin, &leftsidebutton);
						SlidingBorder = SlideGoPreviousPage(hMixerItems[MIXER_SLIDEWINDOW]);
					}
					else if(GUI_RectsIntersect(&rightsidebutton,&ptRect))
					{
						sidebutton = 2;
						WM_InvalidateRect(pMsg->hWin, &rightsidebutton);
						SlidingBorder = SlideGoNextPage(hMixerItems[MIXER_SLIDEWINDOW]);
					}
				}
			}
			else
			{
				//WM_ReleaseCapture();
				sidebutton = 0;
				if(pPID_State->x<50)
				{
					WM_InvalidateRect(pMsg->hWin, &leftsidebutton);
				}
				else
				{
					WM_InvalidateRect(pMsg->hWin, &rightsidebutton);
				}
			}
		}
		//! No more sliding
		break;
	default:
		WM_DefaultProc(pMsg);
	}
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa Sliding window support
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

static WM_HWIN hSlideWindow;
static void SlideWindowProc(WM_MESSAGE* pMsg);
static void IndicatorProc(WM_MESSAGE* pMsg);

WM_HWIN CreateSlidePanel(int x0, int y0, int width, int height, WM_HWIN hParent, U16 Style)
{
	return WM_CreateWindowAsChild(x0,y0,width,height,hParent,Style,IndicatorProc,0);
}

static U8 DeleteSlidePanel(WM_HWIN hSlidePanel)
{
	WM_DeleteWindow(hSlidePanel);
	return 0;
}

static U8 IndicatorCreateItems(WM_HWIN hParent)
{
	hMixerItems[MIXER_SLIDEWINDOW] = WM_CreateWindowAsChild(0,0,1100,WM_GetWindowSizeY(hParent),hParent,WM_CF_SHOW|WM_CF_MEMDEV,SlideWindowProc,0);
	return 0;
}

static U8 IndicatorDeleteItems()
{
	if(hMixerItems[MIXER_SLIDEWINDOW])
	{
		WM_DeleteWindow(hMixerItems[MIXER_SLIDEWINDOW]);
	}
	return 0;
}

static void IndicatorProc(WM_MESSAGE* pMsg)
{
	switch (pMsg->MsgId)
	{
	case WM_CREATE:
		IndicatorCreateItems(pMsg->hWin);
		break;
	case WM_DELETE:
		IndicatorDeleteItems();
		break;
	case WM_PAINT:
		GUI_SetBkColor(GUI_BLACK);
		GUI_Clear();
		break;
	default:
		WM_DefaultProc(pMsg);
	}
}

static void MixerLowcutInfo(int value,char* buf)
{
	if(0==value)
	{
		strcpy(buf,"OFF");
	}
	else
	{
		if(127 == value)
		{
			strcpy(buf,"300Hz");
		}
		else
		{
			int val = (int) (value * 2.36);
			sprintf(buf,"%dHz",val);
		}
	}
}

static void MixerHeighcutInfo(int value, char* buf)
{
	value = 127-value;
	if(127==value)
	{
		strcpy(buf,"OFF");
	}
	else
	{
		float val = value*4.5 + 150;
		sprintf(buf,"%.3fKHz",val/100);
	}
}

static void MixerReverbsendInfo(int value, char* buf)
{
	if(0==value)
	{
		strcpy(buf,"-oo");
	}
	else
	{
		if(127 == value)
		{
			strcpy(buf,"0dB");
		}
		else
		{
			float val = value;
			sprintf(buf,"%.3fdB",(val-1)/3-42);
		}
	}
}

static U8 SlideCreateItems(WM_HWIN hParent)
{
	int x,y;
	x = 0;
	y = 0;
	hMixerItems[MIXER_PROGLOWCUT] = MisaProgressbar_CreateEx(x,y,0,0,hParent,MIXER_PROGLOWCUT_ID,"Low","cut",MISAPROG_FONT24,MixerLowcutInfo);
	x += bmSLIDER.XSize;
	x += 100;
	hMixerItems[MIXER_PROGHIGHCUT] = MisaProgressbar_CreateEx(x,y,0,0,hParent,MIXER_PROGHIGHCUT_ID,"High","cut",MISAPROG_FONT24,MixerHeighcutInfo);
	x += bmSLIDER.XSize;
	x += 100;
	hMixerItems[MIXER_PROGINPUTGAIN] = MisaProgressbar_CreateEx(x,y,0,0,hParent,MIXER_PROGINPUTGAIN_ID,"Input","gain",MISAPROG_FONT24,0);
	x += bmSLIDER.XSize;
	x += 100;
	hMixerItems[MIXER_PROGLEVEL] = MisaProgressbar_CreateEx(x,y,0,0,hParent,MIXER_PROGLEVEL_ID,"Level",0,MISAPROG_FONT24,0);
	x += bmSLIDER.XSize;
	x += 100;
	hMixerItems[MIXER_PROGREVERBSEND] = MisaProgressbar_CreateEx(x,y,0,0,hParent,MIXER_PROGREVERBSEND_ID,"Reverb","send",MISAPROG_FONT24,MixerReverbsendInfo);
	x += bmSLIDER.XSize;
	x += 100;
	hMixerItems[MIXER_PROGPAN] = MisaProgressbar_CreateEx(x,y,0,0,hParent,MIXER_PROGPAN_ID,"Pan",0,MISAPROG_FONT24,0);
	x += bmSLIDER.XSize;
	x += 100;
	return 0;
}

static U8 SlideDeleteItems()
{
	if(hMixerItems[MIXER_PROGLOWCUT])
	{
		MisaProgressbar_Delete(hMixerItems[MIXER_PROGLOWCUT]);
	}
	if(hMixerItems[MIXER_PROGHIGHCUT])
	{
		MisaProgressbar_Delete(hMixerItems[MIXER_PROGHIGHCUT]);
	}
	if(hMixerItems[MIXER_PROGINPUTGAIN])
	{
		MisaProgressbar_Delete(hMixerItems[MIXER_PROGINPUTGAIN]);
	}
	if(hMixerItems[MIXER_PROGLEVEL])
	{
		MisaProgressbar_Delete(hMixerItems[MIXER_PROGLEVEL]);
	}
	if(hMixerItems[MIXER_PROGREVERBSEND])
	{
		MisaProgressbar_Delete(hMixerItems[MIXER_PROGREVERBSEND]);
	}
	return 0;
}

static void SlideWindowProc(WM_MESSAGE* pMsg)
{
	int x,y;
	int NCode,Id;
	std::vector<int> effect;
	//GUI_PID_STATE* pPID_State;
	//static GUI_PID_STATE PID_LastState;
	switch (pMsg->MsgId)
	{
	case WM_CREATE:
		SlideCreateItems(pMsg->hWin);
		break;
	case WM_DELETE:
		SlideDeleteItems();
		break;
	case WM_PAINT:
		GUI_SetBkColor(GUI_BLACK);
		GUI_Clear();
		break;
	case WM_NOTIFY_PARENT:
		Id = WM_GetId(pMsg->hWinSrc);
		NCode = pMsg->Data.v;
		switch(NCode)
		{
		case WM_NOTIFICATION_CLICKED:
			break;
		case WM_NOTIFICATION_RELEASED:
			break;
		case WM_NOTIFICATION_VALUE_CHANGED:
			x = MisaProgressbar_GetPercent(pMsg->hWinSrc);
			switch(Id)
			{
			case MIXER_PROGLOWCUT_ID:
				MixerSetLowcut(MixerGetFX(),x<127?x:127);
				break;
			case MIXER_PROGHIGHCUT_ID:
				x=127-x;
				MixerSetHighcut(MixerGetFX(),x<127?x:127);
				break;
			case MIXER_PROGINPUTGAIN_ID:
				MixerSetInputgain(MixerGetFX(),x<127?x:127);
				break;
			case MIXER_PROGLEVEL_ID:
			default:
				MixerSetLevel(MixerGetFX(),x<127?x:127);
				break;
			case MIXER_PROGREVERBSEND_ID:
				MixerSetReverbsend(MixerGetFX(),x<127?x:127);
				break;
			case MIXER_PROGPAN_ID:
				MixerSetPan(MixerGetFX(),x<127?x:127);
			}
			break;
		case WM_NOTIFICATION_SEL_CHANGED:
			x = WM_GetWindowOrgX(pMsg->hWinSrc)+50;
			y = WM_GetWindowOrgY(pMsg->hWinSrc)+340;
			switch(Id)
			{
			case MIXER_PROGLOWCUT_ID:
				MisaSetcontrolmenu_PopupEx(hMixer,pMsg->hWinSrc,x,y,0,0,MixerLowcutInfo,MixerLowcutInfo);
				break;
			case MIXER_PROGHIGHCUT_ID:
				MisaSetcontrolmenu_PopupEx(hMixer,pMsg->hWinSrc,x,y,0,0,MixerHeighcutInfo,MixerHeighcutInfo);
				break;
			case MIXER_PROGINPUTGAIN_ID:
				MisaSetcontrolmenu_Popup(hMixer,pMsg->hWinSrc,x,y);
				break;
			case MIXER_PROGLEVEL_ID:
				MisaSetcontrolmenu_Popup(hMixer,pMsg->hWinSrc,x,y);
				break;
			case MIXER_PROGREVERBSEND_ID:
				MisaSetcontrolmenu_PopupEx(hMixer,pMsg->hWinSrc,x,y,0,0,MixerReverbsendInfo,MixerReverbsendInfo);
				break;
			default:
				MisaSetcontrolmenu_Popup(hMixer,pMsg->hWinSrc,x,y);
			}
			effect = SynthFindEffect(SynthTranslateEffect(Id),MixerGetFX());
			y = effect.size();
			for(x=0;x<y;x++)
			{
				if(-1 != effect[x])
				{
					MisaMenu_SetStatus(AssigneMentMenuTranslate(effect[x]&0x000000FF),2,0,(effect[x]>>8)&0x000000FF);
					MisaMenu_SetStatus(CTL_VARMIN,0,(effect[x]>>16)&0x000000FF,(effect[x]>>8)&0x000000FF);
					MisaMenu_SetStatus(CTL_VARMAX,0,(effect[x]>>24)&0x000000FF,(effect[x]>>8)&0x000000FF);
				}
			}
			break;
		}
		break;
	case WM_TOUCH:
#if 0		// No more sliding
		if(pMsg->Data.p)
		{
			pPID_State = (GUI_PID_STATE*)pMsg->Data.p;
			if(pPID_State->Pressed)
			{
				int dx;
				if(0 == PID_LastState.Pressed)
				{
					WM_SetCapture(pMsg->hWin,0);
					PID_LastState = *pPID_State;
				}
				else
				{
					#if 1 // Remove bounce back effect
					x = WM_GetWindowOrgX(pMsg->hWin);
					y = WM_GetWindowSizeX(pMsg->hWin);
					dx = pPID_State->x-PID_LastState.x;
					if(dx>0)
					{
						WM_MoveWindow(pMsg->hWin,dx>INDICATORFRAME-x?INDICATORFRAME-x:dx,0);
					}
					else if(dx<0)
					{
						WM_MoveWindow(pMsg->hWin,dx<GUI_GetScreenSizeX()-INDICATORFRAME-y-x?GUI_GetScreenSizeX()-INDICATORFRAME-y-x:dx,0);
					}
					#else
					WM_MoveWindow(pMsg->hWin,pPID_State->x-PID_LastState.x,0);
					#endif
				}
			}
			else
			{
				PID_LastState = *pPID_State;
				WM_ReleaseCapture();
				x = WM_GetWindowOrgX(pMsg->hWin);
				y = WM_GetWindowSizeX(pMsg->hWin);
				if(INDICATORFRAME < x)
				{
					WM_MoveWindow(pMsg->hWin,INDICATORFRAME-x,0);
				}
				else if(GUI_GetScreenSizeX()-INDICATORFRAME-y > x)
				{
					WM_MoveWindow(pMsg->hWin,GUI_GetScreenSizeX()-INDICATORFRAME-y-x,0);
				}
				SlidingBorder = GetSlidingBordercheck(pMsg->hWin,hMixer);
			}
		}
		else
		{
			PID_LastState.Pressed = 0;
			PID_LastState.x = 0;
			PID_LastState.y = 0;
			WM_ReleaseCapture();
		}
#endif		// No more sliding
		break;
	default:
		WM_DefaultProc(pMsg);
	}
}

static void MixerControlMenuProc(int menuId)
{
	AssignEffectControlMenuProc(menuId,MixerGetFX());
}

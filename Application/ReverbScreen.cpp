
#include <cstdlib>
#include <iostream>

#include "GUI.h"
#include "WM.h"
#include "BUTTON.h"

#include "MisaWidget.h"

#include "MisaAppInc.h"

#include "MainMenu.h"
#include "ReverbScreen.h"

#include "MisamenuConfig.h"

#include "Resource.h"

///////////////////////////////////////////////////////////////////////////////
//

#define REVERB_TYPE_XPOS 80
#define REVERB_TYPE_YPOS 120

#define REVERB_XOFFSET 2
#define REVERB_YOFFSET 2

///////////////////////////////////////////////////////////////////////////////
//

typedef enum __REVERBITEMS
{
	REVERB_INDICATOR,
	REVERB_SLIDEWINDOW,
	REVERB_LEFTARROW,
	REVERB_RIGHTARROW,
	REVERB_PROGLEVEL,
	REVERB_PROGPRELPF,
	REVERB_PROGCHAR,
	REVERB_PROGTIME,
	REVERB_PROGDELAYFDBK,
	REVERB_PROGDELAYTIME,
	REVERB_POPMENU,
	REVERB_MAX
} REVERBITEMS;

///////////////////////////////////////////////////////////////////////////////
//

static WM_HWIN hReverb;
static void ReverbProc(WM_MESSAGE* pMsg);
static WM_HWIN hReverbItems[REVERB_MAX];
static COMMCOMPONENT hCOMPcomm;

static WM_HWIN CreateSlidePanel(int x0, int y0, int width, int height, WM_HWIN hParent, U16 Style);
static U8 DeleteSlidePanel(WM_HWIN hSlidePanel);
static U8 SlidingBorder;

static int ReverbGetFX();
static void ReverbControlMenuProc(int menuId);

static const GUI_RECT rectRevType=
{
	300,
	62,
	500,
	130
};
static int revIndex;
static const char* strRevtype[]=
{
	"std room 1",
	"std room 2",
	"std room 3",
	"hall 1",
	"hall 2",
	"plate",
	"delay",
	"pan delay"
};

///////////////////////////////////////////////////////////////////////////////
//

U8 CreateReverbScreen(void* pPara)
{
	int x,y;
	x=GUI_GetScreenSizeX();
	y=GUI_GetScreenSizeY();
	SlidingBorder = 1;
	revIndex = 0;
	hReverb = WM_CreateWindow(0, 0, x, y, /*WM_CF_SHOW | */WM_CF_MEMDEV, ReverbProc, 0);
	return 0;
}

U8 DeleteReverbScreen()
{
	WM_DeleteWindow(hReverb);
	return 0;
}

U8 TopReverbScreen(WM_HWIN hPreWin)
{
	WM_HideWindow(hPreWin);
	WM_ShowWindow(hReverb);
	//WM_BringToTop(hReverb);
	SetWindowToUpdateIfPresetModified(hReverb);
	return 0;
}

U8 UpdateReverbInfo(PREVERB_INFO pInfo)
{
	std::vector<int> effect;
	//if(pInfo->Fx != ReverbGetFX())
	//{
	//	MisaRadio_SetStatus(hCOMPcomm.hFX1,pInfo->Fx?0:1);
	//	MisaRadio_SetStatus(hCOMPcomm.hFX2,pInfo->Fx?1:0);
	//}
	revIndex = pInfo->type;
	WM_InvalidateRect(hReverb, &rectRevType);
	MisaProgressbar_SetPercentEx(hReverbItems[REVERB_PROGPRELPF], pInfo->pre_lpf, 0);
	MisaProgressbar_SetPercentEx(hReverbItems[REVERB_PROGCHAR], pInfo->character, 0);
	MisaProgressbar_SetPercentEx(hReverbItems[REVERB_PROGLEVEL], pInfo->level, 0);
	MisaProgressbar_SetPercentEx(hReverbItems[REVERB_PROGTIME], pInfo->time, 0);
	MisaProgressbar_SetPercentEx(hReverbItems[REVERB_PROGDELAYFDBK], pInfo->delayfdbk, 0);
	MisaProgressbar_SetPercentEx(hReverbItems[REVERB_PROGDELAYTIME], pInfo->delaytime, 0);
	effect = SynthFindEffect(SynthTranslateEffect(REVERB_ID_PROGPRELPF),0);
	MisaProgressbar_SetCheck(hReverbItems[REVERB_PROGPRELPF], effect.size()?1:0);
	effect = SynthFindEffect(SynthTranslateEffect(REVERB_ID_PROGCHAR),0);
	MisaProgressbar_SetCheck(hReverbItems[REVERB_PROGCHAR], effect.size()?1:0);
	effect = SynthFindEffect(SynthTranslateEffect(REVERB_ID_PROGLEVEL),0);
	MisaProgressbar_SetCheck(hReverbItems[REVERB_PROGLEVEL], effect.size()?1:0);
	effect = SynthFindEffect(SynthTranslateEffect(REVERB_ID_PROGTIME),0);
	MisaProgressbar_SetCheck(hReverbItems[REVERB_PROGTIME], effect.size()?1:0);
	effect = SynthFindEffect(SynthTranslateEffect(REVERB_ID_PROGDELAYFDBK),0);
	MisaProgressbar_SetCheck(hReverbItems[REVERB_PROGDELAYFDBK], effect.size()?1:0);
	effect = SynthFindEffect(SynthTranslateEffect(REVERB_ID_PROGDELAYTIME),0);
	MisaProgressbar_SetCheck(hReverbItems[REVERB_PROGDELAYTIME], effect.size()?1:0);

	SynthSetDragCentre(SynthTranslateEffect(REVERB_ID_PROGPRELPF),0,pInfo->pre_lpf);
	SynthSetDragCentre(SynthTranslateEffect(REVERB_ID_PROGCHAR),0,pInfo->character);
	SynthSetDragCentre(SynthTranslateEffect(REVERB_ID_PROGLEVEL),0,pInfo->level);
	SynthSetDragCentre(SynthTranslateEffect(REVERB_ID_PROGTIME),0,pInfo->time);
	SynthSetDragCentre(SynthTranslateEffect(REVERB_ID_PROGDELAYFDBK),0,pInfo->delayfdbk);
	SynthSetDragCentre(SynthTranslateEffect(REVERB_ID_PROGDELAYTIME),0,pInfo->delaytime);
	return 0;
}

U8 GetReverbInfo(PREVERB_INFO pInfo)
{
	//pInfo->Fx = ReverbGetFX();
	pInfo->type = revIndex;
	pInfo->pre_lpf = MisaProgressbar_GetPercent(hReverbItems[REVERB_PROGPRELPF]);
	pInfo->character = MisaProgressbar_GetPercent(hReverbItems[REVERB_PROGCHAR]);
	pInfo->level = MisaProgressbar_GetPercent(hReverbItems[REVERB_PROGLEVEL]);
	pInfo->time = MisaProgressbar_GetPercent(hReverbItems[REVERB_PROGTIME]);
	pInfo->delayfdbk = MisaProgressbar_GetPercent(hReverbItems[REVERB_PROGDELAYFDBK]);
	pInfo->delaytime = MisaProgressbar_GetPercent(hReverbItems[REVERB_PROGDELAYTIME]);
	return 0;
}

static U8 ReverbCreateItems(WM_HWIN hParent)
{
	int x,y;
	hCOMPcomm = CreateCommComponentEx(hParent,COMMON_ITEM_CLOSE);
	memset(hReverbItems,0,sizeof(hReverbItems));
	x = 4;
	y = bmEMPTYTITLEBAR.YSize;
	hReverbItems[REVERB_LEFTARROW] = BUTTON_CreateAsChild(x,y,bmLEFTARROW_UN.XSize,bmLEFTARROW_UN.YSize,hParent,REVERB_ID_LEFTARROW,WM_CF_SHOW|WM_CF_MEMDEV);
	BUTTON_SetFocussable(hReverbItems[REVERB_LEFTARROW],0);
	BUTTON_SetBitmap(hReverbItems[REVERB_LEFTARROW],BUTTON_CI_UNPRESSED,&bmLEFTARROW_UN);
	BUTTON_SetBitmap(hReverbItems[REVERB_LEFTARROW],BUTTON_CI_PRESSED,&bmLEFTARROW_SE);
	x += bmLEFTARROW_UN.XSize+356;	
	hReverbItems[REVERB_RIGHTARROW] = BUTTON_CreateAsChild(x,y,bmRIGHTARROW_UN.XSize,bmRIGHTARROW_UN.YSize,hParent,REVERB_ID_RIGHTARROW,WM_CF_SHOW|WM_CF_MEMDEV);
	BUTTON_SetFocussable(hReverbItems[REVERB_RIGHTARROW],0);
	BUTTON_SetBitmap(hReverbItems[REVERB_RIGHTARROW],BUTTON_CI_UNPRESSED,&bmRIGHTARROW_UN);
	BUTTON_SetBitmap(hReverbItems[REVERB_RIGHTARROW],BUTTON_CI_PRESSED,&bmRIGHTARROW_SE);
	y += bmDISTYPEAREA.YSize;
	y += REVERB_YOFFSET;
	x = 50;
	hReverbItems[REVERB_INDICATOR] = CreateSlidePanel(x,y,WM_GetWindowSizeX(hParent)-100,bmSLIDER.YSize,hParent,WM_CF_SHOW|WM_CF_MEMDEV);
	return 0;
}

static U8 ReverbDeleteItems()
{
	if(hReverbItems[REVERB_LEFTARROW])
	{
		BUTTON_Delete(hReverbItems[REVERB_LEFTARROW]);
	}
	if(hReverbItems[REVERB_RIGHTARROW])
	{
		BUTTON_Delete(hReverbItems[REVERB_RIGHTARROW]);
	}
	if(hReverbItems[REVERB_INDICATOR])
	{
		DeleteSlidePanel(hReverbItems[REVERB_INDICATOR]);
	}
	DeleteCommComponent(hCOMPcomm);
	return 0;
}

static int ReverbGetFX()
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

static void ReverbProc(WM_MESSAGE* pMsg)
{
	int x, y;
	int NCode,Id;
	ProcessCommComponent(pMsg, &hCOMPcomm);
	switch (pMsg->MsgId)
	{
	case WM_CREATE:
		ReverbCreateItems(pMsg->hWin);
		break;
	case WM_DELETE:
		ReverbDeleteItems();
		break;
	case WM_PAINT:
		GUI_SetBkColor(GUI_BLACK);
		GUI_Clear();
		GUI_SetTextMode(GUI_TEXTMODE_TRANS);
		GUI_DrawBitmap(&bmEMPTYTITLEBAR, 0, 0);
		x = 222;
		y = bmEMPTYTITLEBAR.YSize;
		GUI_DrawBitmap(&bmDISTYPEAREA, x, y);
		GUI_SetFont(&GUI_Font32B_ASCII);
		x = WM_GetWindowSizeX(pMsg->hWin);
		{
			std::string title("Reverb");
			GUI_DispStringHCenterAt(GetTitleWithPreset(title).c_str(), x / 2, 5);
		}
		y += 20;
		GUI_DispStringHCenterAt(strRevtype[revIndex], x / 2, y);
		y = WM_GetWindowOrgY(hReverbItems[REVERB_SLIDEWINDOW]);
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
				DEBUG_STDOUT("REVERB button clicked!");
				break;
			case WM_NOTIFICATION_RELEASED:
				switch(Id)
				{
				case REVERB_ID_LEFTARROW:
					--revIndex;
					if(0>revIndex)
					{
						revIndex = sizeof(strRevtype)/4-1;
					}
					WM_InvalidateRect(pMsg->hWin, &rectRevType);
					ReverbSetType(revIndex);
					break;
				case REVERB_ID_RIGHTARROW:
					++revIndex;
					if(sizeof(strRevtype)/4 <= revIndex)
					{
						revIndex = 0;
					}
					WM_InvalidateRect(pMsg->hWin, &rectRevType);
					ReverbSetType(revIndex);
					break;
				// Removed
				//case COMMON_ID_FX1:
				//case COMMON_ID_FX2:
				case COMMON_ID_CLOSE:
					UpdateSynthSettingEx(MISAMODULE_REVERB);
					break;
				case CTL_NONE_ID:
				case CTL_TOUCH_X_ID:
				case CTL_TOUCH_Y_ID:
				case CTL_DRAG_X_ID:
				case CTL_DRAG_Y_ID:
				case CTL_BALL_X_ID:
				case CTL_BALL_Y_ID:
				case CTL_VARIATION_ID:
					ReverbControlMenuProc(Id);
					break;
				default:
					;
				}
				DEBUG_STDOUT("REVERB button released!");
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
	hReverbItems[REVERB_SLIDEWINDOW] = WM_CreateWindowAsChild(0,0,1100,WM_GetWindowSizeY(hParent),hParent,WM_CF_SHOW|WM_CF_MEMDEV,SlideWindowProc,0);
	return 0;
}

static U8 IndicatorDeleteItems()
{
	if(hReverbItems[REVERB_SLIDEWINDOW])
	{
		WM_DeleteWindow(hReverbItems[REVERB_SLIDEWINDOW]);
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

static U8 SlideCreateItems(WM_HWIN hParent)
{
	int x,y;
	x = 0;
	y = 0;
	hReverbItems[REVERB_PROGLEVEL] = MisaProgressbar_Create(x,y,0,0,hParent,REVERB_ID_PROGLEVEL,"LEVEL",MISAPROG_FONT24);
	x += bmSLIDER.XSize;
	x += 100;
	hReverbItems[REVERB_PROGPRELPF] = MisaProgressbar_CreateEx(x,y,0,0,hParent,REVERB_ID_PROGPRELPF,"PRE","LPF",MISAPROG_FONT24,0);
	x += bmSLIDER.XSize;
	x += 100;
	hReverbItems[REVERB_PROGCHAR] = MisaProgressbar_Create(x,y,0,0,hParent,REVERB_ID_PROGCHAR,"CHAR",MISAPROG_FONT24);
	x += bmSLIDER.XSize;
	x += 100;
	hReverbItems[REVERB_PROGTIME] = MisaProgressbar_Create(x,y,0,0,hParent,REVERB_ID_PROGTIME,"TIME",MISAPROG_FONT24);
	x += bmSLIDER.XSize;
	x += 100;
	hReverbItems[REVERB_PROGDELAYFDBK] = MisaProgressbar_CreateEx(x,y,0,0,hParent,REVERB_ID_PROGDELAYFDBK,"DELAY","FDBK",MISAPROG_FONT24,0);
	x += bmSLIDER.XSize;
	x += 100;
	hReverbItems[REVERB_PROGDELAYTIME] = MisaProgressbar_CreateEx(x,y,0,0,hParent,REVERB_ID_PROGDELAYTIME,"DELAY","TIME",MISAPROG_FONT24,0);
	return 0;
}

static U8 SlideDeleteItems()
{
	if(hReverbItems[REVERB_PROGPRELPF])
	{
		MisaProgressbar_Delete(hReverbItems[REVERB_PROGPRELPF]);
	}
	if(hReverbItems[REVERB_PROGCHAR])
	{
		MisaProgressbar_Delete(hReverbItems[REVERB_PROGCHAR]);
	}
	if(hReverbItems[REVERB_PROGLEVEL])
	{
		MisaProgressbar_Delete(hReverbItems[REVERB_PROGLEVEL]);
	}
	if(hReverbItems[REVERB_PROGTIME])
	{
		MisaProgressbar_Delete(hReverbItems[REVERB_PROGTIME]);
	}
	if(hReverbItems[REVERB_PROGDELAYFDBK])
	{
		MisaProgressbar_Delete(hReverbItems[REVERB_PROGDELAYFDBK]);
	}
	if(hReverbItems[REVERB_PROGDELAYTIME])
	{
		MisaProgressbar_Delete(hReverbItems[REVERB_PROGDELAYTIME]);
	}
	return 0;
}

static void SlideWindowProc(WM_MESSAGE* pMsg)
{
	int x,y;
	int NCode,Id;
	std::vector<int> effect;
	GUI_PID_STATE* pPID_State;
	static GUI_PID_STATE PID_LastState;
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
			case REVERB_ID_PROGPRELPF:
				ReverbSetPreLpf(x<127?x:127);
				break;
			case REVERB_ID_PROGCHAR:
				ReverbSetCharacter(x<127?x:127);
				break;
			case REVERB_ID_PROGLEVEL:
				ReverbSetLevel(x<127?x:127);
				break;
			case REVERB_ID_PROGTIME:
				ReverbSetTime(x<127?x:127);
				break;
			case REVERB_ID_PROGDELAYFDBK:
				ReverbSetDelayFeedback(x<127?x:127);
				break;
			case REVERB_ID_PROGDELAYTIME:
				ReverbSetPreDelay(x<127?x:127);
				break;
			default:
				;
			}
			break;
		case WM_NOTIFICATION_SEL_CHANGED:
			x = WM_GetWindowOrgX(pMsg->hWinSrc)+50;
			y = WM_GetWindowOrgY(pMsg->hWinSrc)+340;
			MisaSetcontrolmenu_Popup(hReverb,pMsg->hWinSrc,x,y);
			effect = SynthFindEffect(SynthTranslateEffect(Id),0);
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
		}
		break;
	case WM_TOUCH:
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
				SlidingBorder = GetSlidingBordercheck(pMsg->hWin,hReverb);
			}
		}
		else
		{
			PID_LastState.Pressed = 0;
			PID_LastState.x = 0;
			PID_LastState.y = 0;
			WM_ReleaseCapture();
		}
		break;
	default:
		WM_DefaultProc(pMsg);
	}
}

static void ReverbControlMenuProc(int menuId)
{
	AssginEffectControlMenuProc(menuId,ReverbGetFX());
}


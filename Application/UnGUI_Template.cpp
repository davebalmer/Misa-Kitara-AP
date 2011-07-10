
#include <cstdlib>
#include <iostream>

#include "GUI.h"
#include "WM.h"
#include "BUTTON.h"

#include "MisaWidget.h"

#include "MisaAppInc.h"

#include "MainMenu.h"
#include "xxxMisaxxxScreen.h"

#include "MisamenuConfig.h"

#include "Resource.h"

///////////////////////////////////////////////////////////////////////////////
//

#define xxxMISAxxx_TYPE_XPOS 80
#define xxxMISAxxx_TYPE_YPOS 120

#define xxxMISAxxx_XOFFSET 2
#define xxxMISAxxx_YOFFSET 2

///////////////////////////////////////////////////////////////////////////////
//

typedef enum __xxxMISAxxxITEMS
{
	
	xxxMISAxxx_INDICATOR,
	xxxMISAxxx_SLIDEWINDOW,
	xxxMISAxxx_LEFTARROW,
	xxxMISAxxx_RIGHTARROW,
	xxxMISAxxx_PROGMODRATE,
	xxxMISAxxx_PROGMODDEPTH,
	xxxMISAxxx_PROGLEVEL,
	xxxMISAxxx_PROGFDBK,
	xxxMISAxxx_PROGFDBKFILTER,
	xxxMISAxxx_PROGSHAPE,
	xxxMISAxxx_PROGTIME,
	xxxMISAxxx_POPMENU,
	xxxMISAxxx_MAX
} xxxMISAxxxITEMS;

typedef enum __xxxMISAxxx_ID
{
	xxxMISAxxx_ID_LEFTARROW=GUI_ID_xxxMISAxxx_BASE,
	xxxMISAxxx_ID_RIGHTARROW,
	xxxMISAxxx_ID_PROGMODRATE,
	xxxMISAxxx_ID_PROGMODDEPTH,
	xxxMISAxxx_ID_PROGLEVEL,
	xxxMISAxxx_ID_PROGFDBK,
	xxxMISAxxx_ID_PROGFDBKFILTER,
	xxxMISAxxx_ID_PROGSHAPE,
	xxxMISAxxx_ID_PROGTIME,
	xxxMISAxxx_ID_POPMENU,
	xxxMISAxxx_ID_MAX
} xxxMISAxxx_ID;

///////////////////////////////////////////////////////////////////////////////
//

static WM_HWIN hxxxMisaxxx;
static void xxxMisaxxxProc(WM_MESSAGE* pMsg);
static WM_HWIN hxxxMisaxxxItems[xxxMISAxxx_MAX];
static COMMCOMPONENT hCOMPcomm;

static WM_HWIN CreateSlidePanel(int x0, int y0, int width, int height, WM_HWIN hParent, U16 Style);
static U8 DeleteSlidePanel(WM_HWIN hSlidePanel);
static U8 SlidingBorder;

static int xxxMisaxxxGetFX();
static U8 xxxMisaxxxLoadParameter(int index, U8 update);
static void xxxMisaxxxControlMenuProc(int menuId);

static const GUI_RECT rectModType=
{
	250,
	62,
	550,
	130
};
static int MisaIndex;

///////////////////////////////////////////////////////////////////////////////
//

U8 CreatexxxMisaxxxScreen(void* pPara)
{
	int x,y;
	x=GUI_GetScreenSizeX();
	y=GUI_GetScreenSizeY();
	SlidingBorder = 1;
	MisaIndex = 0;
	hxxxMisaxxx = WM_CreateWindow(0, 0, x, y, /*WM_CF_SHOW | */WM_CF_MEMDEV, xxxMisaxxxProc, 0);
	return 0;
}

U8 DeletexxxMisaxxxScreen()
{
	WM_DeleteWindow(hxxxMisaxxx);
	return 0;
}

U8 TopxxxMisaxxxScreen(WM_HWIN hPreWin)
{
	WM_HideWindow(hPreWin);
	WM_ShowWindow(hxxxMisaxxx);
	//WM_BringToTop(hxxxMisaxxx);
	return 0;
}

U8 UpdatexxxMisaxxxInfo(PxxxMISAxxx_INFO pInfo)
{
#if 0
	if(pInfo->Fx != xxxMisaxxxGetFX())
	{
		MisaRadio_SetStatus(hCOMPcomm.hFX1,pInfo->Fx?0:1);
		MisaRadio_SetStatus(hCOMPcomm.hFX2,pInfo->Fx?1:0);
	}
	MisaCheckbox_SetStatus(hCOMPcomm.hCheckOn,pInfo->On);
	if(pInfo->On)
	{
		MisaIndex = pInfo->type;
		WM_InvalidateRect(hxxxMisaxxx, &rectModType);
		MisaProgressbar_SetPercentEx(hxxxMisaxxxItems[xxxMISAxxx_PROGMODRATE],pInfo->mod_rate,0);
		MisaProgressbar_SetPercentEx(hxxxMisaxxxItems[xxxMISAxxx_PROGMODDEPTH],pInfo->mod_depth,0);
		MisaProgressbar_SetPercentEx(hxxxMisaxxxItems[xxxMISAxxx_PROGLEVEL],pInfo->level,0);
		MisaProgressbar_SetPercentEx(hxxxMisaxxxItems[xxxMISAxxx_PROGFDBK],pInfo->fdbk,0);
		MisaProgressbar_SetPercentEx(hxxxMisaxxxItems[xxxMISAxxx_PROGFDBKFILTER],pInfo->fdbkfilter,0);
		MisaProgressbar_SetPercentEx(hxxxMisaxxxItems[xxxMISAxxx_PROGSHAPE],pInfo->shape,0);
		MisaProgressbar_SetPercentEx(hxxxMisaxxxItems[xxxMISAxxx_PROGTIME],pInfo->time,0);
	}
	else
	{
		xxxMisaxxxLoadParameter(MisaIndex,0);
	}
#endif
	return 0;
}

U8 GetxxxMisaxxxInfo(PxxxMISAxxx_INFO pInfo)
{
#if 0
	pInfo->Fx = xxxMisaxxxGetFX();
	pInfo->On = MisaCheckbox_GetStatus(hCOMPcomm.hCheckOn);
	pInfo->type = MisaIndex;
	WM_InvalidateRect(hxxxMisaxxx, &rectModType);
	pInfo->mod_rate = MisaProgressbar_GetPercent(hxxxMisaxxxItems[xxxMISAxxx_PROGMODRATE]);
	pInfo->mod_depth = MisaProgressbar_GetPercent(hxxxMisaxxxItems[xxxMISAxxx_PROGMODDEPTH]);
	pInfo->level = MisaProgressbar_GetPercent(hxxxMisaxxxItems[xxxMISAxxx_PROGLEVEL]);
	pInfo->fdbk = MisaProgressbar_GetPercent(hxxxMisaxxxItems[xxxMISAxxx_PROGFDBK]);
	pInfo->fdbkfilter = MisaProgressbar_GetPercent(hxxxMisaxxxItems[xxxMISAxxx_PROGFDBKFILTER]);
	pInfo->shape = MisaProgressbar_GetPercent(hxxxMisaxxxItems[xxxMISAxxx_PROGSHAPE]);
	pInfo->time = MisaProgressbar_GetPercent(hxxxMisaxxxItems[xxxMISAxxx_PROGTIME]);
#endif
	return 0;
}

static U8 xxxMisaxxxCreateItems(WM_HWIN hParent)
{
	int x,y;
	hCOMPcomm = CreateCommComponent(hParent);
	memset(hxxxMisaxxxItems,0,sizeof(hxxxMisaxxxItems));
	x = 4;
	y = bmEMPTYTITLEBAR.YSize;
	hxxxMisaxxxItems[xxxMISAxxx_LEFTARROW] = BUTTON_CreateAsChild(x,y,bmLEFTARROW_UN.XSize,bmLEFTARROW_UN.YSize,hParent,xxxMISAxxx_ID_LEFTARROW,WM_CF_SHOW|WM_CF_MEMDEV);
	BUTTON_SetFocussable(hxxxMisaxxxItems[xxxMISAxxx_LEFTARROW],0);
	BUTTON_SetBitmap(hxxxMisaxxxItems[xxxMISAxxx_LEFTARROW],BUTTON_CI_UNPRESSED,&bmLEFTARROW_UN);
	BUTTON_SetBitmap(hxxxMisaxxxItems[xxxMISAxxx_LEFTARROW],BUTTON_CI_PRESSED,&bmLEFTARROW_SE);
	x += bmLEFTARROW_UN.XSize+356;	
	hxxxMisaxxxItems[xxxMISAxxx_RIGHTARROW] = BUTTON_CreateAsChild(x,y,bmRIGHTARROW_UN.XSize,bmRIGHTARROW_UN.YSize,hParent,xxxMISAxxx_ID_RIGHTARROW,WM_CF_SHOW|WM_CF_MEMDEV);
	BUTTON_SetFocussable(hxxxMisaxxxItems[xxxMISAxxx_RIGHTARROW],0);
	BUTTON_SetBitmap(hxxxMisaxxxItems[xxxMISAxxx_RIGHTARROW],BUTTON_CI_UNPRESSED,&bmRIGHTARROW_UN);
	BUTTON_SetBitmap(hxxxMisaxxxItems[xxxMISAxxx_RIGHTARROW],BUTTON_CI_PRESSED,&bmRIGHTARROW_SE);
	y += bmDISTYPEAREA.YSize;
	y += xxxMISAxxx_YOFFSET;
	x = 50;
	hxxxMisaxxxItems[xxxMISAxxx_INDICATOR] = CreateSlidePanel(x,y,WM_GetWindowSizeX(hParent)-100,bmSLIDER.YSize,hParent,WM_CF_SHOW|WM_CF_MEMDEV);
	return 0;
}

static U8 xxxMisaxxxDeleteItems()
{
	if(hxxxMisaxxxItems[xxxMISAxxx_LEFTARROW])
	{
		BUTTON_Delete(hxxxMisaxxxItems[xxxMISAxxx_LEFTARROW]);
	}
	if(hxxxMisaxxxItems[xxxMISAxxx_RIGHTARROW])
	{
		BUTTON_Delete(hxxxMisaxxxItems[xxxMISAxxx_RIGHTARROW]);
	}
	if(hxxxMisaxxxItems[xxxMISAxxx_INDICATOR])
	{
		DeleteSlidePanel(hxxxMisaxxxItems[xxxMISAxxx_INDICATOR]);
	}
	DeleteCommComponent(hCOMPcomm);
	return 0;
}

static int xxxMisaxxxGetFX()
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

static U8 xxxMisaxxxLoadParameter(int index, U8 update)
{
#if 0	
	MisaProgressbar_SetPercentEx(hxxxMisaxxxItems[xxxMISAxxx_PROGMODRATE], pModpresets[index]->rate,update);
	MisaProgressbar_SetPercentEx(hxxxMisaxxxItems[xxxMISAxxx_PROGMODDEPTH], pModpresets[index]->depth,update);
	MisaProgressbar_SetPercentEx(hxxxMisaxxxItems[xxxMISAxxx_PROGLEVEL], pModpresets[index]->level,update);
	MisaProgressbar_SetPercentEx(hxxxMisaxxxItems[xxxMISAxxx_PROGFDBK], pModpresets[index]->Feedback,update);
	MisaProgressbar_SetPercentEx(hxxxMisaxxxItems[xxxMISAxxx_PROGFDBKFILTER], pModpresets[index]->HDAMP,update);
	MisaProgressbar_SetPercentEx(hxxxMisaxxxItems[xxxMISAxxx_PROGSHAPE], pModpresets[index]->shape,update);
	MisaProgressbar_SetPercentEx(hxxxMisaxxxItems[xxxMISAxxx_PROGTIME], pModpresets[index]->delay,update);
	if(MisaCheckbox_GetStatus(hCOMPcomm.hCheckOn))
	{
		xxxMisaxxxSetPreset(xxxMisaxxxGetFX(),index);
	}
#endif
	return 0;
}

static void xxxMisaxxxProc(WM_MESSAGE* pMsg)
{
	int x, y;
	int NCode,Id;
	ProcessCommComponent(pMsg, &hCOMPcomm);
	switch (pMsg->MsgId)
	{
	case WM_CREATE:
		xxxMisaxxxCreateItems(pMsg->hWin);
		xxxMisaxxxLoadParameter(MisaIndex,0);
		break;
	case WM_DELETE:
		xxxMisaxxxDeleteItems();
		break;
	case WM_PAINT:
		GUI_SetBkColor(GUI_BLACK);
		GUI_Clear();
		GUI_SetTextMode(GUI_TEXTMODE_TRANS);
		GUI_DrawBitmap(&bmEMPTYTITLEBAR, 0, 0);
		x = 222;
		y += bmEMPTYTITLEBAR.YSize;
		GUI_DrawBitmap(&bmDISTYPEAREA, x, y);
		GUI_SetFont(&GUI_Font32B_ASCII);
		x = WM_GetWindowSizeX(pMsg->hWin);
		GUI_DispStringHCenterAt("xxxMisaxxx", x / 2, 5);
		y += 20;
		// Draw slide indicator
		y = WM_GetWindowOrgY(hxxxMisaxxxItems[xxxMISAxxx_SLIDEWINDOW]);
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
				DEBUG_STDOUT("xxxMISAxxx button clicked!");
				break;
			case WM_NOTIFICATION_RELEASED:
				switch(Id)
				{
				case xxxMISAxxx_ID_LEFTARROW:
					--MisaIndex;
					if(0>MisaIndex)
					{
						//MisaIndex = sizeof()/4-1;
					}
					WM_InvalidateRect(pMsg->hWin, &rectModType);
					xxxMisaxxxLoadParameter(MisaIndex,1);
					break;
				case xxxMISAxxx_ID_RIGHTARROW:
					++MisaIndex;
					//if(sizeof(pModpresets)/4 <= MisaIndex)
					{
						MisaIndex = 0;
					}
					WM_InvalidateRect(pMsg->hWin, &rectModType);
					xxxMisaxxxLoadParameter(MisaIndex,1);
					break;
				case COMMON_ID_CHECKON:
					if(MisaCheckbox_GetStatus(hCOMPcomm.hCheckOn))
					{
						x = MisaProgressbar_GetPercent(hxxxMisaxxxItems[xxxMISAxxx_PROGMODRATE]);
						//xxxMisaxxxSetRate(xxxMisaxxxGetFX(),x<127?x:127);
						x = MisaProgressbar_GetPercent(hxxxMisaxxxItems[xxxMISAxxx_PROGMODDEPTH]);
						//xxxMisaxxxSetDepth(xxxMisaxxxGetFX(),x<127?x:127);
						x = MisaProgressbar_GetPercent(hxxxMisaxxxItems[xxxMISAxxx_PROGLEVEL]);
						//xxxMisaxxxSetLevel(xxxMisaxxxGetFX(),x<127?x:127);
						x = MisaProgressbar_GetPercent(hxxxMisaxxxItems[xxxMISAxxx_PROGFDBK]);
						//xxxMisaxxxSetChorusFeedback(xxxMisaxxxGetFX(),x<127?x:127);
						x = MisaProgressbar_GetPercent(hxxxMisaxxxItems[xxxMISAxxx_PROGFDBKFILTER]);
						//xxxMisaxxxSetDelayFeedbackFilter(xxxMisaxxxGetFX(),x<127?x:127);
						x = MisaProgressbar_GetPercent(hxxxMisaxxxItems[xxxMISAxxx_PROGSHAPE]);
						//xxxMisaxxxSetTremolo(xxxMisaxxxGetFX(),x<127?x:127);
						x = MisaProgressbar_GetPercent(hxxxMisaxxxItems[xxxMISAxxx_PROGTIME]);
						//xxxMisaxxxSetChorusDelayTime(xxxMisaxxxGetFX(),x<127?x:127);
						//xxxMisaxxxSetPreset(xxxMisaxxxGetFX(),MisaIndex);
					}
					else
					{
						//xxxMisaxxxSetRate(xxxMisaxxxGetFX(),0);
						//xxxMisaxxxSetDepth(xxxMisaxxxGetFX(),0);
						//xxxMisaxxxSetLevel(xxxMisaxxxGetFX(),0);
						//xxxMisaxxxSetChorusFeedback(xxxMisaxxxGetFX(),0);
						//xxxMisaxxxSetDelayFeedbackFilter(xxxMisaxxxGetFX(),0);
						//xxxMisaxxxSetTremolo(xxxMisaxxxGetFX(),0);
						//xxxMisaxxxSetChorusDelayTime(xxxMisaxxxGetFX(),0);
					}
					break;
				case CTL_NONE_ID:
				case CTL_TOUCH_X_ID:
				case CTL_TOUCH_Y_ID:
				case CTL_DRAG_X_ID:
				case CTL_DRAG_Y_ID:
				case CTL_BALL_X_ID:
				case CTL_BALL_Y_ID:
				case CTL_VARIATION_ID:
					xxxMisaxxxControlMenuProc(Id);
					break;
				default:
					;
				}
				DEBUG_STDOUT("xxxMISAxxx button released!");
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
	hxxxMisaxxxItems[xxxMISAxxx_SLIDEWINDOW] = WM_CreateWindowAsChild(0,0,1300,WM_GetWindowSizeY(hParent),hParent,WM_CF_SHOW|WM_CF_MEMDEV,SlideWindowProc,0);
	return 0;
}

static U8 IndicatorDeleteItems()
{
	if(hxxxMisaxxxItems[xxxMISAxxx_SLIDEWINDOW])
	{
		WM_DeleteWindow(hxxxMisaxxxItems[xxxMISAxxx_SLIDEWINDOW]);
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
	hxxxMisaxxxItems[xxxMISAxxx_PROGMODRATE] = MisaProgressbar_CreateEx(x,y,0,0,hParent,xxxMISAxxx_ID_PROGMODRATE,"MOD","RATE",MISAPROG_FONT24,0);
	x += bmSLIDER.XSize;
	x += 100;
	hxxxMisaxxxItems[xxxMISAxxx_PROGMODDEPTH] = MisaProgressbar_CreateEx(x,y,0,0,hParent,xxxMISAxxx_ID_PROGMODDEPTH,"MOD","DEPTH",MISAPROG_FONT24,0);
	x += bmSLIDER.XSize;
	x += 100;
	hxxxMisaxxxItems[xxxMISAxxx_PROGLEVEL] = MisaProgressbar_Create(x,y,0,0,hParent,xxxMISAxxx_ID_PROGLEVEL,"LEVEL",MISAPROG_FONT24);
	x += bmSLIDER.XSize;
	x += 100;
	hxxxMisaxxxItems[xxxMISAxxx_PROGFDBK] = MisaProgressbar_Create(x,y,0,0,hParent,xxxMISAxxx_ID_PROGFDBK,"FDBK",MISAPROG_FONT24);
	x += bmSLIDER.XSize;
	x += 100;
	hxxxMisaxxxItems[xxxMISAxxx_PROGFDBKFILTER] = MisaProgressbar_CreateEx(x,y,0,0,hParent,xxxMISAxxx_ID_PROGFDBKFILTER,"FDBK","FILTER",MISAPROG_FONT24,0);
	x += bmSLIDER.XSize;
	x += 100;
	hxxxMisaxxxItems[xxxMISAxxx_PROGSHAPE] = MisaProgressbar_Create(x,y,0,0,hParent,xxxMISAxxx_ID_PROGSHAPE,"SHAPE",MISAPROG_FONT24);
	x += bmSLIDER.XSize;
	x += 100;
	hxxxMisaxxxItems[xxxMISAxxx_PROGTIME] = MisaProgressbar_Create(x,y,0,0,hParent,xxxMISAxxx_ID_PROGTIME,"TIME",MISAPROG_FONT24);
	x += bmSLIDER.XSize;
	x += 100;
	return 0;
}

static U8 SlideDeleteItems()
{
	if(hxxxMisaxxxItems[xxxMISAxxx_PROGMODRATE])
	{
		MisaProgressbar_Delete(hxxxMisaxxxItems[xxxMISAxxx_PROGMODRATE]);
	}
	if(hxxxMisaxxxItems[xxxMISAxxx_PROGMODDEPTH])
	{
		MisaProgressbar_Delete(hxxxMisaxxxItems[xxxMISAxxx_PROGMODDEPTH]);
	}
	if(hxxxMisaxxxItems[xxxMISAxxx_PROGLEVEL])
	{
		MisaProgressbar_Delete(hxxxMisaxxxItems[xxxMISAxxx_PROGLEVEL]);
	}
	if(hxxxMisaxxxItems[xxxMISAxxx_PROGFDBK])
	{
		MisaProgressbar_Delete(hxxxMisaxxxItems[xxxMISAxxx_PROGFDBK]);
	}
	if(hxxxMisaxxxItems[xxxMISAxxx_PROGFDBKFILTER])
	{
		MisaProgressbar_Delete(hxxxMisaxxxItems[xxxMISAxxx_PROGFDBKFILTER]);
	}
	if(hxxxMisaxxxItems[xxxMISAxxx_PROGSHAPE])
	{
		MisaProgressbar_Delete(hxxxMisaxxxItems[xxxMISAxxx_PROGSHAPE]);
	}
	if(hxxxMisaxxxItems[xxxMISAxxx_PROGTIME])
	{
		MisaProgressbar_Delete(hxxxMisaxxxItems[xxxMISAxxx_PROGTIME]);
	}
	return 0;
}

static void SlideWindowProc(WM_MESSAGE* pMsg)
{
	int x,y;
	int NCode,Id;
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
			if(MisaCheckbox_GetStatus(hCOMPcomm.hCheckOn))
			{
				x = MisaProgressbar_GetPercent(pMsg->hWinSrc);
				switch(Id)
				{
				case xxxMISAxxx_ID_PROGMODRATE:
					//xxxMisaxxxSetRate(xxxMisaxxxGetFX(),x<127?x:127);
					break;
				case xxxMISAxxx_ID_PROGMODDEPTH:
					//xxxMisaxxxSetDepth(xxxMisaxxxGetFX(),x<127?x:127);
					break;
				case xxxMISAxxx_ID_PROGLEVEL:
					//xxxMisaxxxSetLevel(xxxMisaxxxGetFX(),x<127?x:127);
					break;
				case xxxMISAxxx_ID_PROGFDBK:
					//xxxMisaxxxSetChorusFeedback(xxxMisaxxxGetFX(),x<127?x:127);
					break;
				case xxxMISAxxx_ID_PROGFDBKFILTER:
					//xxxMisaxxxSetDelayFeedbackFilter(xxxMisaxxxGetFX(),x<127?x:127);
					break;
				case xxxMISAxxx_ID_PROGSHAPE:
					//xxxMisaxxxSetTremolo(xxxMisaxxxGetFX(),x<127?x:127);
					break;
				case xxxMISAxxx_ID_PROGTIME:
					//xxxMisaxxxSetChorusDelayTime(xxxMisaxxxGetFX(),x<127?x:127);
					break;
				}
			}
			break;
		case WM_NOTIFICATION_SEL_CHANGED:
			x = WM_GetWindowOrgX(pMsg->hWinSrc)+50;
			y = WM_GetWindowOrgY(pMsg->hWinSrc)+340;
			MisaSetcontrolmenu_Popup(hxxxMisaxxx,pMsg->hWinSrc,x,y);
			break;
		}
		break;
	case WM_TOUCH:
		if(pMsg->Data.p)
		{
			pPID_State = (GUI_PID_STATE*)pMsg->Data.p;
			if(pPID_State->Pressed)
			{
				if(0 == PID_LastState.Pressed)
				{
					WM_SetCapture(pMsg->hWin,0);
					PID_LastState = *pPID_State;
				}
				else
				{
					WM_MoveWindow(pMsg->hWin,pPID_State->x-PID_LastState.x,0);
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
				SlidingBorder = GetSlidingBordercheck(pMsg->hWin,hxxxMisaxxx);
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

static void xxxMisaxxxControlMenuProc(int menuId)
{
	AssginEffectControlMenuProc(menuId,xxxMisaxxxGetFX());
}

#if 0 // Slide window processing
	case WM_TOUCH:
		if(pMsg->Data.p)
		{
			pPID_State = (GUI_PID_STATE*)pMsg->Data.p;
			if(pPID_State->Pressed)
			{
				if(0 == PID_LastState.Pressed)
				{
					WM_BringToTop(pMsg->hWin);
					PID_LastState = *pPID_State;
				}
				else
				{
					WM_MoveWindow(pMsg->hWin,pPID_State->x-PID_LastState.x,0/*pPID_State->y-PID_LastState.y*/);
					lx = PID_LastState.x;
					ly = PID_LastState.y;
					cx = pPID_State->x;
					cy = pPID_State->y;
					dx = pPID_State->x-PID_LastState.x;
					dy = pPID_State->y-PID_LastState.y;
				}
			}
			else
			{
				PID_LastState = *pPID_State;
			}
		}
		else
		{
			PID_LastState.Pressed = 0;
			PID_LastState.x = 0;
			PID_LastState.y = 0;
		}
		//WM_InvalidateWindow(pMsg->hWin);

#endif


#include <cstdlib>
#include <iostream>

#include "GUI.h"
#include "WM.h"
#include "BUTTON.h"

#include "MisaWidget.h"

#include "MisaAppInc.h"

#include "MainMenu.h"
#include "DelayScreen.h"

#include "MisamenuConfig.h"

#include "Resource.h"

///////////////////////////////////////////////////////////////////////////////
//

#define DELAY_TYPE_XPOS 80
#define DELAY_TYPE_YPOS 120

#define DELAY_XOFFSET 2
#define DELAY_YOFFSET 2

///////////////////////////////////////////////////////////////////////////////
//

typedef enum __DELAYITEMS
{
	DELAY_INDICATOR,
	DELAY_SLIDEWINDOW,
	DELAY_MONO,
	DELAY_STEREO,
	DELAY_PROGPRELPF,
	DELAY_PROGTIME,
	DELAY_PROGLEVEL,
	DELAY_PROGFDBK,
	DELAY_PROGFDBKFILTER,
	DELAY_MAX
} DELAYITEMS;

///////////////////////////////////////////////////////////////////////////////
//

static WM_HWIN hDelay;
static void DelayProc(WM_MESSAGE* pMsg);
static WM_HWIN hDelayItems[DELAY_MAX];
static COMMCOMPONENT hCOMPcomm;

static WM_HWIN CreateSlidePanel(int x0, int y0, int width, int height, WM_HWIN hParent, U16 Style);
static U8 DeleteSlidePanel(WM_HWIN hSlidePanel);
static U8 SlidingBorder;

static int DelayGetFX();
static void DelayControlMenuProc(int menuId);

///////////////////////////////////////////////////////////////////////////////
//

U8 CreateDelayScreen(void* pPara)
{
	int x,y;
	x=GUI_GetScreenSizeX();
	y=GUI_GetScreenSizeY();
	SlidingBorder = 1;
	hDelay = WM_CreateWindow(0, 0, x, y, /*WM_CF_SHOW | */WM_CF_MEMDEV, DelayProc, 0);
	return 0;
}

U8 DeleteDelayScreen()
{
	WM_DeleteWindow(hDelay);
	return 0;
}

U8 TopDelayScreen(WM_HWIN hPreWin)
{
	WM_HideWindow(hPreWin);
	WM_ShowWindow(hDelay);
	//WM_BringToTop(hDelay);
	return 0;
}

U8 UpdateDelayInfo(PDELAY_INFO pInfo)
{
	std::vector<int> effect;
	if(pInfo->Fx != DelayGetFX())
	{
		MisaRadio_SetStatus(hCOMPcomm.hFX1,pInfo->Fx?0:1);
		MisaRadio_SetStatus(hCOMPcomm.hFX2,pInfo->Fx?1:0);
	}
	MisaCheckbox_SetStatus(hCOMPcomm.hCheckOn,pInfo->On);
	MisaRadio_SetStatus(hDelayItems[DELAY_MONO],!pInfo->monostereo);
	MisaRadio_SetStatus(hDelayItems[DELAY_STEREO],pInfo->monostereo);
	MisaProgressbar_SetPercentEx(hDelayItems[DELAY_PROGPRELPF], pInfo->pre_lpf, 0);
	MisaProgressbar_SetPercentEx(hDelayItems[DELAY_PROGTIME], pInfo->time, 0);
	MisaProgressbar_SetPercentEx(hDelayItems[DELAY_PROGLEVEL], pInfo->level, 0);
	MisaProgressbar_SetPercentEx(hDelayItems[DELAY_PROGFDBK], pInfo->fdbk, 0);
	MisaProgressbar_SetPercentEx(hDelayItems[DELAY_PROGFDBKFILTER], pInfo->fdbkfilter, 0);
	effect = SynthFindEffect(SynthTranslateEffect(DELAY_ID_PROGPRELPF),DelayGetFX());
	MisaProgressbar_SetCheck(hDelayItems[DELAY_PROGPRELPF], effect.size()?1:0);
	effect = SynthFindEffect(SynthTranslateEffect(DELAY_ID_PROGTIME),DelayGetFX());
	MisaProgressbar_SetCheck(hDelayItems[DELAY_PROGTIME], effect.size()?1:0);
	effect = SynthFindEffect(SynthTranslateEffect(DELAY_ID_PROGLEVEL),DelayGetFX());
	MisaProgressbar_SetCheck(hDelayItems[DELAY_PROGLEVEL], effect.size()?1:0);
	effect = SynthFindEffect(SynthTranslateEffect(DELAY_ID_PROGFDBK),DelayGetFX());
	MisaProgressbar_SetCheck(hDelayItems[DELAY_PROGFDBK], effect.size()?1:0);
	effect = SynthFindEffect(SynthTranslateEffect(DELAY_ID_PROGFDBKFILTER),DelayGetFX());
	MisaProgressbar_SetCheck(hDelayItems[DELAY_PROGFDBKFILTER], effect.size()?1:0);

	SynthSetDragCentre(SynthTranslateEffect(DELAY_ID_PROGPRELPF),pInfo->Fx,pInfo->pre_lpf);
	SynthSetDragCentre(SynthTranslateEffect(DELAY_ID_PROGTIME),pInfo->Fx,pInfo->time);
	SynthSetDragCentre(SynthTranslateEffect(DELAY_ID_PROGLEVEL),pInfo->Fx,pInfo->level);
	SynthSetDragCentre(SynthTranslateEffect(DELAY_ID_PROGFDBK),pInfo->Fx,pInfo->fdbk);
	SynthSetDragCentre(SynthTranslateEffect(DELAY_ID_PROGFDBKFILTER),pInfo->Fx,pInfo->fdbkfilter);
	return 0;
}

U8 GetDelayInfo(PDELAY_INFO pInfo)
{
	pInfo->Fx = DelayGetFX();
	pInfo->On = MisaCheckbox_GetStatus(hCOMPcomm.hCheckOn);
	pInfo->monostereo = MisaRadio_GetStatus(hDelayItems[DELAY_STEREO]);
	pInfo->pre_lpf = MisaProgressbar_GetPercent(hDelayItems[DELAY_PROGPRELPF]);
	pInfo->time = MisaProgressbar_GetPercent(hDelayItems[DELAY_PROGTIME]);
	pInfo->level = MisaProgressbar_GetPercent(hDelayItems[DELAY_PROGLEVEL]);
	pInfo->fdbk = MisaProgressbar_GetPercent(hDelayItems[DELAY_PROGFDBK]);
	pInfo->fdbkfilter = MisaProgressbar_GetPercent(hDelayItems[DELAY_PROGFDBKFILTER]);
	return 0;
}

static U8 DelayCreateItems(WM_HWIN hParent)
{
	int x,y;
	hCOMPcomm = CreateCommComponent(hParent);
	memset(hDelayItems,0,sizeof(hDelayItems));
	x=0;
	y = bmEMPTYTITLEBAR.YSize;
	hDelayItems[DELAY_MONO] = MisaRadio_Create(x, y, bmDELAYMONO_UN.XSize, bmDELAYMONO_UN.YSize, hParent, DELAY_ID_MONO, WM_CF_SHOW|WM_CF_MEMDEV, &bmDELAYMONO_UN, &bmDELAYMONO_SE);
	x += bmDELAYMONO_UN.XSize;
	hDelayItems[DELAY_STEREO] = MisaRadio_Create(x, y, bmDELAYSTEREO_UN.XSize, bmDELAYSTEREO_UN.YSize, hParent, DELAY_ID_STEREO, WM_CF_SHOW|WM_CF_MEMDEV, &bmDELAYSTEREO_UN, &bmDELAYSTEREO_SE);
	y += bmDELAYSTEREO_UN.YSize;
	x = 50;
	hDelayItems[DELAY_INDICATOR] = CreateSlidePanel(x,y,WM_GetWindowSizeX(hParent)-100,bmSLIDER.YSize,hParent,WM_CF_SHOW|WM_CF_MEMDEV);
	return 0;
}

static U8 DelayDeleteItems()
{
	DeleteCommComponent(hCOMPcomm);
	return 0;
}

static int DelayGetFX()
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

static void DelayProc(WM_MESSAGE* pMsg)
{
	int x, y;
	int NCode,Id;
	ProcessCommComponent(pMsg, &hCOMPcomm);
	switch (pMsg->MsgId)
	{
	case WM_CREATE:
		DelayCreateItems(pMsg->hWin);
		break;
	case WM_DELETE:
		DelayDeleteItems();
		break;
	case WM_PAINT:
		GUI_SetBkColor(GUI_BLACK);
		GUI_Clear();
		GUI_SetTextMode(GUI_TEXTMODE_TRANS);
		GUI_DrawBitmap(&bmEMPTYTITLEBAR, 0, 0);
		GUI_SetFont(&GUI_Font32B_ASCII);
		x = WM_GetWindowSizeX(pMsg->hWin);
		GUI_DispStringHCenterAt("Delay", x / 2, 5);
		y = WM_GetWindowOrgY(hDelayItems[DELAY_SLIDEWINDOW]);
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
				DEBUG_STDOUT("DELAY button clicked!");
				break;
			case WM_NOTIFICATION_RELEASED:
				switch(Id)
				{
				case DELAY_ID_MONO:
					MisaRadio_SetStatus(hDelayItems[DELAY_STEREO],0);
					DelaySetMode(DelayGetFX(),0);
					break;
				case DELAY_ID_STEREO:
					MisaRadio_SetStatus(hDelayItems[DELAY_MONO],0);
					DelaySetMode(DelayGetFX(),1);
					break;
				case COMMON_ID_CHECKON:
					DelaySetOn(DelayGetFX(),MisaCheckbox_GetStatus(hCOMPcomm.hCheckOn)?true:false);
					break;
				case COMMON_ID_FX1:
				case COMMON_ID_FX2:
				case COMMON_ID_CLOSE:
					UpdateSynthSettingEx(MISAMODULE_DELAY);
					break;
				case CTL_NONE_ID:
				case CTL_TOUCH_X_ID:
				case CTL_TOUCH_Y_ID:
				case CTL_DRAG_X_ID:
				case CTL_DRAG_Y_ID:
				case CTL_BALL_X_ID:
				case CTL_BALL_Y_ID:
				case CTL_VARIATION_ID:
					DelayControlMenuProc(Id);
					break;
				default:
					;
				}
				DEBUG_STDOUT("DELAY button released!");
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
	hDelayItems[DELAY_SLIDEWINDOW] = WM_CreateWindowAsChild(0,0,900,WM_GetWindowSizeY(hParent),hParent,WM_CF_SHOW|WM_CF_MEMDEV,SlideWindowProc,0);
	return 0;
}

static U8 IndicatorDeleteItems()
{
	if(hDelayItems[DELAY_SLIDEWINDOW])
	{
		WM_DeleteWindow(hDelayItems[DELAY_SLIDEWINDOW]);
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
	hDelayItems[DELAY_PROGPRELPF] = MisaProgressbar_CreateEx(x,y,0,0,hParent,DELAY_ID_PROGPRELPF,"PRE","LPF",MISAPROG_FONT24,0);
	x += bmSLIDER.XSize;
	x += 100;
	hDelayItems[DELAY_PROGTIME] = MisaProgressbar_Create(x,y,0,0,hParent,DELAY_ID_PROGTIME,"TIME",MISAPROG_FONT24);
	x += bmSLIDER.XSize;
	x += 100;
	hDelayItems[DELAY_PROGLEVEL] = MisaProgressbar_Create(x,y,0,0,hParent,DELAY_ID_PROGLEVEL,"LEVEL",MISAPROG_FONT24);
	x += bmSLIDER.XSize;
	x += 100;
	hDelayItems[DELAY_PROGFDBK] = MisaProgressbar_Create(x,y,0,0,hParent,DELAY_ID_PROGFDBK,"FDBK",MISAPROG_FONT24);
	x += bmSLIDER.XSize;
	x += 100;
	hDelayItems[DELAY_PROGFDBKFILTER] = MisaProgressbar_CreateEx(x,y,0,0,hParent,DELAY_ID_PROGFDBKFILTER,"FDBK","FILTER",MISAPROG_FONT24,0);
	return 0;
}

static U8 SlideDeleteItems()
{
	if(hDelayItems[DELAY_PROGPRELPF])
	{
		MisaProgressbar_Delete(hDelayItems[DELAY_PROGPRELPF]);
	}
	if(hDelayItems[DELAY_PROGTIME])
	{
		MisaProgressbar_Delete(hDelayItems[DELAY_PROGTIME]);
	}
	if(hDelayItems[DELAY_PROGLEVEL])
	{
		MisaProgressbar_Delete(hDelayItems[DELAY_PROGLEVEL]);
	}
	if(hDelayItems[DELAY_PROGFDBK])
	{
		MisaProgressbar_Delete(hDelayItems[DELAY_PROGFDBK]);
	}
	if(hDelayItems[DELAY_PROGFDBKFILTER])
	{
		MisaProgressbar_Delete(hDelayItems[DELAY_PROGFDBKFILTER]);
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
			switch(Id)
			{
			case DELAY_ID_PROGPRELPF:
				GUI_Clear();
				break;
			default:
				;
			}
			break;
		case WM_NOTIFICATION_VALUE_CHANGED:
			x = MisaProgressbar_GetPercent(pMsg->hWinSrc);
			switch(Id)
			{
			case DELAY_ID_PROGPRELPF:
				DelaySetPreLp(DelayGetFX(),x<127?x:127);
				break;
			case DELAY_ID_PROGTIME:
				DelaySetTime(DelayGetFX(),x<127?x:127);
				break;
			case DELAY_ID_PROGLEVEL:
				DelaySetLevel(DelayGetFX(),x<127?x:127);
				break;
			case DELAY_ID_PROGFDBK:
				DelaySetFeedback(DelayGetFX(),x<127?x:127);
				break;
			case DELAY_ID_PROGFDBKFILTER:
				DelaySetFeedbackFilter(DelayGetFX(),x<127?x:127);
				break;
			}
			break;
		case WM_NOTIFICATION_SEL_CHANGED:
			x = WM_GetWindowOrgX(pMsg->hWinSrc)+50;
			y = WM_GetWindowOrgY(pMsg->hWinSrc)+340;
			MisaSetcontrolmenu_Popup(hDelay,pMsg->hWinSrc,x,y);
			effect = SynthFindEffect(SynthTranslateEffect(Id),DelayGetFX());
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
				SlidingBorder = GetSlidingBordercheck(pMsg->hWin,hDelay);
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

static void DelayControlMenuProc(int menuId)
{
	AssginEffectControlMenuProc(menuId,DelayGetFX());
}


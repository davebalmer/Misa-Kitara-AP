
#include <cstdlib>
#include <iostream>

#include "GUI.h"
#include "WM.h"
#include "BUTTON.h"

#include "MisaWidget.h"

#include "MisaAppInc.h"

#include "MainMenu.h"
#include "CompressionScreen.h"

#include "MisamenuConfig.h"

#include "Resource.h"

///////////////////////////////////////////////////////////////////////////////
// layout
// title:799*62
// 2
// 4,left arrow: 218*68,type area:356*68,right arrow:218*68,4
// 2
// slider:100*366
// common control:800*100
///////////////////////////////////////////////////////////////////////////////
//

#define COMPRESSION_XOFFSET 2
#define COMPRESSION_YOFFSET 2

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

typedef enum __COMPRESSIONITEMS
{
	COMPRESSION_INDICATOR,
	COMPRESSION_SLIDEWINDOW,
	COMPRESSION_HARD,
	COMPRESSION_SOFT,
	COMPRESSION_PROGATTACK,
	COMPRESSION_PROGRELEASE,
	COMPRESSION_PROGBOOST,
	COMPRESSION_PROGTRES,
	COMPRESSION_PROGRATIO,
	COMPRESSION_MAX
} COMPRESSIONITEMS;

///////////////////////////////////////////////////////////////////////////////
//

static WM_HWIN hCompression;
static void CompressionProc(WM_MESSAGE* pMsg);
static WM_HWIN hCompressionItems[COMPRESSION_MAX];
static COMMCOMPONENT hCOMPcomm;

static WM_HWIN CreateSlidePanel(int x0, int y0, int width, int height, WM_HWIN hParent, U16 Style);
static U8 DeleteSlidePanel(WM_HWIN hSlidePanel);
static U8 SlidingBorder;
static int CompressionGetFX();
static void CompressionControlMenuProc(int menuId);

///////////////////////////////////////////////////////////////////////////////
//

U8 CreateCompressionScreen(void* pPara)
{
	int x,y;
	x=GUI_GetScreenSizeX();
	y=GUI_GetScreenSizeY();
	SlidingBorder = 1;
	hCompression = WM_CreateWindow(0, 0, x, y, /*WM_CF_SHOW | */WM_CF_MEMDEV, CompressionProc, 0);
	return 0;
}

U8 DeleteCompressionScreen()
{
	WM_DeleteWindow(hCompression);
	return 0;
}

U8 TopCompressionScreen(WM_HWIN hPreWin)
{
	WM_HideWindow(hPreWin);
	WM_ShowWindow(hCompression);
	SetWindowToUpdateIfPresetModified(hCompression);
	return 0;
}

U8 UpdateCompressionInfo(PCOMPRESSION_INFO pInfo)
{
	std::vector<int> effect;
	if(pInfo->Fx != CompressionGetFX())
	{
		MisaRadio_SetStatus(hCOMPcomm.hFX1,pInfo->Fx?0:1);
		MisaRadio_SetStatus(hCOMPcomm.hFX2,pInfo->Fx?1:0);
	}
	MisaRadio_SetStatus(hCompressionItems[COMPRESSION_HARD],!pInfo->softhard);
	MisaRadio_SetStatus(hCompressionItems[COMPRESSION_SOFT],pInfo->softhard);
	MisaCheckbox_SetStatus(hCOMPcomm.hCheckOn,pInfo->On);
	MisaProgressbar_SetPercentEx(hCompressionItems[COMPRESSION_PROGATTACK],pInfo->attack,1);
	MisaProgressbar_SetPercentEx(hCompressionItems[COMPRESSION_PROGRELEASE],pInfo->release,1);
	MisaProgressbar_SetPercentEx(hCompressionItems[COMPRESSION_PROGBOOST],pInfo->boost,1);
	MisaProgressbar_SetPercentEx(hCompressionItems[COMPRESSION_PROGTRES],pInfo->thres,1);
	MisaProgressbar_SetPercentEx(hCompressionItems[COMPRESSION_PROGRATIO],pInfo->ratio,1);
	effect = SynthFindEffect(SynthTranslateEffect(COMPRESSION_ID_PROGATTACK),CompressionGetFX());
	MisaProgressbar_SetCheck(hCompressionItems[COMPRESSION_PROGATTACK], effect.size()?1:0);
	effect = SynthFindEffect(SynthTranslateEffect(COMPRESSION_ID_PROGRELEASE),CompressionGetFX());
	MisaProgressbar_SetCheck(hCompressionItems[COMPRESSION_PROGRELEASE], effect.size()?1:0);
	effect = SynthFindEffect(SynthTranslateEffect(COMPRESSION_ID_PROGBOOST),CompressionGetFX());
	MisaProgressbar_SetCheck(hCompressionItems[COMPRESSION_PROGBOOST], effect.size()?1:0);
	effect = SynthFindEffect(SynthTranslateEffect(COMPRESSION_ID_PROGTRES),CompressionGetFX());
	MisaProgressbar_SetCheck(hCompressionItems[COMPRESSION_PROGTRES], effect.size()?1:0);
	effect = SynthFindEffect(SynthTranslateEffect(COMPRESSION_ID_PROGRATIO),CompressionGetFX());
	MisaProgressbar_SetCheck(hCompressionItems[COMPRESSION_PROGRATIO], effect.size()?1:0);

	SynthSetDragCentre(SynthTranslateEffect(COMPRESSION_ID_PROGATTACK),pInfo->Fx,pInfo->attack);
	SynthSetDragCentre(SynthTranslateEffect(COMPRESSION_ID_PROGRELEASE),pInfo->Fx,pInfo->release);
	SynthSetDragCentre(SynthTranslateEffect(COMPRESSION_ID_PROGBOOST),pInfo->Fx,pInfo->boost);
	SynthSetDragCentre(SynthTranslateEffect(COMPRESSION_ID_PROGTRES),pInfo->Fx,pInfo->thres);
	SynthSetDragCentre(SynthTranslateEffect(COMPRESSION_ID_PROGRATIO),pInfo->Fx,pInfo->ratio);
	return 0;
}

U8 GetCompressionInfo(PCOMPRESSION_INFO pInfo)
{
	pInfo->Fx = CompressionGetFX();
	pInfo->On = MisaCheckbox_GetStatus(hCOMPcomm.hCheckOn);
	pInfo->softhard = MisaRadio_GetStatus(hCompressionItems[COMPRESSION_SOFT]);
	pInfo->attack = MisaProgressbar_GetPercent(hCompressionItems[COMPRESSION_PROGATTACK]);
	pInfo->release= MisaProgressbar_GetPercent(hCompressionItems[COMPRESSION_PROGRELEASE]);
	pInfo->boost = MisaProgressbar_GetPercent(hCompressionItems[COMPRESSION_PROGBOOST]);
	pInfo->thres = MisaProgressbar_GetPercent(hCompressionItems[COMPRESSION_PROGTRES]);
	pInfo->ratio = MisaProgressbar_GetPercent(hCompressionItems[COMPRESSION_PROGRATIO]);
	return 0;
}

static U8 CompressionCreateItems(WM_HWIN hParent)
{
	int x,y;
	memset(hCompressionItems,0,sizeof(hCompressionItems));
	hCOMPcomm = CreateCommComponent(hParent);
	x = 1;
	y = bmEMPTYTITLEBAR.YSize;
	hCompressionItems[COMPRESSION_HARD] = MisaRadio_Create(x, y, bmCOMHARD_UN.XSize, bmCOMHARD_UN.YSize, hParent, COMPRESSION_ID_HARD, WM_CF_SHOW|WM_CF_MEMDEV, &bmCOMHARD_UN, &bmCOMHARD_SE);
	x += bmCOMHARD_UN.XSize;
	hCompressionItems[COMPRESSION_SOFT] = MisaRadio_Create(x, y, bmCOMSOFT_UN.XSize, bmCOMSOFT_UN.YSize, hParent, COMPRESSION_ID_SOFT, WM_CF_SHOW|WM_CF_MEMDEV, &bmCOMSOFT_UN, &bmCOMSOFT_SE);
	y += bmCOMHARD_UN.YSize;
	x = 50;
	hCompressionItems[COMPRESSION_INDICATOR] = CreateSlidePanel(x,y,WM_GetWindowSizeX(hParent)-100,bmSLIDER.YSize,hParent,WM_CF_SHOW|WM_CF_MEMDEV);
	return 0;
}

static U8 CompressionDeleteItems()
{
	if(hCompressionItems[COMPRESSION_HARD])
	{
		MisaRadio_Delete(hCompressionItems[COMPRESSION_HARD]);
	}
	if(hCompressionItems[COMPRESSION_SOFT])
	{
		MisaRadio_Delete(hCompressionItems[COMPRESSION_SOFT]);
	}
	if(hCompressionItems[COMPRESSION_INDICATOR])
	{
		DeleteSlidePanel(hCompressionItems[COMPRESSION_INDICATOR]);
	}
	/*if(hCompressionItems[COMPRESSION_PROGATTACK])
	{
		MisaProgressbar_Delete(hCompressionItems[COMPRESSION_PROGATTACK]);
	}
	if(hCompressionItems[COMPRESSION_PROGRELEASE])
	{
		MisaProgressbar_Delete(hCompressionItems[COMPRESSION_PROGRELEASE]);
	}
	if(hCompressionItems[COMPRESSION_PROGBOOST])
	{
		MisaProgressbar_Delete(hCompressionItems[COMPRESSION_PROGBOOST]);
	}
	if(hCompressionItems[COMPRESSION_PROGTRES])
	{
		MisaProgressbar_Delete(hCompressionItems[COMPRESSION_PROGTRES]);
	}
	if(hCompressionItems[COMPRESSION_PROGRATIO])
	{
		MisaProgressbar_Delete(hCompressionItems[COMPRESSION_PROGRATIO]);
	}*/
	DeleteCommComponent(hCOMPcomm);
	return 0;
}

static int CompressionGetFX()
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

static void CompressionProc(WM_MESSAGE* pMsg)
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
		CompressionCreateItems(pMsg->hWin);
		break;
	case WM_DELETE:
		CompressionDeleteItems();
		break;
	case WM_PAINT:
		GUI_SetBkColor(GUI_BLACK);
		GUI_Clear();
		GUI_SetTextMode(GUI_TEXTMODE_TRANS);
		GUI_DrawBitmap(&bmEMPTYTITLEBAR, 0, 0);
		GUI_SetFont(&GUI_Font32B_ASCII);
		x = WM_GetWindowSizeX(pMsg->hWin);
		{
			std::string title("Compression");
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

		y = WM_GetWindowOrgY(hCompressionItems[COMPRESSION_SLIDEWINDOW]);
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
				DEBUG_STDOUT("Compression button clicked!");
				break;
			case WM_NOTIFICATION_RELEASED:
				switch(Id)
				{
				case COMPRESSION_ID_HARD:
					MisaRadio_SetStatus(hCompressionItems[COMPRESSION_SOFT],0);
					CompressorSetKnee(CompressionGetFX(),0);
					break;
				case COMPRESSION_ID_SOFT:
					MisaRadio_SetStatus(hCompressionItems[COMPRESSION_HARD],0);
					CompressorSetKnee(CompressionGetFX(),1);
					break;
				case COMMON_ID_CHECKON:
					CompressorSetOn(CompressionGetFX(),MisaCheckbox_GetStatus(hCOMPcomm.hCheckOn)?true:false);
					break;
				case COMMON_ID_FX1:
				case COMMON_ID_FX2:
				case COMMON_ID_CLOSE:
					UpdateSynthSettingEx(MISAMODULE_COMPRESSION);
					break;
				case CTL_NONE_ID:
				case CTL_TOUCH_X_ID:
				case CTL_TOUCH_Y_ID:
				case CTL_DRAG_X_ID:
				case CTL_DRAG_Y_ID:
				case CTL_BALL_X_ID:
				case CTL_BALL_Y_ID:
				case CTL_VARIATION_ID:
					CompressionControlMenuProc(Id);
					break;
				default:
					;
				}
				DEBUG_STDOUT("Compression button released!");
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
						SlidingBorder = SlideGoPreviousPage(hCompressionItems[COMPRESSION_SLIDEWINDOW]);
					}
					else if(GUI_RectsIntersect(&rightsidebutton,&ptRect))
					{
						sidebutton = 2;
						WM_InvalidateRect(pMsg->hWin, &rightsidebutton);
						SlidingBorder = SlideGoNextPage(hCompressionItems[COMPRESSION_SLIDEWINDOW]);
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
	hCompressionItems[COMPRESSION_SLIDEWINDOW] = WM_CreateWindowAsChild(0,0,900,WM_GetWindowSizeY(hParent),hParent,WM_CF_SHOW|WM_CF_MEMDEV,SlideWindowProc,0);
	return 0;
}

static U8 IndicatorDeleteItems()
{
	if(hCompressionItems[COMPRESSION_SLIDEWINDOW])
	{
		WM_DeleteWindow(hCompressionItems[COMPRESSION_SLIDEWINDOW]);
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
	hCompressionItems[COMPRESSION_PROGATTACK] = MisaProgressbar_Create(x,y,0,0,hParent,COMPRESSION_ID_PROGATTACK,"ATTACK",MISAPROG_FONT16);
	x += bmSLIDER.XSize;
	x += 100;
	hCompressionItems[COMPRESSION_PROGRELEASE] = MisaProgressbar_Create(x,y,0,0,hParent,COMPRESSION_ID_PROGRELEASE,"RELEASE",MISAPROG_FONT16);
	x += bmSLIDER.XSize;
	x += 100;
	hCompressionItems[COMPRESSION_PROGBOOST] = MisaProgressbar_Create(x,y,0,0,hParent,COMPRESSION_ID_PROGBOOST,"BOOST",MISAPROG_FONT16);
	x += bmSLIDER.XSize;
	x += 100;
	hCompressionItems[COMPRESSION_PROGTRES] = MisaProgressbar_Create(x,y,0,0,hParent,COMPRESSION_ID_PROGTRES,"THRES",MISAPROG_FONT16);
	x += bmSLIDER.XSize;
	x += 100;
	hCompressionItems[COMPRESSION_PROGRATIO] = MisaProgressbar_Create(x,y,0,0,hParent,COMPRESSION_ID_PROGRATIO,"RATIO",MISAPROG_FONT16);
	return 0;
}

static U8 SlideDeleteItems()
{
	if(hCompressionItems[COMPRESSION_PROGATTACK])
	{
		MisaProgressbar_Delete(hCompressionItems[COMPRESSION_PROGATTACK]);
	}
	if(hCompressionItems[COMPRESSION_PROGRELEASE])
	{
		MisaProgressbar_Delete(hCompressionItems[COMPRESSION_PROGRELEASE]);
	}
	if(hCompressionItems[COMPRESSION_PROGBOOST])
	{
		MisaProgressbar_Delete(hCompressionItems[COMPRESSION_PROGBOOST]);
	}
	if(hCompressionItems[COMPRESSION_PROGTRES])
	{
		MisaProgressbar_Delete(hCompressionItems[COMPRESSION_PROGTRES]);
	}
	if(hCompressionItems[COMPRESSION_PROGRATIO])
	{
		MisaProgressbar_Delete(hCompressionItems[COMPRESSION_PROGRATIO]);
	}
	return 0;
}

static void SlideWindowProc(WM_MESSAGE* pMsg)
{
	int x,y;
	int NCode,Id;
	std::vector<int> effect;
	// No more sliding GUI_PID_STATE* pPID_State;
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
#if 0
		case WM_NOTIFICATION_RELEASED:
			x = MisaProgressbar_GetPercent(pMsg->hWinSrc);
			switch(Id)
			{
			case COMPRESSION_ID_PROGATTACK:
				CompressorSetAttack(CompressionGetFX(),x<127?x:127);
				break;
			case COMPRESSION_ID_PROGRELEASE:
				CompressorSetRelease(CompressionGetFX(),x<127?x:127);
				break;
			case COMPRESSION_ID_PROGBOOST:
				CompressorSetBoost(CompressionGetFX(),x<127?x:127);
				break;
			case COMPRESSION_ID_PROGTRES:
				CompressorSetThreshold(CompressionGetFX(),x<127?x:127);
				break;
			case COMPRESSION_ID_PROGRATIO:
				CompressorSetRatio(CompressionGetFX(),x<127?x:127);
				break;
			default:
				;
			}
			break;
#endif
		case WM_NOTIFICATION_VALUE_CHANGED:
			x = MisaProgressbar_GetPercent(pMsg->hWinSrc);
			switch(Id)
			{
			case COMPRESSION_ID_PROGATTACK:
				CompressorSetAttack(CompressionGetFX(),x<127?x:127);
				break;
			case COMPRESSION_ID_PROGRELEASE:
				CompressorSetRelease(CompressionGetFX(),x<127?x:127);
				break;
			case COMPRESSION_ID_PROGBOOST:
				CompressorSetBoost(CompressionGetFX(),x<127?x:127);
				break;
			case COMPRESSION_ID_PROGTRES:
				CompressorSetThreshold(CompressionGetFX(),x<127?x:127);
				break;
			case COMPRESSION_ID_PROGRATIO:
				CompressorSetRatio(CompressionGetFX(),x<127?x:127);
				break;
			default:
				;
			}
			break;
		case WM_NOTIFICATION_SEL_CHANGED:
			x = WM_GetWindowOrgX(pMsg->hWinSrc)+50;
			y = WM_GetWindowOrgY(pMsg->hWinSrc)+340;
			MisaSetcontrolmenu_Popup(hCompression,pMsg->hWinSrc,x,y);
			effect = SynthFindEffect(SynthTranslateEffect(Id),CompressionGetFX());
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
				SlidingBorder = GetSlidingBordercheck(pMsg->hWin,hCompression);
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

static void CompressionControlMenuProc(int menuId)
{
	AssignEffectControlMenuProc(menuId,CompressionGetFX());
}


#include <cstdlib>
#include <iostream>

#include "GUI.h"
#include "WM.h"
#include "TEXT.h"
#include "BUTTON.h"

#include "MisaWidget.h"

#include "MisaAppInc.h"

#include "MainMenu.h"
#include "MixerScreen.h"

#include "EqScreen.h"

#include "MisamenuConfig.h"

#include "Resource.h"

#define MISA_SMALLPROGRESSBARLAYOUT

///////////////////////////////////////////////////////////////////////////////
//

#define EQ_TYPE_XPOS 80
#define EQ_TYPE_YPOS 120

#define EQ_XOFFSET 2
#define EQ_YOFFSET 2

///////////////////////////////////////////////////////////////////////////////
//

typedef enum __EQITEMS
{
	EQ_INDICATOR,
	EQ_SLIDEWINDOW,
	EQ_TOMIXERSCREEN,
	EQ_LOWBANDGAIN,
	EQ_LOWBANDFREQ,
	EQ_LOWMIDBANDQ,
	EQ_LOWMIDBANDGAIN,
	EQ_LOWMIDBANDFREQ,
	EQ_HIGHMIDBANDGAIN,
	EQ_HIGHMIDBANDFREQ,
	EQ_HIGHMIDBANDQ,
	EQ_HIGHBANDGAIN,
	EQ_HIGHBANDFREQ,
	EQ_MAX
} EQITEMS;

///////////////////////////////////////////////////////////////////////////////
//

static WM_HWIN hEq;
static void EqProc(WM_MESSAGE* pMsg);
static WM_HWIN hEqItems[EQ_MAX];
static COMMCOMPONENT hCOMPcomm;

static WM_HWIN CreateSlidePanel(int x0, int y0, int width, int height, WM_HWIN hParent, U16 Style);
static U8 DeleteSlidePanel(WM_HWIN hSlidePanel);
static U8 SlidingBorder;

#ifdef MISA_SMALLPROGRESSBARLAYOUT

#define EQ_PAGE_POSITIVE_FACTOR 175
#define EQ_PAGE_NEGATIVE_FACTOR -175

static const GUI_RECT leftsidebutton=
{
	0,64,39,498
};
static const GUI_RECT rightsidebutton=
{
	760,64,799,498
};

static void EqSlideGoNextPage();
static void EqSlideGoPreviousPage();

#endif // MISA_SMALLPROGRESSBARLAYOUT

static void EqControlMenuProc(int menuId);

///////////////////////////////////////////////////////////////////////////////
//

U8 CreateEqScreen(void* pPara)
{
	int x,y;
	x=GUI_GetScreenSizeX();
	y=GUI_GetScreenSizeY();
	SlidingBorder = 1;
	hEq = WM_CreateWindow(0, 0, x, y, /*WM_CF_SHOW | */WM_CF_MEMDEV, EqProc, 0);
	return 0;
}

U8 DeleteEqScreen()
{
	WM_DeleteWindow(hEq);
	return 0;
}

static U8 EnableEqWindow();

U8 TopEqScreen(WM_HWIN hPreWin)
{
	WM_HideWindow(hPreWin);
	WM_ShowWindow(hEq);
	//WM_BringToTop(hEq);
	return 0;
}

static U8 EnableEqWindow()
{
	if(hEq)
	{
		WM_EnableWindow(hEq);
	}
	if(hEqItems[EQ_TOMIXERSCREEN])
	{
		WM_EnableWindow(hEqItems[EQ_TOMIXERSCREEN]);
	}
	if(hEqItems[EQ_INDICATOR])
	{
		WM_EnableWindow(hEqItems[EQ_INDICATOR]);
	}
	if(hEqItems[EQ_SLIDEWINDOW])
	{
		WM_EnableWindow(hEqItems[EQ_SLIDEWINDOW]);
	}
	if(hEqItems[EQ_LOWBANDGAIN])
	{
		WM_EnableWindow(hEqItems[EQ_LOWBANDGAIN]);
	}
	if(hEqItems[EQ_LOWBANDFREQ])
	{
		WM_EnableWindow(hEqItems[EQ_LOWBANDFREQ]);
	}
	if(hEqItems[EQ_LOWMIDBANDQ])
	{
		WM_EnableWindow(hEqItems[EQ_LOWMIDBANDQ]);
	}
	if(hEqItems[EQ_LOWMIDBANDGAIN])
	{
		WM_EnableWindow(hEqItems[EQ_LOWMIDBANDGAIN]);
	}
	if(hEqItems[EQ_LOWMIDBANDFREQ])
	{
		WM_EnableWindow(hEqItems[EQ_LOWMIDBANDFREQ]);
	}
	if(hEqItems[EQ_HIGHMIDBANDGAIN])
	{
		WM_EnableWindow(hEqItems[EQ_HIGHMIDBANDGAIN]);
	}
	if(hEqItems[EQ_HIGHMIDBANDFREQ])
	{
		WM_EnableWindow(hEqItems[EQ_HIGHMIDBANDFREQ]);
	}
	if(hEqItems[EQ_HIGHMIDBANDQ])
	{
		WM_EnableWindow(hEqItems[EQ_HIGHMIDBANDQ]);
	}
	if(hEqItems[EQ_HIGHBANDGAIN])
	{
		WM_EnableWindow(hEqItems[EQ_HIGHBANDGAIN]);
	}
	if(hEqItems[EQ_HIGHBANDFREQ])
	{
		WM_EnableWindow(hEqItems[EQ_HIGHBANDFREQ]);
	}
	return 0;
}

static U8 DisableEqWindow()
{
	if(hEq)
	{
		WM_DisableWindow(hEq);
	}
	if(hEqItems[EQ_TOMIXERSCREEN])
	{
		WM_DisableWindow(hEqItems[EQ_TOMIXERSCREEN]);
	}
	if(hEqItems[EQ_INDICATOR])
	{
		WM_DisableWindow(hEqItems[EQ_INDICATOR]);
	}
	if(hEqItems[EQ_SLIDEWINDOW])
	{
		WM_DisableWindow(hEqItems[EQ_SLIDEWINDOW]);
	}
	if(hEqItems[EQ_LOWBANDGAIN])
	{
		WM_DisableWindow(hEqItems[EQ_LOWBANDGAIN]);
	}
	if(hEqItems[EQ_LOWBANDFREQ])
	{
		WM_DisableWindow(hEqItems[EQ_LOWBANDFREQ]);
	}
	if(hEqItems[EQ_LOWMIDBANDQ])
	{
		WM_DisableWindow(hEqItems[EQ_LOWMIDBANDQ]);
	}
	if(hEqItems[EQ_LOWMIDBANDGAIN])
	{
		WM_DisableWindow(hEqItems[EQ_LOWMIDBANDGAIN]);
	}
	if(hEqItems[EQ_LOWMIDBANDFREQ])
	{
		WM_DisableWindow(hEqItems[EQ_LOWMIDBANDFREQ]);
	}
	if(hEqItems[EQ_HIGHMIDBANDGAIN])
	{
		WM_DisableWindow(hEqItems[EQ_HIGHMIDBANDGAIN]);
	}
	if(hEqItems[EQ_HIGHMIDBANDFREQ])
	{
		WM_DisableWindow(hEqItems[EQ_HIGHMIDBANDFREQ]);
	}
	if(hEqItems[EQ_HIGHMIDBANDQ])
	{
		WM_DisableWindow(hEqItems[EQ_HIGHMIDBANDQ]);
	}
	if(hEqItems[EQ_HIGHBANDGAIN])
	{
		WM_DisableWindow(hEqItems[EQ_HIGHBANDGAIN]);
	}
	if(hEqItems[EQ_HIGHBANDFREQ])
	{
		WM_DisableWindow(hEqItems[EQ_HIGHBANDFREQ]);
	}
	return 0;
}

U8 UpdateEqInfo(PEQ_INFO pInfo)
{
#ifdef MISA_SMALLPROGRESSBARLAYOUT
	std::vector<int> effect;
	MisaCheckbox_SetStatus(hCOMPcomm.hCheckOn,pInfo->On);
	MisaSmallProgressCombo_SetPercentEx(hEqItems[EQ_LOWBANDGAIN],pInfo->lowbandgain,0);
	MisaSmallProgressCombo_SetPercentEx(hEqItems[EQ_LOWBANDFREQ],pInfo->lowbandfreq,0);
	MisaSmallProgressCombo_SetPercentEx(hEqItems[EQ_LOWMIDBANDQ],pInfo->lowmidbandQ,0);
	MisaSmallProgressCombo_SetPercentEx(hEqItems[EQ_LOWMIDBANDGAIN],pInfo->lowmidbandgain,0);
	MisaSmallProgressCombo_SetPercentEx(hEqItems[EQ_LOWMIDBANDFREQ],pInfo->lowmidbandfreq,0);
	MisaSmallProgressCombo_SetPercentEx(hEqItems[EQ_HIGHMIDBANDGAIN],pInfo->highmidbandgain,0);
	MisaSmallProgressCombo_SetPercentEx(hEqItems[EQ_HIGHMIDBANDFREQ],pInfo->highmidbandfreq,0);
	MisaSmallProgressCombo_SetPercentEx(hEqItems[EQ_HIGHMIDBANDQ],pInfo->highmidbandQ,0);
	MisaSmallProgressCombo_SetPercentEx(hEqItems[EQ_HIGHBANDGAIN],pInfo->highbandgain,0);
	MisaSmallProgressCombo_SetPercentEx(hEqItems[EQ_HIGHBANDFREQ],pInfo->highbandfreq,0);
	effect = SynthFindEffect(SynthTranslateEffect(EQ_LOWBANDGAIN_ID),0);
	MisaSmallProgressCombo_SetCheck(hEqItems[EQ_LOWBANDGAIN], effect.size()?1:0);
	effect = SynthFindEffect(SynthTranslateEffect(EQ_LOWBANDFREQ_ID),0);
	MisaSmallProgressCombo_SetCheck(hEqItems[EQ_LOWBANDFREQ], effect.size()?1:0);
	effect = SynthFindEffect(SynthTranslateEffect(EQ_LOWMIDBANDQ_ID),0);
	MisaSmallProgressCombo_SetCheck(hEqItems[EQ_LOWMIDBANDQ], effect.size()?1:0);
	effect = SynthFindEffect(SynthTranslateEffect(EQ_LOWMIDBANDGAIN_ID),0);
	MisaSmallProgressCombo_SetCheck(hEqItems[EQ_LOWMIDBANDGAIN], effect.size()?1:0);
	effect = SynthFindEffect(SynthTranslateEffect(EQ_LOWMIDBANDFREQ_ID),0);
	MisaSmallProgressCombo_SetCheck(hEqItems[EQ_LOWMIDBANDFREQ], effect.size()?1:0);
	effect = SynthFindEffect(SynthTranslateEffect(EQ_HIGHMIDBANDGAIN_ID),0);
	MisaSmallProgressCombo_SetCheck(hEqItems[EQ_HIGHMIDBANDGAIN], effect.size()?1:0);
	effect = SynthFindEffect(SynthTranslateEffect(EQ_HIGHMIDBANDFREQ_ID),0);
	MisaSmallProgressCombo_SetCheck(hEqItems[EQ_HIGHMIDBANDFREQ], effect.size()?1:0);
	effect = SynthFindEffect(SynthTranslateEffect(EQ_HIGHMIDBANDQ_ID),0);
	MisaSmallProgressCombo_SetCheck(hEqItems[EQ_HIGHMIDBANDQ], effect.size()?1:0);
	effect = SynthFindEffect(SynthTranslateEffect(EQ_HIGHBANDGAIN_ID),0);
	MisaSmallProgressCombo_SetCheck(hEqItems[EQ_HIGHBANDGAIN], effect.size()?1:0);
	effect = SynthFindEffect(SynthTranslateEffect(EQ_HIGHBANDFREQ_ID),0);
	MisaSmallProgressCombo_SetCheck(hEqItems[EQ_HIGHBANDFREQ], effect.size()?1:0);

	SynthSetDragCentre(SynthTranslateEffect(EQ_LOWBANDGAIN_ID),0,pInfo->lowbandgain);
	SynthSetDragCentre(SynthTranslateEffect(EQ_LOWBANDFREQ_ID),0,pInfo->lowbandfreq);
	SynthSetDragCentre(SynthTranslateEffect(EQ_LOWMIDBANDQ_ID),0,pInfo->lowmidbandQ);
	SynthSetDragCentre(SynthTranslateEffect(EQ_LOWMIDBANDGAIN_ID),0,pInfo->lowmidbandgain);
	SynthSetDragCentre(SynthTranslateEffect(EQ_LOWMIDBANDFREQ_ID),0,pInfo->lowmidbandfreq);
	SynthSetDragCentre(SynthTranslateEffect(EQ_HIGHMIDBANDGAIN_ID),0,pInfo->highmidbandgain);
	SynthSetDragCentre(SynthTranslateEffect(EQ_HIGHMIDBANDFREQ_ID),0,pInfo->highmidbandfreq);
	SynthSetDragCentre(SynthTranslateEffect(EQ_HIGHMIDBANDQ_ID),0,pInfo->highmidbandQ);
	SynthSetDragCentre(SynthTranslateEffect(EQ_HIGHBANDGAIN_ID),0,pInfo->highbandgain);
	SynthSetDragCentre(SynthTranslateEffect(EQ_HIGHBANDFREQ_ID),0,pInfo->highbandfreq);
#else
	std::vector<int> effect;
	MisaCheckbox_SetStatus(hCOMPcomm.hCheckOn,pInfo->On);
	MisaProgressbar_SetPercentEx(hEqItems[EQ_LOWBANDGAIN],pInfo->lowbandgain,0);
	MisaProgressbar_SetPercentEx(hEqItems[EQ_LOWBANDFREQ],pInfo->lowbandfreq,0);
	MisaProgressbar_SetPercentEx(hEqItems[EQ_LOWMIDBANDQ],pInfo->lowmidbandQ,0);
	MisaProgressbar_SetPercentEx(hEqItems[EQ_LOWMIDBANDGAIN],pInfo->lowmidbandgain,0);
	MisaProgressbar_SetPercentEx(hEqItems[EQ_LOWMIDBANDFREQ],pInfo->lowmidbandfreq,0);
	MisaProgressbar_SetPercentEx(hEqItems[EQ_HIGHMIDBANDGAIN],pInfo->highmidbandgain,0);
	MisaProgressbar_SetPercentEx(hEqItems[EQ_HIGHMIDBANDFREQ],pInfo->highmidbandfreq,0);
	MisaProgressbar_SetPercentEx(hEqItems[EQ_HIGHMIDBANDQ],pInfo->highmidbandQ,0);
	MisaProgressbar_SetPercentEx(hEqItems[EQ_HIGHBANDGAIN],pInfo->highbandgain,0);
	MisaProgressbar_SetPercentEx(hEqItems[EQ_HIGHBANDFREQ],pInfo->highbandfreq,0);
	effect = SynthFindEffect(SynthTranslateEffect(EQ_LOWBANDGAIN_ID),0);
	MisaProgressbar_SetCheck(hEqItems[EQ_LOWBANDGAIN], effect.size()?1:0);
	effect = SynthFindEffect(SynthTranslateEffect(EQ_LOWBANDFREQ_ID),0);
	MisaProgressbar_SetCheck(hEqItems[EQ_LOWBANDFREQ], effect.size()?1:0);
	effect = SynthFindEffect(SynthTranslateEffect(EQ_LOWMIDBANDQ_ID),0);
	MisaProgressbar_SetCheck(hEqItems[EQ_LOWMIDBANDQ], effect.size()?1:0);
	effect = SynthFindEffect(SynthTranslateEffect(EQ_LOWMIDBANDGAIN_ID),0);
	MisaProgressbar_SetCheck(hEqItems[EQ_LOWMIDBANDGAIN], effect.size()?1:0);
	effect = SynthFindEffect(SynthTranslateEffect(EQ_LOWMIDBANDFREQ_ID),0);
	MisaProgressbar_SetCheck(hEqItems[EQ_LOWMIDBANDFREQ], effect.size()?1:0);
	effect = SynthFindEffect(SynthTranslateEffect(EQ_HIGHMIDBANDGAIN_ID),0);
	MisaProgressbar_SetCheck(hEqItems[EQ_HIGHMIDBANDGAIN], effect.size()?1:0);
	effect = SynthFindEffect(SynthTranslateEffect(EQ_HIGHMIDBANDFREQ_ID),0);
	MisaProgressbar_SetCheck(hEqItems[EQ_HIGHMIDBANDFREQ], effect.size()?1:0);
	effect = SynthFindEffect(SynthTranslateEffect(EQ_HIGHMIDBANDQ_ID),0);
	MisaProgressbar_SetCheck(hEqItems[EQ_HIGHMIDBANDQ], effect.size()?1:0);
	effect = SynthFindEffect(SynthTranslateEffect(EQ_HIGHBANDGAIN_ID),0);
	MisaProgressbar_SetCheck(hEqItems[EQ_HIGHBANDGAIN], effect.size()?1:0);
	effect = SynthFindEffect(SynthTranslateEffect(EQ_HIGHBANDFREQ_ID),0);
	MisaProgressbar_SetCheck(hEqItems[EQ_HIGHBANDFREQ], effect.size()?1:0);

	SynthSetDragCentre(SynthTranslateEffect(EQ_LOWBANDGAIN_ID),0,pInfo->lowbandgain);
	SynthSetDragCentre(SynthTranslateEffect(EQ_LOWBANDFREQ_ID),0,pInfo->lowbandfreq);
	SynthSetDragCentre(SynthTranslateEffect(EQ_LOWMIDBANDQ_ID),0,pInfo->lowmidbandQ);
	SynthSetDragCentre(SynthTranslateEffect(EQ_LOWMIDBANDGAIN_ID),0,pInfo->lowmidbandgain);
	SynthSetDragCentre(SynthTranslateEffect(EQ_LOWMIDBANDFREQ_ID),0,pInfo->lowmidbandfreq);
	SynthSetDragCentre(SynthTranslateEffect(EQ_HIGHMIDBANDGAIN_ID),0,pInfo->highmidbandgain);
	SynthSetDragCentre(SynthTranslateEffect(EQ_HIGHMIDBANDFREQ_ID),0,pInfo->highmidbandfreq);
	SynthSetDragCentre(SynthTranslateEffect(EQ_HIGHMIDBANDQ_ID),0,pInfo->highmidbandQ);
	SynthSetDragCentre(SynthTranslateEffect(EQ_HIGHBANDGAIN_ID),0,pInfo->highbandgain);
	SynthSetDragCentre(SynthTranslateEffect(EQ_HIGHBANDFREQ_ID),0,pInfo->highbandfreq);
#endif
	return 0;
}

U8 GetEqInfo(PEQ_INFO pInfo)
{
#ifdef MISA_SMALLPROGRESSBARLAYOUT
	pInfo->lowbandgain = MisaSmallProgressCombo_GetPercent(hEqItems[EQ_LOWBANDGAIN]);
	pInfo->lowbandfreq = MisaSmallProgressCombo_GetPercent(hEqItems[EQ_LOWBANDFREQ]);
	pInfo->lowmidbandQ = MisaSmallProgressCombo_GetPercent(hEqItems[EQ_LOWMIDBANDQ]);
	pInfo->lowmidbandgain = MisaSmallProgressCombo_GetPercent(hEqItems[EQ_LOWMIDBANDGAIN]);
	pInfo->lowmidbandfreq = MisaSmallProgressCombo_GetPercent(hEqItems[EQ_LOWMIDBANDFREQ]);
	pInfo->highmidbandgain= MisaSmallProgressCombo_GetPercent(hEqItems[EQ_HIGHMIDBANDGAIN]);
	pInfo->highmidbandfreq = MisaSmallProgressCombo_GetPercent(hEqItems[EQ_HIGHMIDBANDFREQ]);
	pInfo->highmidbandQ = MisaSmallProgressCombo_GetPercent(hEqItems[EQ_HIGHMIDBANDQ]);
	pInfo->highbandgain = MisaSmallProgressCombo_GetPercent(hEqItems[EQ_HIGHBANDGAIN]);
	pInfo->highbandfreq = MisaSmallProgressCombo_GetPercent(hEqItems[EQ_HIGHBANDFREQ]);
	pInfo->On = MisaCheckbox_GetStatus(hCOMPcomm.hCheckOn);
#else
	pInfo->lowbandgain = MisaProgressbar_GetPercent(hEqItems[EQ_LOWBANDGAIN]);
	pInfo->lowbandfreq = MisaProgressbar_GetPercent(hEqItems[EQ_LOWBANDFREQ]);
	pInfo->lowmidbandQ = MisaProgressbar_GetPercent(hEqItems[EQ_LOWMIDBANDQ]);
	pInfo->lowmidbandgain = MisaProgressbar_GetPercent(hEqItems[EQ_LOWMIDBANDGAIN]);
	pInfo->lowmidbandfreq = MisaProgressbar_GetPercent(hEqItems[EQ_LOWMIDBANDFREQ]);
	pInfo->highmidbandgain= MisaProgressbar_GetPercent(hEqItems[EQ_HIGHMIDBANDGAIN]);
	pInfo->highmidbandfreq = MisaProgressbar_GetPercent(hEqItems[EQ_HIGHMIDBANDFREQ]);
	pInfo->highmidbandQ = MisaProgressbar_GetPercent(hEqItems[EQ_HIGHMIDBANDQ]);
	pInfo->highbandgain = MisaProgressbar_GetPercent(hEqItems[EQ_HIGHBANDGAIN]);
	pInfo->highbandfreq = MisaProgressbar_GetPercent(hEqItems[EQ_HIGHBANDFREQ]);
	pInfo->On = MisaCheckbox_GetStatus(hCOMPcomm.hCheckOn);
#endif
	return 0;
}

static U8 EqCreateItems(WM_HWIN hParent)
{
	int x,y;
	hCOMPcomm = CreateCommComponentEx(hParent,COMMON_ITEM_CLOSE|COMMON_ITEM_CHECK);
	memset(hEqItems,0,sizeof(hEqItems));
	x=2;
	y=500;
	x+=bmFX1_NORMAL.XSize;
	x+=bmFX2_NORMAL.XSize;
	x-=15+100;
	hEqItems[EQ_TOMIXERSCREEN] = BUTTON_CreateAsChild(x,y,bmTOMIXER_NORMAL.XSize,bmTOMIXER_NORMAL.YSize,hParent,EQ_TOMIXERSCREEN_ID,WM_CF_SHOW|WM_CF_MEMDEV);
	BUTTON_SetFocussable(hEqItems[EQ_TOMIXERSCREEN],0);
	BUTTON_SetBitmap(hEqItems[EQ_TOMIXERSCREEN],BUTTON_CI_UNPRESSED,&bmTOMIXER_NORMAL);
	BUTTON_SetBitmap(hEqItems[EQ_TOMIXERSCREEN],BUTTON_CI_PRESSED,&bmTOMIXER_PRESSED);

	y = bmEMPTYTITLEBAR.YSize;
	//y += bmDISTYPEAREA.YSize;
	y += EQ_YOFFSET;
	x = 50;
	hEqItems[EQ_INDICATOR] = CreateSlidePanel(x,y,WM_GetWindowSizeX(hParent)-100,bmSLIDER.YSize+bmDISTYPEAREA.YSize,hParent,WM_CF_SHOW|WM_CF_MEMDEV);
	return 0;
}

static U8 EqDeleteItems()
{
	if(hEqItems[EQ_TOMIXERSCREEN])
	{
		BUTTON_Delete(hEqItems[EQ_TOMIXERSCREEN]);
	}
	if(hEqItems[EQ_INDICATOR])
	{
		DeleteSlidePanel(hEqItems[EQ_INDICATOR]);
	}
	DeleteCommComponent(hCOMPcomm);
	return 0;
}

static void EqProc(WM_MESSAGE* pMsg)
{
	int x, y;
	int NCode,Id;
#ifdef MISA_SMALLPROGRESSBARLAYOUT
	GUI_PID_STATE* pPID_State;
	static U8 sidebutton = 0;
#endif
	ProcessCommComponent(pMsg, &hCOMPcomm);
	switch (pMsg->MsgId)
	{
	case WM_CREATE:
		EqCreateItems(pMsg->hWin);
		break;
	case WM_DELETE:
		EqDeleteItems();
		break;
	case WM_PAINT:
		GUI_SetColor(GUI_BLACK);
		GUI_Clear();
		GUI_SetColor(GUI_WHITE);
		GUI_SetTextMode(GUI_TEXTMODE_TRANS);
		GUI_DrawBitmap(&bmEMPTYTITLEBAR, 0, 0);
		GUI_SetFont(&GUI_Font32B_ASCII);
		x = WM_GetWindowSizeX(pMsg->hWin);
		GUI_DispStringHCenterAt("EQ", x / 2, 5);

#ifdef MISA_SMALLPROGRESSBARLAYOUT
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
#endif
		// Draw slide indicator
		y = WM_GetWindowOrgY(hEqItems[EQ_SLIDEWINDOW]);
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
				DEBUG_STDOUT("EQ button clicked!");
				break;
			case WM_NOTIFICATION_RELEASED:
				switch(Id)
				{
				case COMMON_ID_CHECKON:
					EqOn(MisaCheckbox_GetStatus(hCOMPcomm.hCheckOn));
					break;
				case EQ_TOMIXERSCREEN_ID:
					TopMixerScreen(pMsg->hWin);
				case COMMON_ID_CLOSE:
					UpdateSynthSettingEx(MISAMODULE_EQ);
					break;
				case CTL_NONE_ID:
				case CTL_TOUCH_X_ID:
				case CTL_TOUCH_Y_ID:
				case CTL_DRAG_X_ID:
				case CTL_DRAG_Y_ID:
				case CTL_BALL_X_ID:
				case CTL_BALL_Y_ID:
				case CTL_VARIATION_ID:
					EqControlMenuProc(Id);
					break;
				default:
					;
				}
				DEBUG_STDOUT("EQ button released!");
				break;
			}
		}
		break;
	case WM_TOUCH:
#ifdef MISA_SMALLPROGRESSBARLAYOUT
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
						EqSlideGoPreviousPage();
					}
					else if(GUI_RectsIntersect(&rightsidebutton,&ptRect))
					{
						sidebutton = 2;
						WM_InvalidateRect(pMsg->hWin, &rightsidebutton);
						EqSlideGoNextPage();
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
		else
		{
		}
#endif
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
#ifdef MISA_SMALLPROGRESSBARLAYOUT
	hEqItems[EQ_SLIDEWINDOW] = WM_CreateWindowAsChild(0,0,1400,WM_GetWindowSizeY(hParent),hParent,WM_CF_SHOW|WM_CF_MEMDEV,SlideWindowProc,0);
#else
	hEqItems[EQ_SLIDEWINDOW] = WM_CreateWindowAsChild(0,0,1500,WM_GetWindowSizeY(hParent),hParent,WM_CF_SHOW|WM_CF_MEMDEV,SlideWindowProc,0);
#endif
	return 0;
}

static U8 IndicatorDeleteItems()
{
	if(hEqItems[EQ_SLIDEWINDOW])
	{
		WM_DeleteWindow(hEqItems[EQ_SLIDEWINDOW]);
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

#ifdef MISA_SMALLPROGRESSBARLAYOUT

static void EqLowGainInfo(int value,char* buf)
{
	if(127==value)
	{
		strcpy(buf,"Low Gain: +12.000dB");
	}
	else if(64==value)
	{
		strcpy(buf,"Low Gain: 0.000dB");
	}
	else
	{
		float val = value*0.189-12;
		sprintf(buf,"Low Gain: %+.3fdB",val);
	}
}

static void EqLowMidGainInfo(int value,char* buf)
{
	if(127==value)
	{
		strcpy(buf,"Low-mid Gain: +12.000dB");
	}
	else if(64==value)
	{
		strcpy(buf,"Low-mid Gain: 0.000dB");
	}
	else
	{
		float val = value*0.189-12;
		sprintf(buf,"Low-mid Gain: %+.3fdB",val);
	}
}

static void EqMidHighGainInfo(int value,char* buf)
{
	if(127==value)
	{
		strcpy(buf,"Mid-high Gain: +12.000dB");
	}
	else if(64==value)
	{
		strcpy(buf,"Mid-high Gain: 0.000dB");
	}
	else
	{
		float val = value*0.189-12;
		sprintf(buf,"Mid-high Gain: %+.3fdB",val);
	}
}

static void EqHighGainInfo(int value,char* buf)
{
	if(127==value)
	{
		strcpy(buf,"High Gain: +12.000dB");
	}
	else if(64==value)
	{
		strcpy(buf,"High Gain: 0.000dB");
	}
	else
	{
		float val = value*0.189-12;
		sprintf(buf,"High Gain: %+.3fdB",val);
	}
}

static void EqLowFreqInfo(int value,char* buf)
{
	if(127==value)
	{
		strcpy(buf,"Low Freq: 800Hz");
	}
	else if(64<value)
	{
		sprintf(buf,"Low Freq: %dHz",(int)((value-64)*6.35)+400);
	}
	else
	{
		sprintf(buf,"Low Freq: %dHz",(int)(value*6.25));
	}
}

static void EqLowMidFreqInfo(int value,char* buf)
{
	float val;
	val = value*0.02;
	sprintf(buf,"Low-mid Freq: %.3fkHz",val);
}

static void EqHighMidFreqInfo(int value,char* buf)
{
	float val;
	if(127==value)
	{
		strcpy(buf,"Mid-high Freq: 10.700kHz");
	}
	else if(64<value)
	{
		val = (value-64)*88.88/1000+5.1;
		sprintf(buf,"Mid-high Freq: %.3fkHz",val);
	}
	else
	{
		val = value*78.75/1000+0.06;
		sprintf(buf,"Mid-high Freq: %.3fkHz",val);
	}
}

static void EqHighFreqInfo(int value,char* buf)
{
	float val;
	if(64<value)
	{
		val = (value-64)*38.1/1000+3.4;
		sprintf(buf,"High Freq: %.3fkHz",val);
	}
	else
	{
		val = value*37.5/1000+1;
		sprintf(buf,"High Freq: %.3fkHz",val);
	}
}

#endif // MISA_SMALLPROGRESSBARLAYOUT

static void EqGainInfo(int value,char* buf)
{
	if(127==value)
	{
		strcpy(buf,"+12.000dB");
	}
	else if(64==value)
	{
		strcpy(buf,"0.000dB");
	}
	else
	{
		float val = value*0.189-12;
		sprintf(buf,"%+.3fdB",val);
	}
}

static void EqLowBandFreqInfo(int value,char* buf)
{
	if(127==value)
	{
		strcpy(buf,"800Hz");
	}
	else if(64<value)
	{
		sprintf(buf,"%dHz",(int)((value-64)*6.35)+400);
	}
	else
	{
		sprintf(buf,"%dHz",(int)(value*6.25));
	}
}

static void EqLowMidBandFreqInfo(int value,char* buf)
{
	float val;
	/*if(127==value)
	{
		strcpy(buf,"2.500kHz");
	}
	else if(64<value)
	{
		val = (value-64)*88.88/1000+5.1;
		sprintf(buf,"%.3fkHz",val);
	}
	else
	{
		val = value*78.75/1000;
		sprintf(buf,"%.3fkHz",val);
	}*/
	val = value*0.02;
	sprintf(buf,"%.3fkHz",val);
}

static void EqHighMidBandFreqInfo(int value,char* buf)
{
	float val;
	if(127==value)
	{
		strcpy(buf,"10.700kHz");
	}
	else if(64<value)
	{
		val = (value-64)*88.88/1000+5.1;
		sprintf(buf,"%.3fkHz",val);
	}
	else
	{
		val = value*78.75/1000+0.06;
		sprintf(buf,"%.3fkHz",val);
	}
}

static void EqHighBandFreqInfo(int value,char* buf)
{
	float val;
	if(64<value)
	{
		val = (value-64)*38.1/1000+3.4;
		sprintf(buf,"%.3fkHz",val);
	}
	else
	{
		val = value*37.5/1000+1;
		sprintf(buf,"%.3fkHz",val);
	}
}

static U8 SlideCreateItems(WM_HWIN hParent)
{
#ifdef MISA_SMALLPROGRESSBARLAYOUT
	int x,y;
	x = 0;
	y = 20;
	hEqItems[EQ_LOWBANDGAIN] = MisaSmallProgressCombo_CreateEx(x,y,0,0,hParent,WM_CF_SHOW|WM_CF_MEMDEV,EQ_LOWBANDGAIN_ID,"Low Gain",EqLowGainInfo);
	y += 100;
	hEqItems[EQ_LOWBANDFREQ] = MisaSmallProgressCombo_CreateEx(x,y,0,0,hParent,WM_CF_SHOW|WM_CF_MEMDEV,EQ_LOWBANDFREQ_ID,"Low Freq",EqLowFreqInfo);
	y += 100;
	hEqItems[EQ_LOWMIDBANDGAIN] = MisaSmallProgressCombo_CreateEx(x,y,0,0,hParent,WM_CF_SHOW|WM_CF_MEMDEV,EQ_LOWMIDBANDGAIN_ID,"Low-mid Gain",EqLowMidGainInfo);
	y += 100;
	hEqItems[EQ_LOWMIDBANDFREQ] = MisaSmallProgressCombo_CreateEx(x,y,0,0,hParent,WM_CF_SHOW|WM_CF_MEMDEV,EQ_LOWMIDBANDFREQ_ID,"Low-mid Freq",EqLowMidFreqInfo);
	x = 350;
	y = 120;
	hEqItems[EQ_LOWMIDBANDQ] = MisaSmallProgressCombo_CreateEx(x,y,0,0,hParent,WM_CF_SHOW|WM_CF_MEMDEV,EQ_LOWMIDBANDQ_ID,"Low-mid Q",0);

	x = 700;
	y = 20;
	hEqItems[EQ_HIGHMIDBANDGAIN] = MisaSmallProgressCombo_CreateEx(x,y,0,0,hParent,WM_CF_SHOW|WM_CF_MEMDEV,EQ_HIGHMIDBANDGAIN_ID,"Mid-high Gain",EqMidHighGainInfo);
	y += 100;
	hEqItems[EQ_HIGHMIDBANDFREQ] = MisaSmallProgressCombo_CreateEx(x,y,0,0,hParent,WM_CF_SHOW|WM_CF_MEMDEV,EQ_HIGHMIDBANDFREQ_ID,"Mid-high Freq",EqHighMidFreqInfo);
	y += 100;
	hEqItems[EQ_HIGHBANDGAIN] = MisaSmallProgressCombo_CreateEx(x,y,0,0,hParent,WM_CF_SHOW|WM_CF_MEMDEV,EQ_HIGHBANDGAIN_ID,"High Gain",EqHighGainInfo);
	y += 100;
	hEqItems[EQ_HIGHBANDFREQ] = MisaSmallProgressCombo_CreateEx(x,y,0,0,hParent,WM_CF_SHOW|WM_CF_MEMDEV,EQ_HIGHBANDFREQ_ID,"High Freq",EqHighFreqInfo);
	x = 1050;
	y = 120;
	hEqItems[EQ_HIGHMIDBANDQ] = MisaSmallProgressCombo_CreateEx(x,y,0,0,hParent,WM_CF_SHOW|WM_CF_MEMDEV,EQ_HIGHMIDBANDQ_ID,"Mid-high Q",0);
#else
	int x,y;
	x = 0;
	y = 0;
	y += bmDISTYPEAREA.YSize;
	hEqItems[EQ_LOWBANDGAIN] = MisaProgressbar_CreateEx(x,y,0,0,hParent,EQ_LOWBANDGAIN_ID,"Gain",0,MISAPROG_FONT24,EqGainInfo);
	x += bmSLIDER.XSize;
	//x += 100;
	hEqItems[EQ_LOWBANDFREQ] = MisaProgressbar_CreateEx(x,y,0,0,hParent,EQ_LOWBANDFREQ_ID,"Freq",0,MISAPROG_FONT24,EqLowBandFreqInfo);
	x += bmSLIDER.XSize;
	x += 100;
	hEqItems[EQ_LOWMIDBANDQ] = MisaProgressbar_CreateEx(x,y,0,0,hParent,EQ_LOWMIDBANDQ_ID,"Q",0,MISAPROG_FONT24,0);
	x += bmSLIDER.XSize;
	x += 100;
	hEqItems[EQ_LOWMIDBANDGAIN] = MisaProgressbar_CreateEx(x,y,0,0,hParent,EQ_LOWMIDBANDGAIN_ID,"Gain",0,MISAPROG_FONT24,EqGainInfo);
	x += bmSLIDER.XSize;
	//x += 100;
	hEqItems[EQ_LOWMIDBANDFREQ] = MisaProgressbar_CreateEx(x,y,0,0,hParent,EQ_LOWMIDBANDFREQ_ID,"Freq",0,MISAPROG_FONT24,EqLowMidBandFreqInfo);
	x += bmSLIDER.XSize;
	x += 100;
	hEqItems[EQ_HIGHMIDBANDGAIN] = MisaProgressbar_CreateEx(x,y,0,0,hParent,EQ_HIGHMIDBANDGAIN_ID,"Gain",0,MISAPROG_FONT24,EqGainInfo);
	x += bmSLIDER.XSize;
	//x += 100;
	hEqItems[EQ_HIGHMIDBANDFREQ] = MisaProgressbar_CreateEx(x,y,0,0,hParent,EQ_HIGHMIDBANDFREQ_ID,"Freq",0,MISAPROG_FONT24,EqHighMidBandFreqInfo);
	x += bmSLIDER.XSize;
	x += 100;
	hEqItems[EQ_HIGHMIDBANDQ] = MisaProgressbar_CreateEx(x,y,0,0,hParent,EQ_HIGHMIDBANDQ_ID,"Q",0,MISAPROG_FONT24,0);
	x += bmSLIDER.XSize;
	x += 100;
	hEqItems[EQ_HIGHBANDGAIN] = MisaProgressbar_CreateEx(x,y,0,0,hParent,EQ_HIGHBANDGAIN_ID,"Gain",0,MISAPROG_FONT24,EqGainInfo);
	x += bmSLIDER.XSize;
	//x += 100;
	hEqItems[EQ_HIGHBANDFREQ] = MisaProgressbar_CreateEx(x,y,0,0,hParent,EQ_HIGHBANDFREQ_ID,"Freq",0,MISAPROG_FONT24,EqHighBandFreqInfo);
#endif
	return 0;
}

static U8 SlideDeleteItems()
{
#ifdef MISA_SMALLPROGRESSBARLAYOUT
	if(hEqItems[EQ_LOWBANDGAIN])
	{
		MisaSmallProgressCombo_Delete(hEqItems[EQ_LOWBANDGAIN]);
	}
	if(hEqItems[EQ_LOWBANDFREQ])
	{
		MisaSmallProgressCombo_Delete(hEqItems[EQ_LOWBANDFREQ]);
	}
	if(hEqItems[EQ_LOWMIDBANDQ])
	{
		MisaSmallProgressCombo_Delete(hEqItems[EQ_LOWMIDBANDQ]);
	}
	if(hEqItems[EQ_LOWMIDBANDGAIN])
	{
		MisaSmallProgressCombo_Delete(hEqItems[EQ_LOWMIDBANDGAIN]);
	}
	if(hEqItems[EQ_LOWMIDBANDFREQ])
	{
		MisaSmallProgressCombo_Delete(hEqItems[EQ_LOWMIDBANDFREQ]);
	}
	if(hEqItems[EQ_HIGHMIDBANDGAIN])
	{
		MisaSmallProgressCombo_Delete(hEqItems[EQ_HIGHMIDBANDGAIN]);
	}
	if(hEqItems[EQ_HIGHMIDBANDFREQ])
	{
		MisaSmallProgressCombo_Delete(hEqItems[EQ_HIGHMIDBANDFREQ]);
	}
	if(hEqItems[EQ_HIGHMIDBANDQ])
	{
		MisaSmallProgressCombo_Delete(hEqItems[EQ_HIGHMIDBANDQ]);
	}
	if(hEqItems[EQ_HIGHBANDGAIN])
	{
		MisaSmallProgressCombo_Delete(hEqItems[EQ_HIGHBANDGAIN]);
	}
	if(hEqItems[EQ_HIGHBANDFREQ])
	{
		MisaSmallProgressCombo_Delete(hEqItems[EQ_HIGHBANDFREQ]);
	}
#else
	if(hEqItems[EQ_LOWBANDGAIN])
	{
		MisaProgressbar_Delete(hEqItems[EQ_LOWBANDGAIN]);
	}
	if(hEqItems[EQ_LOWBANDFREQ])
	{
		MisaProgressbar_Delete(hEqItems[EQ_LOWBANDFREQ]);
	}
	if(hEqItems[EQ_LOWMIDBANDQ])
	{
		MisaProgressbar_Delete(hEqItems[EQ_LOWMIDBANDQ]);
	}
	if(hEqItems[EQ_LOWMIDBANDGAIN])
	{
		MisaProgressbar_Delete(hEqItems[EQ_LOWMIDBANDGAIN]);
	}
	if(hEqItems[EQ_LOWMIDBANDFREQ])
	{
		MisaProgressbar_Delete(hEqItems[EQ_LOWMIDBANDFREQ]);
	}
	if(hEqItems[EQ_HIGHMIDBANDGAIN])
	{
		MisaProgressbar_Delete(hEqItems[EQ_HIGHMIDBANDGAIN]);
	}
	if(hEqItems[EQ_HIGHMIDBANDFREQ])
	{
		MisaProgressbar_Delete(hEqItems[EQ_HIGHMIDBANDFREQ]);
	}
	if(hEqItems[EQ_HIGHMIDBANDQ])
	{
		MisaProgressbar_Delete(hEqItems[EQ_HIGHMIDBANDQ]);
	}
	if(hEqItems[EQ_HIGHBANDGAIN])
	{
		MisaProgressbar_Delete(hEqItems[EQ_HIGHBANDGAIN]);
	}
	if(hEqItems[EQ_HIGHBANDFREQ])
	{
		MisaProgressbar_Delete(hEqItems[EQ_HIGHBANDFREQ]);
	}
#endif
	return 0;
}

static void SlideWindowProc(WM_MESSAGE* pMsg)
{
	int x,y;
	int NCode,Id;
	std::vector<int> effect;
#ifndef MISA_SMALLPROGRESSBARLAYOUT
	GUI_PID_STATE* pPID_State;
	static GUI_PID_STATE PID_LastState;
#endif // MISA_SMALLPROGRESSBARLAYOUT
	switch (pMsg->MsgId)
	{
	case WM_CREATE:
		SlideCreateItems(pMsg->hWin);
		break;
	case WM_DELETE:
		SlideDeleteItems();
		break;
	case WM_PAINT:
		GUI_SetColor(GUI_BLACK);
		GUI_Clear();
#ifdef MISA_SMALLPROGRESSBARLAYOUT
#else
		GUI_SetColor(GUI_WHITE);
		GUI_SetFont(&GUI_Font32B_ASCII);
		GUI_DispStringHCenterAt("LOWEST", 100, 30);
		GUI_DispStringHCenterAt("Low Mid Band Q", 350, 30);
		GUI_DispStringHCenterAt("LOWER", 600, 30);
		GUI_DispStringHCenterAt("HIGHER", 900, 30);
		GUI_DispStringHCenterAt("High Mid Band Q", 1150, 30);
		GUI_DispStringHCenterAt("HIGHEST", 1400, 30);
#endif
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
#ifdef MISA_SMALLPROGRESSBARLAYOUT
			x = MisaSmallProgressCombo_GetPercent(pMsg->hWinSrc);
#else
			x = MisaProgressbar_GetPercent(pMsg->hWinSrc);
#endif
			switch(Id)
			{
			case EQ_LOWBANDGAIN_ID:
				EqSetLowestBandGain(x<127?x:127);
				break;
			case EQ_LOWBANDFREQ_ID:
				EqSetLowestBandFrequency(x<127?x:127);
				break;
			case EQ_LOWMIDBANDQ_ID:
				EqSetLowMidQ(x<127?x:127);
				break;
			case EQ_LOWMIDBANDGAIN_ID:
				EqSetLowerBandGain(x<127?x:127);
				break;
			case EQ_LOWMIDBANDFREQ_ID:
				EqSetLowerBandFrequency(x<127?x:127);
				break;
			case EQ_HIGHMIDBANDGAIN_ID:
				EqSetHigherBandGain(x<127?x:127);
				break;
			case EQ_HIGHMIDBANDFREQ_ID:
				EqSetHigherBandFrequency(x<127?x:127);
				break;
			case EQ_HIGHMIDBANDQ_ID:
				EqSetHighMidQ(x<127?x:127);
				break;
			case EQ_HIGHBANDGAIN_ID:
				EqSetHighestBandGain(x<127?x:127);
				break;
			case EQ_HIGHBANDFREQ_ID:
				EqSetHighestBandFrequency(x<127?x:127);
				break;
			default:
				;
			}
			break;
		case WM_NOTIFICATION_SEL_CHANGED:
#ifdef MISA_SMALLPROGRESSBARLAYOUT
			x = WM_GetWindowOrgX(pMsg->hWinSrc);
			y = WM_GetWindowOrgY(pMsg->hWinSrc);
#else
			x = WM_GetWindowOrgX(pMsg->hWinSrc)+50;
			y = WM_GetWindowOrgY(pMsg->hWinSrc)+340;
#endif
			switch(Id)
			{
			case EQ_LOWBANDGAIN_ID:
			case EQ_LOWMIDBANDGAIN_ID:
			case EQ_HIGHMIDBANDGAIN_ID:
			case EQ_HIGHBANDGAIN_ID:
				MisaSetcontrolmenu_PopupEx(hEq,pMsg->hWinSrc,x,y,0,0,EqGainInfo,EqGainInfo);
				break;
			case EQ_LOWBANDFREQ_ID:
				MisaSetcontrolmenu_PopupEx(hEq,pMsg->hWinSrc,x,y,0,0,EqLowBandFreqInfo,EqLowBandFreqInfo);
				break;
			case EQ_LOWMIDBANDFREQ_ID:
				MisaSetcontrolmenu_PopupEx(hEq,pMsg->hWinSrc,x,y,0,0,EqLowMidBandFreqInfo,EqLowMidBandFreqInfo);
				break;
			case EQ_HIGHMIDBANDFREQ_ID:
				MisaSetcontrolmenu_PopupEx(hEq,pMsg->hWinSrc,x,y,0,0,EqHighMidBandFreqInfo,EqHighMidBandFreqInfo);
				break;
			case EQ_HIGHBANDFREQ_ID:
				MisaSetcontrolmenu_PopupEx(hEq,pMsg->hWinSrc,x,y,0,0,EqHighBandFreqInfo,EqHighBandFreqInfo);
				break;
			default:
				MisaSetcontrolmenu_Popup(hEq,pMsg->hWinSrc,x,y);
			}
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
			break;
		}
		break;
	case WM_TOUCH:
#ifndef MISA_SMALLPROGRESSBARLAYOUT
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
					//DisableEqWindow();
					//Misa_SmoothlyMoveWindow(pMsg->hWin,hEq,INDICATORFRAME-x);
					//EnableEqWindow();
					WM_MoveWindow(pMsg->hWin,INDICATORFRAME-x,0);
				}
				else if(GUI_GetScreenSizeX()-INDICATORFRAME-y > x)
				{
					//DisableEqWindow();
					//Misa_SmoothlyMoveWindow(pMsg->hWin,hEq,GUI_GetScreenSizeX()-INDICATORFRAME-y-x);
					//EnableEqWindow();
					WM_MoveWindow(pMsg->hWin,GUI_GetScreenSizeX()-INDICATORFRAME-y-x,0);
				}
#if 0// turn off the auto sliding effect
				else
				{
					#if 1
					if(-INDICATORFRAME<x && x<INDICATORFRAME)
					{
						
						DisableEqWindow();
						Misa_SmoothlyMoveWindow(pMsg->hWin,hEq,INDICATORFRAME-x);
						EnableEqWindow();
					}
					else if(GUI_GetScreenSizeX()-y-INDICATORFRAME<x && x<GUI_GetScreenSizeX()-y+INDICATORFRAME)
					{
						
						DisableEqWindow();
						Misa_SmoothlyMoveWindow(pMsg->hWin,hEq,GUI_GetScreenSizeX()-INDICATORFRAME-y-x);
						EnableEqWindow();
					}
					else if(x<-INDICATORFRAME && (GUI_GetScreenSizeX()-y+INDICATORFRAME)/2<x)
					{
						Misa_SmoothlyMoveWindow(pMsg->hWin,hEq,GUI_GetScreenSizeX()-INDICATORFRAME-y-x);
					}
					else if(x>GUI_GetScreenSizeX()-y+INDICATORFRAME)
					{
						Misa_SmoothlyMoveWindow(pMsg->hWin,hEq,INDICATORFRAME-x);
					}
					#else
					if(x>=-350)
					{
						if(x>-150)
						{
							DisableEqWindow();
							Misa_SmoothlyMoveWindow(pMsg->hWin,hEq,INDICATORFRAME-x);
							EnableEqWindow();
							//WM_MoveWindow(pMsg->hWin,INDICATORFRAME-x,0);
						}
						else
						{
							DisableEqWindow();
							Misa_SmoothlyMoveWindow(pMsg->hWin,hEq,GUI_GetScreenSizeX()-INDICATORFRAME-y-x);
							EnableEqWindow();
							//WM_MoveWindow(pMsg->hWin,GUI_GetScreenSizeX()-INDICATORFRAME-y-x,0);
						}
					}
					else
					{
						if(x>-550)
						{
							DisableEqWindow();
							Misa_SmoothlyMoveWindow(pMsg->hWin,hEq,INDICATORFRAME-x);
							EnableEqWindow();
							//WM_MoveWindow(pMsg->hWin,INDICATORFRAME-x,0);
						}
						else
						{
							DisableEqWindow();
							Misa_SmoothlyMoveWindow(pMsg->hWin,hEq,GUI_GetScreenSizeX()-INDICATORFRAME-y-x);
							EnableEqWindow();
							//WM_MoveWindow(pMsg->hWin,GUI_GetScreenSizeX()-INDICATORFRAME-y-x,0);
						}
					}
					#endif
				}
#endif
				SlidingBorder = GetSlidingBordercheck(pMsg->hWin,hEq);
			}
		}
		else
		{
			PID_LastState.Pressed = 0;
			PID_LastState.x = 0;
			PID_LastState.y = 0;
			WM_ReleaseCapture();
		}
#endif // MISA_SMALLPROGRESSBARLAYOUT
		break;
	default:
		WM_DefaultProc(pMsg);
	}
}

#ifdef MISA_SMALLPROGRESSBARLAYOUT

static void EqSlideGoNextPage()
{
	int x,y;
	x = WM_GetWindowOrgX(hEqItems[EQ_SLIDEWINDOW]);
	y = WM_GetWindowSizeX(hEqItems[EQ_SLIDEWINDOW]);
	WM_MoveWindow(hEqItems[EQ_SLIDEWINDOW],EQ_PAGE_NEGATIVE_FACTOR<GUI_GetScreenSizeX()-INDICATORFRAME-y-x?GUI_GetScreenSizeX()-INDICATORFRAME-y-x:EQ_PAGE_NEGATIVE_FACTOR,0);
	GUI_Delay(10);
	x = WM_GetWindowOrgX(hEqItems[EQ_SLIDEWINDOW]);
	y = WM_GetWindowSizeX(hEqItems[EQ_SLIDEWINDOW]);
	WM_MoveWindow(hEqItems[EQ_SLIDEWINDOW],EQ_PAGE_NEGATIVE_FACTOR<GUI_GetScreenSizeX()-INDICATORFRAME-y-x?GUI_GetScreenSizeX()-INDICATORFRAME-y-x:EQ_PAGE_NEGATIVE_FACTOR,0);
	GUI_Delay(10);
	x = WM_GetWindowOrgX(hEqItems[EQ_SLIDEWINDOW]);
	y = WM_GetWindowSizeX(hEqItems[EQ_SLIDEWINDOW]);
	WM_MoveWindow(hEqItems[EQ_SLIDEWINDOW],EQ_PAGE_NEGATIVE_FACTOR<GUI_GetScreenSizeX()-INDICATORFRAME-y-x?GUI_GetScreenSizeX()-INDICATORFRAME-y-x:EQ_PAGE_NEGATIVE_FACTOR,0);
	GUI_Delay(10);
	x = WM_GetWindowOrgX(hEqItems[EQ_SLIDEWINDOW]);
	y = WM_GetWindowSizeX(hEqItems[EQ_SLIDEWINDOW]);
	WM_MoveWindow(hEqItems[EQ_SLIDEWINDOW],EQ_PAGE_NEGATIVE_FACTOR<GUI_GetScreenSizeX()-INDICATORFRAME-y-x?GUI_GetScreenSizeX()-INDICATORFRAME-y-x:EQ_PAGE_NEGATIVE_FACTOR,0);
	SlidingBorder = GetSlidingBordercheck(hEqItems[EQ_SLIDEWINDOW],hEq);
}

static void EqSlideGoPreviousPage()
{
	int x;
	x = WM_GetWindowOrgX(hEqItems[EQ_SLIDEWINDOW]);
	WM_MoveWindow(hEqItems[EQ_SLIDEWINDOW],EQ_PAGE_POSITIVE_FACTOR>INDICATORFRAME-x?INDICATORFRAME-x:EQ_PAGE_POSITIVE_FACTOR,0);
	GUI_Delay(10);
	x = WM_GetWindowOrgX(hEqItems[EQ_SLIDEWINDOW]);
	WM_MoveWindow(hEqItems[EQ_SLIDEWINDOW],EQ_PAGE_POSITIVE_FACTOR>INDICATORFRAME-x?INDICATORFRAME-x:EQ_PAGE_POSITIVE_FACTOR,0);
	GUI_Delay(10);
	x = WM_GetWindowOrgX(hEqItems[EQ_SLIDEWINDOW]);
	WM_MoveWindow(hEqItems[EQ_SLIDEWINDOW],EQ_PAGE_POSITIVE_FACTOR>INDICATORFRAME-x?INDICATORFRAME-x:EQ_PAGE_POSITIVE_FACTOR,0);
	GUI_Delay(10);
	x = WM_GetWindowOrgX(hEqItems[EQ_SLIDEWINDOW]);
	WM_MoveWindow(hEqItems[EQ_SLIDEWINDOW],EQ_PAGE_POSITIVE_FACTOR>INDICATORFRAME-x?INDICATORFRAME-x:EQ_PAGE_POSITIVE_FACTOR,0);
	SlidingBorder = GetSlidingBordercheck(hEqItems[EQ_SLIDEWINDOW],hEq);
}

static void EqControlMenuProc(int menuId)
{
	AssginEffectControlMenuProc_SPC(menuId,0);
}

#else // MISA_SMALLPROGRESSBARLAYOUT

static void EqControlMenuProc(int menuId)
{
	AssginEffectControlMenuProc(menuId,0);
}

#endif //  MISA_SMALLPROGRESSBARLAYOUT


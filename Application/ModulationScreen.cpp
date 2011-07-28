
#include <cstdlib>
#include <iostream>

#include "GUI.h"
#include "WM.h"
#include "BUTTON.h"

#include "MisaWidget.h"

#include "MisaAppInc.h"

#include "MainMenu.h"
#include "ModulationScreen.h"
#include "ModulationPresets.h"

#include "MisamenuConfig.h"

#include "Resource.h"

///////////////////////////////////////////////////////////////////////////////
//

#define MODULATION_TYPE_XPOS 80
#define MODULATION_TYPE_YPOS 120

#define MODULATION_XOFFSET 2
#define MODULATION_YOFFSET 2


// No more sliding
#define PAGE_POSITIVE_FACTOR 175
#define PAGE_NEGATIVE_FACTOR -175

static const GUI_RECT leftsidebutton=
{
	0,64,39,498
};
static const GUI_RECT rightsidebutton=
{
	760,64,799,498
};

static void SlideGoNextPage();
static void SlideGoPreviousPage();

///////////////////////////////////////////////////////////////////////////////
//

typedef enum __MODULATIONITEMS
{
	
	MODULATION_INDICATOR,
	MODULATION_SLIDEWINDOW,
	MODULATION_LEFTARROW,
	MODULATION_RIGHTARROW,
	MODULATION_PROGMODRATE,
	MODULATION_PROGMODDEPTH,
	MODULATION_PROGLEVEL,
	MODULATION_PROGFDBK,
	MODULATION_PROGFDBKFILTER,
	MODULATION_PROGSHAPE,
	MODULATION_PROGTIME,
	MODULATION_MAX
} MODULATIONITEMS;

///////////////////////////////////////////////////////////////////////////////
//

static WM_HWIN hModulation;
static void ModulationProc(WM_MESSAGE* pMsg);
static WM_HWIN hModulationItems[MODULATION_MAX];
static COMMCOMPONENT hCOMPcomm;

static WM_HWIN CreateSlidePanel(int x0, int y0, int width, int height, WM_HWIN hParent, U16 Style);
static U8 DeleteSlidePanel(WM_HWIN hSlidePanel);
static U8 SlidingBorder;
static U8 ShowArrow;
static int ModulationGetFX();
static void ModulationControlMenuProc(int menuId);

static const GUI_RECT rectModType=
{
	250,
	62,
	550,
	130
};
static int modIndex;

///////////////////////////////////////////////////////////////////////////////
//

U8 CreateModulationScreen(void* pPara)
{
	int x,y;
	x=GUI_GetScreenSizeX();
	y=GUI_GetScreenSizeY();
	SlidingBorder = 1;
	ShowArrow = 1;
	modIndex = 0;
	hModulation = WM_CreateWindow(0, 0, x, y, /*WM_CF_SHOW | */WM_CF_MEMDEV, ModulationProc, 0);
	return 0;
}

U8 DeleteModulationScreen()
{
	WM_DeleteWindow(hModulation);
	return 0;
}

U8 TopModulationScreen(WM_HWIN hPreWin)
{
	WM_HideWindow(hPreWin);
	WM_ShowWindow(hModulation);
	//WM_BringToTop(hModulation);
	SetWindowToUpdateIfPresetModified(hModulation);
	return 0;
}

static U8 ModulationAdjustItems(int index)
{
	int x,y,width,height;
	x = WM_GetWindowOrgX(hModulationItems[MODULATION_SLIDEWINDOW]);
	y = WM_GetWindowOrgY(hModulationItems[MODULATION_SLIDEWINDOW]);
	width = WM_GetWindowSizeX(hModulationItems[MODULATION_PROGMODRATE]);
	height = WM_GetWindowSizeY(hModulationItems[MODULATION_SLIDEWINDOW]);
	if(19<=modIndex && 22>=modIndex)
	{
		if(WM_IsVisible(hModulationItems[MODULATION_PROGLEVEL]))
		{
			WM_HideWindow(hModulationItems[MODULATION_PROGLEVEL]);
		}
		if(WM_IsVisible(hModulationItems[MODULATION_PROGFDBK]))
		{
			WM_HideWindow(hModulationItems[MODULATION_PROGFDBK]);
		}
		if(WM_IsVisible(hModulationItems[MODULATION_PROGFDBKFILTER]))
		{
			WM_HideWindow(hModulationItems[MODULATION_PROGFDBKFILTER]);
		}
		if(WM_IsVisible(hModulationItems[MODULATION_PROGTIME]))
		{
			WM_HideWindow(hModulationItems[MODULATION_PROGTIME]);
		}
		if(!WM_IsVisible(hModulationItems[MODULATION_PROGSHAPE]))
		{
			WM_ShowWindow(hModulationItems[MODULATION_PROGSHAPE]);			
		}
		WM_SetWindowPos(hModulationItems[MODULATION_PROGMODRATE], x+100, y, width, height);
		WM_SetWindowPos(hModulationItems[MODULATION_PROGMODDEPTH], x+300, y, width, height);
		WM_SetWindowPos(hModulationItems[MODULATION_PROGSHAPE], x+500, y, width, height);
		WM_SetSize(hModulationItems[MODULATION_SLIDEWINDOW],700,height);
		y = WM_GetWindowSizeX(hModulationItems[MODULATION_SLIDEWINDOW]);
		if(INDICATORFRAME < x)
		{
			WM_MoveWindow(hModulationItems[MODULATION_SLIDEWINDOW],INDICATORFRAME-x,0);
		}
		else if(GUI_GetScreenSizeX()-INDICATORFRAME-y > x)
		{
			WM_MoveWindow(hModulationItems[MODULATION_SLIDEWINDOW],GUI_GetScreenSizeX()-INDICATORFRAME-y-x,0);
		}
		SlidingBorder = 0;
		ShowArrow = 0;
		WM_InvalidateWindow(hModulation);
	}
	else if(15<=modIndex && 18>=modIndex)
	{
		if(WM_IsVisible(hModulationItems[MODULATION_PROGLEVEL]))
		{
			WM_HideWindow(hModulationItems[MODULATION_PROGLEVEL]);
		}
		if(WM_IsVisible(hModulationItems[MODULATION_PROGFDBKFILTER]))
		{
			WM_HideWindow(hModulationItems[MODULATION_PROGFDBKFILTER]);
		}
		if(WM_IsVisible(hModulationItems[MODULATION_PROGTIME]))
		{
			WM_HideWindow(hModulationItems[MODULATION_PROGTIME]);
		}
		if(WM_IsVisible(hModulationItems[MODULATION_PROGSHAPE]))
		{
			WM_HideWindow(hModulationItems[MODULATION_PROGSHAPE]);			
		}
		if(!WM_IsVisible(hModulationItems[MODULATION_PROGFDBK]))
		{
			WM_ShowWindow(hModulationItems[MODULATION_PROGFDBK]);
		}
		WM_SetWindowPos(hModulationItems[MODULATION_PROGMODRATE], x+100, y, width, height);
		WM_SetWindowPos(hModulationItems[MODULATION_PROGMODDEPTH], x+300, y, width, height);
		WM_SetWindowPos(hModulationItems[MODULATION_PROGFDBK], x+500, y, width, height);
		WM_SetSize(hModulationItems[MODULATION_SLIDEWINDOW],700,height);
		y = WM_GetWindowSizeX(hModulationItems[MODULATION_SLIDEWINDOW]);
		if(INDICATORFRAME < x)
		{
			WM_MoveWindow(hModulationItems[MODULATION_SLIDEWINDOW],INDICATORFRAME-x,0);
		}
		else if(GUI_GetScreenSizeX()-INDICATORFRAME-y > x)
		{
			WM_MoveWindow(hModulationItems[MODULATION_SLIDEWINDOW],GUI_GetScreenSizeX()-INDICATORFRAME-y-x,0);
		}
		SlidingBorder = 0;
		ShowArrow = 0;
		WM_InvalidateWindow(hModulation);
	}
	else
	{
		if(!WM_IsVisible(hModulationItems[MODULATION_PROGLEVEL]))
		{
			WM_ShowWindow(hModulationItems[MODULATION_PROGLEVEL]);
		}
		if(!WM_IsVisible(hModulationItems[MODULATION_PROGFDBK]))
		{
			WM_ShowWindow(hModulationItems[MODULATION_PROGFDBK]);
		}
		if(!WM_IsVisible(hModulationItems[MODULATION_PROGFDBKFILTER]))
		{
			WM_ShowWindow(hModulationItems[MODULATION_PROGFDBKFILTER]);
		}
		if(!WM_IsVisible(hModulationItems[MODULATION_PROGTIME]))
		{
			WM_ShowWindow(hModulationItems[MODULATION_PROGTIME]);
		}
		if(WM_IsVisible(hModulationItems[MODULATION_PROGSHAPE]))
		{
			WM_HideWindow(hModulationItems[MODULATION_PROGSHAPE]);
		}
		WM_SetWindowPos(hModulationItems[MODULATION_PROGMODRATE], x, y, width, height);
		WM_SetWindowPos(hModulationItems[MODULATION_PROGMODDEPTH], x+200, y, width, height);
		WM_SetWindowPos(hModulationItems[MODULATION_PROGFDBK], x+600, y, width, height);
		WM_SetWindowPos(hModulationItems[MODULATION_PROGTIME], x+1000, y, width, height);
		WM_SetSize(hModulationItems[MODULATION_SLIDEWINDOW],1100,height);
		y = WM_GetWindowSizeX(hModulationItems[MODULATION_SLIDEWINDOW]);
		if(INDICATORFRAME < x)
		{
			WM_MoveWindow(hModulationItems[MODULATION_SLIDEWINDOW],INDICATORFRAME-x,0);
		}
		else if(GUI_GetScreenSizeX()-INDICATORFRAME-y > x)
		{
			WM_MoveWindow(hModulationItems[MODULATION_SLIDEWINDOW],GUI_GetScreenSizeX()-INDICATORFRAME-y-x,0);
		}
		SlidingBorder = GetSlidingBordercheck(hModulationItems[MODULATION_SLIDEWINDOW],hModulation);
		ShowArrow = 1;
	}
	return 0;
}

U8 UpdateModulationInfo(PMODULATION_INFO pInfo)
{
	std::vector<int> effect;
	if(pInfo->Fx != ModulationGetFX())
	{
		MisaRadio_SetStatus(hCOMPcomm.hFX1,pInfo->Fx?0:1);
		MisaRadio_SetStatus(hCOMPcomm.hFX2,pInfo->Fx?1:0);
	}
	MisaCheckbox_SetStatus(hCOMPcomm.hCheckOn,pInfo->On);
	// If set modulation preset has been called it would automatically been turned on. so turn it off if 'On' button is not checked.
	//ModulationSetOn(ModulationGetFX(),MisaCheckbox_GetStatus(hCOMPcomm.hCheckOn));
	modIndex = pInfo->type;
	WM_InvalidateRect(hModulation, &rectModType);
	if(modIndex)
	{
		MisaProgressbar_SetPercentEx(hModulationItems[MODULATION_PROGMODRATE],pInfo->mod_rate,0);
		MisaProgressbar_SetPercentEx(hModulationItems[MODULATION_PROGMODDEPTH],pInfo->mod_depth,0);
		MisaProgressbar_SetPercentEx(hModulationItems[MODULATION_PROGLEVEL],pInfo->level,0);
		MisaProgressbar_SetPercentEx(hModulationItems[MODULATION_PROGFDBK],pInfo->fdbk,0);
		MisaProgressbar_SetPercentEx(hModulationItems[MODULATION_PROGFDBKFILTER],pInfo->fdbkfilter,0);
		MisaProgressbar_SetPercentEx(hModulationItems[MODULATION_PROGSHAPE],pInfo->shape,0);
		MisaProgressbar_SetPercentEx(hModulationItems[MODULATION_PROGTIME],pInfo->time,0);
	}
	else
	{
		MisaProgressbar_SetPercentEx(hModulationItems[MODULATION_PROGMODRATE],0,0);
		MisaProgressbar_SetPercentEx(hModulationItems[MODULATION_PROGMODDEPTH],0,0);
		MisaProgressbar_SetPercentEx(hModulationItems[MODULATION_PROGLEVEL],0,0);
		MisaProgressbar_SetPercentEx(hModulationItems[MODULATION_PROGFDBK],0,0);
		MisaProgressbar_SetPercentEx(hModulationItems[MODULATION_PROGFDBKFILTER],0,0);
		MisaProgressbar_SetPercentEx(hModulationItems[MODULATION_PROGSHAPE],0,0);
		MisaProgressbar_SetPercentEx(hModulationItems[MODULATION_PROGTIME],0,0);
	}
	effect = SynthFindEffect(SynthTranslateEffect(MODULATION_ID_PROGMODRATE),ModulationGetFX());
	MisaProgressbar_SetCheck(hModulationItems[MODULATION_PROGMODRATE], effect.size()?1:0);
	effect = SynthFindEffect(SynthTranslateEffect(MODULATION_ID_PROGMODDEPTH),ModulationGetFX());
	MisaProgressbar_SetCheck(hModulationItems[MODULATION_PROGMODDEPTH], effect.size()?1:0);
	effect = SynthFindEffect(SynthTranslateEffect(MODULATION_ID_PROGLEVEL),ModulationGetFX());
	MisaProgressbar_SetCheck(hModulationItems[MODULATION_PROGLEVEL], effect.size()?1:0);
	effect = SynthFindEffect(SynthTranslateEffect(MODULATION_ID_PROGFDBK),ModulationGetFX());
	MisaProgressbar_SetCheck(hModulationItems[MODULATION_PROGFDBK], effect.size()?1:0);
	effect = SynthFindEffect(SynthTranslateEffect(MODULATION_ID_PROGFDBKFILTER),ModulationGetFX());
	MisaProgressbar_SetCheck(hModulationItems[MODULATION_PROGFDBKFILTER], effect.size()?1:0);
	effect = SynthFindEffect(SynthTranslateEffect(MODULATION_ID_PROGSHAPE),ModulationGetFX());
	MisaProgressbar_SetCheck(hModulationItems[MODULATION_PROGSHAPE], effect.size()?1:0);
	effect = SynthFindEffect(SynthTranslateEffect(MODULATION_ID_PROGTIME),ModulationGetFX());
	MisaProgressbar_SetCheck(hModulationItems[MODULATION_PROGTIME], effect.size()?1:0);
	ModulationAdjustItems(modIndex);

	SynthSetDragCentre(SynthTranslateEffect(MODULATION_ID_PROGMODRATE),pInfo->Fx,pInfo->mod_rate);
	SynthSetDragCentre(SynthTranslateEffect(MODULATION_ID_PROGMODDEPTH),pInfo->Fx,pInfo->mod_depth);
	SynthSetDragCentre(SynthTranslateEffect(MODULATION_ID_PROGLEVEL),pInfo->Fx,pInfo->level);
	SynthSetDragCentre(SynthTranslateEffect(MODULATION_ID_PROGFDBK),pInfo->Fx,pInfo->fdbk);
	SynthSetDragCentre(SynthTranslateEffect(MODULATION_ID_PROGFDBKFILTER),pInfo->Fx,pInfo->fdbkfilter);
	SynthSetDragCentre(SynthTranslateEffect(MODULATION_ID_PROGSHAPE),pInfo->Fx,pInfo->shape);
	SynthSetDragCentre(SynthTranslateEffect(MODULATION_ID_PROGTIME),pInfo->Fx,pInfo->time);
	return 0;
}

U8 GetModulationInfo(PMODULATION_INFO pInfo)
{
	pInfo->Fx = ModulationGetFX();
	pInfo->On = MisaCheckbox_GetStatus(hCOMPcomm.hCheckOn);
	pInfo->type = modIndex;
	//WM_InvalidateRect(hModulation, &rectModType);
	if(modIndex)
	{
		pInfo->mod_rate = MisaProgressbar_GetPercent(hModulationItems[MODULATION_PROGMODRATE]);
		pInfo->mod_depth = MisaProgressbar_GetPercent(hModulationItems[MODULATION_PROGMODDEPTH]);
		pInfo->level = MisaProgressbar_GetPercent(hModulationItems[MODULATION_PROGLEVEL]);
		pInfo->fdbk = MisaProgressbar_GetPercent(hModulationItems[MODULATION_PROGFDBK]);
		pInfo->fdbkfilter = MisaProgressbar_GetPercent(hModulationItems[MODULATION_PROGFDBKFILTER]);
		pInfo->shape = MisaProgressbar_GetPercent(hModulationItems[MODULATION_PROGSHAPE]);
		pInfo->time = MisaProgressbar_GetPercent(hModulationItems[MODULATION_PROGTIME]);
	}
	else
	{
		memset(pInfo,0,sizeof(MODULATION_INFO));
	}
	return 0;
}

static U8 ModulationCreateItems(WM_HWIN hParent)
{
	int x,y;
	hCOMPcomm = CreateCommComponent(hParent);
	memset(hModulationItems,0,sizeof(hModulationItems));
	x = 4;
	y = bmEMPTYTITLEBAR.YSize;
	hModulationItems[MODULATION_LEFTARROW] = BUTTON_CreateAsChild(x,y,bmLEFTARROW_UN.XSize,bmLEFTARROW_UN.YSize,hParent,MODULATION_ID_LEFTARROW,WM_CF_SHOW|WM_CF_MEMDEV);
	BUTTON_SetFocussable(hModulationItems[MODULATION_LEFTARROW],0);
	BUTTON_SetBitmap(hModulationItems[MODULATION_LEFTARROW],BUTTON_CI_UNPRESSED,&bmLEFTARROW_UN);
	BUTTON_SetBitmap(hModulationItems[MODULATION_LEFTARROW],BUTTON_CI_PRESSED,&bmLEFTARROW_SE);
	x += bmLEFTARROW_UN.XSize+356;	
	hModulationItems[MODULATION_RIGHTARROW] = BUTTON_CreateAsChild(x,y,bmRIGHTARROW_UN.XSize,bmRIGHTARROW_UN.YSize,hParent,MODULATION_ID_RIGHTARROW,WM_CF_SHOW|WM_CF_MEMDEV);
	BUTTON_SetFocussable(hModulationItems[MODULATION_RIGHTARROW],0);
	BUTTON_SetBitmap(hModulationItems[MODULATION_RIGHTARROW],BUTTON_CI_UNPRESSED,&bmRIGHTARROW_UN);
	BUTTON_SetBitmap(hModulationItems[MODULATION_RIGHTARROW],BUTTON_CI_PRESSED,&bmRIGHTARROW_SE);
	y += bmDISTYPEAREA.YSize;
	y += MODULATION_YOFFSET;
	x = 50;
	hModulationItems[MODULATION_INDICATOR] = CreateSlidePanel(x,y,WM_GetWindowSizeX(hParent)-100,bmSLIDER.YSize,hParent,WM_CF_SHOW|WM_CF_MEMDEV);
	return 0;
}

static U8 ModulationDeleteItems()
{
	if(hModulationItems[MODULATION_LEFTARROW])
	{
		BUTTON_Delete(hModulationItems[MODULATION_LEFTARROW]);
	}
	if(hModulationItems[MODULATION_RIGHTARROW])
	{
		BUTTON_Delete(hModulationItems[MODULATION_RIGHTARROW]);
	}
	if(hModulationItems[MODULATION_INDICATOR])
	{
		DeleteSlidePanel(hModulationItems[MODULATION_INDICATOR]);
	}
	DeleteCommComponent(hCOMPcomm);
	return 0;
}

static int ModulationGetFX()
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

U8 ModulationLoadParameter(int index, U8 update)
{
	MisaProgressbar_SetPercentEx(hModulationItems[MODULATION_PROGMODRATE], pModpresets[index]->rate,update);
	MisaProgressbar_SetPercentEx(hModulationItems[MODULATION_PROGMODDEPTH], pModpresets[index]->depth,update);
	MisaProgressbar_SetPercentEx(hModulationItems[MODULATION_PROGLEVEL], pModpresets[index]->level,update);
	MisaProgressbar_SetPercentEx(hModulationItems[MODULATION_PROGFDBK], pModpresets[index]->Feedback,update);
	MisaProgressbar_SetPercentEx(hModulationItems[MODULATION_PROGFDBKFILTER], pModpresets[index]->HDAMP,update);
	MisaProgressbar_SetPercentEx(hModulationItems[MODULATION_PROGSHAPE], pModpresets[index]->shape,update);
	MisaProgressbar_SetPercentEx(hModulationItems[MODULATION_PROGTIME], pModpresets[index]->delay,update);
	//if(MisaCheckbox_GetStatus(hCOMPcomm.hCheckOn))
	//{
	//	ModulationSetPreset(ModulationGetFX(),index);
	//}
	return 0;
}

static void ModulationProc(WM_MESSAGE* pMsg)
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
		ModulationCreateItems(pMsg->hWin);
		ModulationLoadParameter(modIndex,0);
		break;
	case WM_DELETE:
		ModulationDeleteItems();
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
			std::string title("Modulation");
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

		y += 20;
		GUI_DispStringHCenterAt(pModpresets[modIndex]->name, x / 2, y);
		y = WM_GetWindowOrgY(hModulationItems[MODULATION_SLIDEWINDOW]);
		y += 170;
		if(0x02&SlidingBorder && ShowArrow)
		{
			x = 4;
			GUI_DrawBitmap(&bmLARROW,x,y);
		}
		if(0x01&SlidingBorder && ShowArrow)
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
				DEBUG_STDOUT("MODULATION button clicked!");
				break;
			case WM_NOTIFICATION_RELEASED:
				switch(Id)
				{
				case MODULATION_ID_LEFTARROW:
					--modIndex;
					if(1>modIndex)
					{
						modIndex = sizeof(pModpresets)/4-1;
					}
					WM_InvalidateRect(pMsg->hWin, &rectModType);
					if(MisaCheckbox_GetStatus(hCOMPcomm.hCheckOn))
					{
						ModulationSetPreset(ModulationGetFX(),modIndex);
					}
					ModulationLoadParameter(modIndex,0);
					ModulationAdjustItems(modIndex);
					break;
				case MODULATION_ID_RIGHTARROW:
					++modIndex;
					if(sizeof(pModpresets)/4 <= modIndex)
					{
						modIndex = 1;
					}
					WM_InvalidateRect(pMsg->hWin, &rectModType);
					if(MisaCheckbox_GetStatus(hCOMPcomm.hCheckOn))
					{
						ModulationSetPreset(ModulationGetFX(),modIndex);
					}
					ModulationLoadParameter(modIndex,0);
					ModulationAdjustItems(modIndex);
					break;
				case COMMON_ID_CHECKON:
					if(MisaCheckbox_GetStatus(hCOMPcomm.hCheckOn))
					{
						ModulationSetOn(ModulationGetFX(),1);
						x = MisaProgressbar_GetPercent(hModulationItems[MODULATION_PROGMODRATE]);
						ModulationSetRate(ModulationGetFX(),x<127?x:127);
						x = MisaProgressbar_GetPercent(hModulationItems[MODULATION_PROGMODDEPTH]);
						ModulationSetDepth(ModulationGetFX(),x<127?x:127);
						x = MisaProgressbar_GetPercent(hModulationItems[MODULATION_PROGLEVEL]);
						ModulationSetLevel(ModulationGetFX(),x<127?x:127);
						x = MisaProgressbar_GetPercent(hModulationItems[MODULATION_PROGFDBK]);
						ModulationSetChorusFeedback(ModulationGetFX(),x<127?x:127);
						x = MisaProgressbar_GetPercent(hModulationItems[MODULATION_PROGFDBKFILTER]);
						ModulationSetDelayFeedbackFilter(ModulationGetFX(),x<127?x:127);
						x = MisaProgressbar_GetPercent(hModulationItems[MODULATION_PROGSHAPE]);
						ModulationSetTremolo(ModulationGetFX(),x<127?x:127);
						x = MisaProgressbar_GetPercent(hModulationItems[MODULATION_PROGTIME]);
						ModulationSetChorusDelayTime(ModulationGetFX(),x<127?x:127);
					}
					else
					{
						ModulationSetOn(ModulationGetFX(),0);
					}
					#if 0
					if(MisaCheckbox_GetStatus(hCOMPcomm.hCheckOn))
					{
						ModulationSetPreset(ModulationGetFX(),modIndex);
						x = MisaProgressbar_GetPercent(hModulationItems[MODULATION_PROGMODRATE]);
						ModulationSetRate(ModulationGetFX(),x<127?x:127);
						x = MisaProgressbar_GetPercent(hModulationItems[MODULATION_PROGMODDEPTH]);
						ModulationSetDepth(ModulationGetFX(),x<127?x:127);
						x = MisaProgressbar_GetPercent(hModulationItems[MODULATION_PROGLEVEL]);
						ModulationSetLevel(ModulationGetFX(),x<127?x:127);
						x = MisaProgressbar_GetPercent(hModulationItems[MODULATION_PROGFDBK]);
						ModulationSetChorusFeedback(ModulationGetFX(),x<127?x:127);
						x = MisaProgressbar_GetPercent(hModulationItems[MODULATION_PROGFDBKFILTER]);
						ModulationSetDelayFeedbackFilter(ModulationGetFX(),x<127?x:127);
						x = MisaProgressbar_GetPercent(hModulationItems[MODULATION_PROGSHAPE]);
						ModulationSetTremolo(ModulationGetFX(),x<127?x:127);
						x = MisaProgressbar_GetPercent(hModulationItems[MODULATION_PROGTIME]);
						ModulationSetChorusDelayTime(ModulationGetFX(),x<127?x:127);
					}
					else
					{
						ModulationSetRate(ModulationGetFX(),0);
						ModulationSetDepth(ModulationGetFX(),0);
						ModulationSetLevel(ModulationGetFX(),0);
						ModulationSetChorusFeedback(ModulationGetFX(),0);
						ModulationSetDelayFeedbackFilter(ModulationGetFX(),0);
						ModulationSetTremolo(ModulationGetFX(),0);
						ModulationSetChorusDelayTime(ModulationGetFX(),0);
					}
					#endif
					break;
				case COMMON_ID_FX1:
				case COMMON_ID_FX2:
				case COMMON_ID_CLOSE:
					UpdateSynthSettingEx(MISAMODULE_MODULATION);
					break;
				case CTL_NONE_ID:
				case CTL_TOUCH_X_ID:
				case CTL_TOUCH_Y_ID:
				case CTL_DRAG_X_ID:
				case CTL_DRAG_Y_ID:
				case CTL_BALL_X_ID:
				case CTL_BALL_Y_ID:
				case CTL_VARIATION_ID:
					ModulationControlMenuProc(Id);
					break;
				default:
					;
				}
				DEBUG_STDOUT("MODULATION button released!");
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
						SlideGoPreviousPage();
					}
					else if(GUI_RectsIntersect(&rightsidebutton,&ptRect))
					{
						sidebutton = 2;
						WM_InvalidateRect(pMsg->hWin, &rightsidebutton);
						SlideGoNextPage();
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
	hModulationItems[MODULATION_SLIDEWINDOW] = WM_CreateWindowAsChild(0,0,1300,WM_GetWindowSizeY(hParent),hParent,WM_CF_SHOW|WM_CF_MEMDEV,SlideWindowProc,0);
	return 0;
}

static U8 IndicatorDeleteItems()
{
	if(hModulationItems[MODULATION_SLIDEWINDOW])
	{
		WM_DeleteWindow(hModulationItems[MODULATION_SLIDEWINDOW]);
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
	hModulationItems[MODULATION_PROGMODRATE] = MisaProgressbar_CreateEx(x,y,0,0,hParent,MODULATION_ID_PROGMODRATE,"MOD","RATE",MISAPROG_FONT24,0);
	x += bmSLIDER.XSize;
	x += 100;
	hModulationItems[MODULATION_PROGMODDEPTH] = MisaProgressbar_CreateEx(x,y,0,0,hParent,MODULATION_ID_PROGMODDEPTH,"MOD","DEPTH",MISAPROG_FONT24,0);
	x += bmSLIDER.XSize;
	x += 100;
	hModulationItems[MODULATION_PROGLEVEL] = MisaProgressbar_Create(x,y,0,0,hParent,MODULATION_ID_PROGLEVEL,"LEVEL",MISAPROG_FONT24);
	x += bmSLIDER.XSize;
	x += 100;
	hModulationItems[MODULATION_PROGFDBK] = MisaProgressbar_Create(x,y,0,0,hParent,MODULATION_ID_PROGFDBK,"FDBK",MISAPROG_FONT24);
	x += bmSLIDER.XSize;
	x += 100;
	hModulationItems[MODULATION_PROGFDBKFILTER] = MisaProgressbar_CreateEx(x,y,0,0,hParent,MODULATION_ID_PROGFDBKFILTER,"FDBK","FILTER",MISAPROG_FONT24,0);
	x += bmSLIDER.XSize;
	x += 100;
	hModulationItems[MODULATION_PROGSHAPE] = MisaProgressbar_Create(x,y,0,0,hParent,MODULATION_ID_PROGSHAPE,"SHAPE",MISAPROG_FONT24);
	x += bmSLIDER.XSize;
	x += 100;
	hModulationItems[MODULATION_PROGTIME] = MisaProgressbar_Create(x,y,0,0,hParent,MODULATION_ID_PROGTIME,"TIME",MISAPROG_FONT24);
	x += bmSLIDER.XSize;
	x += 100;
	return 0;
}

static U8 SlideDeleteItems()
{
	if(hModulationItems[MODULATION_PROGMODRATE])
	{
		MisaProgressbar_Delete(hModulationItems[MODULATION_PROGMODRATE]);
	}
	if(hModulationItems[MODULATION_PROGMODDEPTH])
	{
		MisaProgressbar_Delete(hModulationItems[MODULATION_PROGMODDEPTH]);
	}
	if(hModulationItems[MODULATION_PROGLEVEL])
	{
		MisaProgressbar_Delete(hModulationItems[MODULATION_PROGLEVEL]);
	}
	if(hModulationItems[MODULATION_PROGFDBK])
	{
		MisaProgressbar_Delete(hModulationItems[MODULATION_PROGFDBK]);
	}
	if(hModulationItems[MODULATION_PROGFDBKFILTER])
	{
		MisaProgressbar_Delete(hModulationItems[MODULATION_PROGFDBKFILTER]);
	}
	if(hModulationItems[MODULATION_PROGSHAPE])
	{
		MisaProgressbar_Delete(hModulationItems[MODULATION_PROGSHAPE]);
	}
	if(hModulationItems[MODULATION_PROGTIME])
	{
		MisaProgressbar_Delete(hModulationItems[MODULATION_PROGTIME]);
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
		case WM_NOTIFICATION_RELEASED:
			break;
		case WM_NOTIFICATION_VALUE_CHANGED:
			x = MisaProgressbar_GetPercent(pMsg->hWinSrc);
			switch(Id)
			{
			case MODULATION_ID_PROGMODRATE:
				ModulationSetRate(ModulationGetFX(),x<127?x:127);
				break;
			case MODULATION_ID_PROGMODDEPTH:
				ModulationSetDepth(ModulationGetFX(),x<127?x:127);
				break;
			case MODULATION_ID_PROGLEVEL:
				ModulationSetLevel(ModulationGetFX(),x<127?x:127);
				break;
			case MODULATION_ID_PROGFDBK:
				ModulationSetChorusFeedback(ModulationGetFX(),x<127?x:127);
				break;
			case MODULATION_ID_PROGFDBKFILTER:
				ModulationSetDelayFeedbackFilter(ModulationGetFX(),x<127?x:127);
				break;
			case MODULATION_ID_PROGSHAPE:
				ModulationSetTremolo(ModulationGetFX(),x<127?x:127);
				break;
			case MODULATION_ID_PROGTIME:
				ModulationSetChorusDelayTime(ModulationGetFX(),x<127?x:127);
				break;
			}
			break;
		case WM_NOTIFICATION_SEL_CHANGED:
			x = WM_GetWindowOrgX(pMsg->hWinSrc)+50;
			y = WM_GetWindowOrgY(pMsg->hWinSrc)+340;
			MisaSetcontrolmenu_Popup(hModulation,pMsg->hWinSrc,x,y);
			effect = SynthFindEffect(SynthTranslateEffect(Id),ModulationGetFX());
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
				SlidingBorder = GetSlidingBordercheck(pMsg->hWin,hModulation);
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

static void ModulationControlMenuProc(int menuId)
{
	AssginEffectControlMenuProc(menuId,ModulationGetFX());
}

// No more sliding
static void SlideGoNextPage()
{
	int x,y;
	x = WM_GetWindowOrgX(hModulationItems[MODULATION_SLIDEWINDOW]);
	y = WM_GetWindowSizeX(hModulationItems[MODULATION_SLIDEWINDOW]);
	WM_MoveWindow(hModulationItems[MODULATION_SLIDEWINDOW],PAGE_NEGATIVE_FACTOR<GUI_GetScreenSizeX()-INDICATORFRAME-y-x?GUI_GetScreenSizeX()-INDICATORFRAME-y-x:PAGE_NEGATIVE_FACTOR,0);
	GUI_Delay(10);
	x = WM_GetWindowOrgX(hModulationItems[MODULATION_SLIDEWINDOW]);
	y = WM_GetWindowSizeX(hModulationItems[MODULATION_SLIDEWINDOW]);
	WM_MoveWindow(hModulationItems[MODULATION_SLIDEWINDOW],PAGE_NEGATIVE_FACTOR<GUI_GetScreenSizeX()-INDICATORFRAME-y-x?GUI_GetScreenSizeX()-INDICATORFRAME-y-x:PAGE_NEGATIVE_FACTOR,0);
	GUI_Delay(10);
	x = WM_GetWindowOrgX(hModulationItems[MODULATION_SLIDEWINDOW]);
	y = WM_GetWindowSizeX(hModulationItems[MODULATION_SLIDEWINDOW]);
	WM_MoveWindow(hModulationItems[MODULATION_SLIDEWINDOW],PAGE_NEGATIVE_FACTOR<GUI_GetScreenSizeX()-INDICATORFRAME-y-x?GUI_GetScreenSizeX()-INDICATORFRAME-y-x:PAGE_NEGATIVE_FACTOR,0);
	GUI_Delay(10);
	x = WM_GetWindowOrgX(hModulationItems[MODULATION_SLIDEWINDOW]);
	y = WM_GetWindowSizeX(hModulationItems[MODULATION_SLIDEWINDOW]);
	WM_MoveWindow(hModulationItems[MODULATION_SLIDEWINDOW],PAGE_NEGATIVE_FACTOR<GUI_GetScreenSizeX()-INDICATORFRAME-y-x?GUI_GetScreenSizeX()-INDICATORFRAME-y-x:PAGE_NEGATIVE_FACTOR,0);
	SlidingBorder = GetSlidingBordercheck(hModulationItems[MODULATION_SLIDEWINDOW],hModulation);
}

static void SlideGoPreviousPage()
{
	int x;
	x = WM_GetWindowOrgX(hModulationItems[MODULATION_SLIDEWINDOW]);
	WM_MoveWindow(hModulationItems[MODULATION_SLIDEWINDOW],PAGE_POSITIVE_FACTOR>INDICATORFRAME-x?INDICATORFRAME-x:PAGE_POSITIVE_FACTOR,0);
	GUI_Delay(10);
	x = WM_GetWindowOrgX(hModulationItems[MODULATION_SLIDEWINDOW]);
	WM_MoveWindow(hModulationItems[MODULATION_SLIDEWINDOW],PAGE_POSITIVE_FACTOR>INDICATORFRAME-x?INDICATORFRAME-x:PAGE_POSITIVE_FACTOR,0);
	GUI_Delay(10);
	x = WM_GetWindowOrgX(hModulationItems[MODULATION_SLIDEWINDOW]);
	WM_MoveWindow(hModulationItems[MODULATION_SLIDEWINDOW],PAGE_POSITIVE_FACTOR>INDICATORFRAME-x?INDICATORFRAME-x:PAGE_POSITIVE_FACTOR,0);
	GUI_Delay(10);
	x = WM_GetWindowOrgX(hModulationItems[MODULATION_SLIDEWINDOW]);
	WM_MoveWindow(hModulationItems[MODULATION_SLIDEWINDOW],PAGE_POSITIVE_FACTOR>INDICATORFRAME-x?INDICATORFRAME-x:PAGE_POSITIVE_FACTOR,0);
	SlidingBorder = GetSlidingBordercheck(hModulationItems[MODULATION_SLIDEWINDOW],hModulation);
}

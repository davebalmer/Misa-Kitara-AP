
#include <cstdlib>
#include <iostream>

#include "GUI.h"
#include "WM.h"
#include "BUTTON.h"

#include "MisaWidget.h"

#include "MisaAppInc.h"

#include "MainMenu.h"
#include "DistortionScreen.h"

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


///////////////////////////////////////////////////////////////////////////////
//

#define DISTORTION_TYPE_XPOS 80
#define DISTORTION_TYPE_YPOS 120

#define DISTORTION_XOFFSET 2
#define DISTORTION_YOFFSET 2

///////////////////////////////////////////////////////////////////////////////
//

typedef enum __DISTORTIONITEMS
{
	DISTORTION_LEFTARROW,
	DISTORTION_RIGHTARROW,
	DISTORTION_PROGDRIVE,
	DISTORTION_PROGTONE,
	DISTORTION_PROGLEVEL,
	DISTORTION_PROGNOISEREDUCTION,
	DISTORTION_PROGBOOSTER,
	//DISTORTION_POPMENU,
	DISTORTION_MAX
} DISTORTIONITEMS;

///////////////////////////////////////////////////////////////////////////////
//

static WM_HWIN hDistortion;
static void DistortionProc(WM_MESSAGE* pMsg);
static int DistortionGetFX();
static void DistortionControlMenuProc(int menuId);

static WM_HWIN hDistortionItems[DISTORTION_MAX];
static COMMCOMPONENT hCOMPcomm;

static const GUI_RECT rectDisType=
{
	300,
	62,
	500,
	130
};
static int disIndex;
static const char* strDistype[]=
{
	"warm drive",
	"overdrive",
	"cry",
	"distortion",
	"metal",
	"max saturation",
	"fuzz"
};

///////////////////////////////////////////////////////////////////////////////
//

U8 CreateDistortionScreen(void* pPara)
{
	int x,y;
	x=GUI_GetScreenSizeX();
	y=GUI_GetScreenSizeY();
	disIndex = 0;
	hDistortion = WM_CreateWindow(0, 0, x, y, /*WM_CF_SHOW | */WM_CF_MEMDEV, DistortionProc, 0);
	return 0;
}

U8 DeleteDistortionScreen()
{
	WM_DeleteWindow(hDistortion);
	return 0;
}

U8 TopDistortionScreen(WM_HWIN hPreWin)
{
	WM_HideWindow(hPreWin);
	WM_ShowWindow(hDistortion);
	//WM_BringToTop(hDistortion);
	SetWindowToUpdateIfPresetModified(hDistortion);
	return 0;
}

U8 UpdateDistortionInfo(PDISTORTION_INFO pInfo)
{
	std::vector<int> effect;
	if(pInfo->Fx != DistortionGetFX())
	{
		MisaRadio_SetStatus(hCOMPcomm.hFX1,pInfo->Fx?0:1);
		MisaRadio_SetStatus(hCOMPcomm.hFX2,pInfo->Fx?1:0);
	}
	disIndex = pInfo->type%(sizeof(strDistype)/4);
	WM_InvalidateRect(hDistortion, &rectDisType);
	MisaCheckbox_SetStatus(hCOMPcomm.hCheckOn,pInfo->On);
	MisaProgressbar_SetPercentEx(hDistortionItems[DISTORTION_PROGDRIVE],pInfo->drive,0);
	MisaProgressbar_SetPercentEx(hDistortionItems[DISTORTION_PROGTONE],pInfo->tone,0);
	MisaProgressbar_SetPercentEx(hDistortionItems[DISTORTION_PROGLEVEL],pInfo->level,0);
	MisaProgressbar_SetPercentEx(hDistortionItems[DISTORTION_PROGNOISEREDUCTION],pInfo->noisereduction,0);
	MisaProgressbar_SetPercentEx(hDistortionItems[DISTORTION_PROGBOOSTER],pInfo->booster,0);
	effect = SynthFindEffect(SynthTranslateEffect(DISTORTION_ID_PROGDRIVE),DistortionGetFX());
	MisaProgressbar_SetCheck(hDistortionItems[DISTORTION_PROGDRIVE], effect.size()?1:0);
	effect = SynthFindEffect(SynthTranslateEffect(DISTORTION_ID_PROGTONE),DistortionGetFX());
	MisaProgressbar_SetCheck(hDistortionItems[DISTORTION_PROGTONE], effect.size()?1:0);
	effect = SynthFindEffect(SynthTranslateEffect(DISTORTION_ID_PROGLEVEL),DistortionGetFX());
	MisaProgressbar_SetCheck(hDistortionItems[DISTORTION_PROGLEVEL], effect.size()?1:0);

	SynthSetDragCentre(SynthTranslateEffect(DISTORTION_ID_PROGDRIVE),pInfo->Fx,pInfo->drive);
	SynthSetDragCentre(SynthTranslateEffect(DISTORTION_ID_PROGTONE),pInfo->Fx,pInfo->tone);
	SynthSetDragCentre(SynthTranslateEffect(DISTORTION_ID_PROGLEVEL),pInfo->Fx,pInfo->level);
	return 0;
}

U8 GetDistortionInfo(PDISTORTION_INFO pInfo)
{
	pInfo->Fx = DistortionGetFX();
	pInfo->On = MisaCheckbox_GetStatus(hCOMPcomm.hCheckOn);
	pInfo->type = disIndex;
	pInfo->drive = MisaProgressbar_GetPercent(hDistortionItems[DISTORTION_PROGDRIVE]);
	pInfo->tone = MisaProgressbar_GetPercent(hDistortionItems[DISTORTION_PROGTONE]);
	pInfo->level = MisaProgressbar_GetPercent(hDistortionItems[DISTORTION_PROGLEVEL]);
	return 0;
}

static void DistortionNoiseReductionCB(int value, char* buf)
{
	if(0 <= value && 26 > value)
	{
		strcpy(buf,"0");
	}
	else if(26 <= value && 52 > value)
	{
		strcpy(buf,"1");
	}
	else if(52 <= value && 78 > value)
	{
		strcpy(buf,"2");
	}
	else if(78 <= 94 && 110 > value)
	{
		strcpy(buf,"3");
	}
	else
	{
		strcpy(buf,"4");
	}
}

static void DistortionBoosterCB(int value, char* buf)
{
	if(0 <= value && 32 > value)
	{
		strcpy(buf,"0");
	}
	else if(32 <= value && 64 > value)
	{
		strcpy(buf,"1");
	}
	else if(64 <= value && 96 > value)
	{
		strcpy(buf,"2");
	}
	else
	{
		strcpy(buf,"3");
	}
}

static U8 DistortionCreateItems(WM_HWIN hParent)
{
	int x,y;
	//MENU_ITEM_DATA menuItem;
	hCOMPcomm = CreateCommComponent(hParent);
	memset(hDistortionItems,0,sizeof(hDistortionItems));
	x = 4;
	y = bmEMPTYTITLEBAR.YSize;
	hDistortionItems[DISTORTION_LEFTARROW] = BUTTON_CreateAsChild(x,y,bmLEFTARROW_UN.XSize,bmLEFTARROW_UN.YSize,hParent,DISTORTION_ID_LEFTARROW,WM_CF_SHOW|WM_CF_MEMDEV);
	BUTTON_SetFocussable(hDistortionItems[DISTORTION_LEFTARROW],0);
	BUTTON_SetBitmap(hDistortionItems[DISTORTION_LEFTARROW],BUTTON_CI_UNPRESSED,&bmLEFTARROW_UN);
	BUTTON_SetBitmap(hDistortionItems[DISTORTION_LEFTARROW],BUTTON_CI_PRESSED,&bmLEFTARROW_SE);
	x += bmLEFTARROW_UN.XSize+356;	
	hDistortionItems[DISTORTION_RIGHTARROW] = BUTTON_CreateAsChild(x,y,bmRIGHTARROW_UN.XSize,bmRIGHTARROW_UN.YSize,hParent,DISTORTION_ID_RIGHTARROW,WM_CF_SHOW|WM_CF_MEMDEV);
	BUTTON_SetFocussable(hDistortionItems[DISTORTION_RIGHTARROW],0);
	BUTTON_SetBitmap(hDistortionItems[DISTORTION_RIGHTARROW],BUTTON_CI_UNPRESSED,&bmRIGHTARROW_UN);
	BUTTON_SetBitmap(hDistortionItems[DISTORTION_RIGHTARROW],BUTTON_CI_PRESSED,&bmRIGHTARROW_SE);
	y += bmDISTYPEAREA.YSize;
	y += DISTORTION_YOFFSET;
	int xbar = 50;
	hDistortionItems[DISTORTION_PROGDRIVE] = MisaProgressbar_Create(xbar,y,0,0,hParent,DISTORTION_ID_PROGDRIVE,"Drive",MISAPROG_FONT24);
	xbar += (bmSLIDER.XSize + 100);
	hDistortionItems[DISTORTION_PROGTONE] = MisaProgressbar_Create(xbar,y,0,0,hParent,DISTORTION_ID_PROGTONE,"Tone",MISAPROG_FONT24);
	xbar += (bmSLIDER.XSize + 100);
	hDistortionItems[DISTORTION_PROGLEVEL] = MisaProgressbar_Create(xbar,y,0,0,hParent,DISTORTION_ID_PROGLEVEL,"Level",MISAPROG_FONT24);
//MZ - remove noise reduction control as it only works with line in
//	hDistortionItems[DISTORTION_PROGNOISEREDUCTION] = MisaProgressbar_CreateEx(500,y,0,0,hParent,DISTORTION_ID_PROGNOISEREDUCTION,"Noise","Reduct.",MISAPROG_FONT24,DistortionNoiseReductionCB);
//	MisaProgressbar_SetCheck(hDistortionItems[DISTORTION_PROGNOISEREDUCTION], 2);
	hDistortionItems[DISTORTION_PROGBOOSTER] = MisaProgressbar_CreateEx(650,y,0,0,hParent,DISTORTION_ID_PROGBOOSTER,"Booster",0,MISAPROG_FONT24,DistortionBoosterCB);
	MisaProgressbar_SetCheck(hDistortionItems[DISTORTION_PROGBOOSTER], 2);
	/*hDistortionItems[DISTORTION_POPMENU] = MENU_CreateEx(0,0,0,0,hParent,WM_CF_MEMDEV,MENU_CF_VERTICAL,DISTORTION_ID_POPMENU);
	menuItem.pText = "warm drive";
	menuItem.Id = DISTORTION_ID_POPMENU;
	menuItem.Flags = 0;
	menuItem.hSubmenu = 0;
	MENU_AddItem(hDistortionItems[DISTORTION_POPMENU],&menuItem);
	menuItem.pText = "overdrive";
	menuItem.Id = DISTORTION_ID_POPMENU;
	menuItem.Flags = 0;
	menuItem.hSubmenu = 0;
	MENU_AddItem(hDistortionItems[DISTORTION_POPMENU],&menuItem);
	menuItem.pText = "cry";
	menuItem.Id = DISTORTION_ID_POPMENU;
	menuItem.Flags = 0;
	menuItem.hSubmenu = 0;
	MENU_AddItem(hDistortionItems[DISTORTION_POPMENU],&menuItem);
	menuItem.pText = "distortion";
	menuItem.Id = DISTORTION_ID_POPMENU;
	menuItem.Flags = 0;
	menuItem.hSubmenu = 0;
	MENU_AddItem(hDistortionItems[DISTORTION_POPMENU],&menuItem);
	menuItem.pText = "metal";
	menuItem.Id = DISTORTION_ID_POPMENU;
	menuItem.Flags = 0;
	menuItem.hSubmenu = 0;
	MENU_AddItem(hDistortionItems[DISTORTION_POPMENU],&menuItem);
	menuItem.pText = "max saturation";
	menuItem.Id = DISTORTION_ID_POPMENU;
	menuItem.Flags = 0;
	menuItem.hSubmenu = 0;
	MENU_AddItem(hDistortionItems[DISTORTION_POPMENU],&menuItem);
	menuItem.pText = "fuzz";
	menuItem.Id = DISTORTION_ID_POPMENU;
	menuItem.Flags = 0;
	menuItem.hSubmenu = 0;
	MENU_AddItem(hDistortionItems[DISTORTION_POPMENU],&menuItem);*/
	return 0;
}

static U8 DistortionDeleteItems()
{
	if(hDistortionItems[DISTORTION_LEFTARROW])
	{
		BUTTON_Delete(hDistortionItems[DISTORTION_LEFTARROW]);
	}
	if(hDistortionItems[DISTORTION_RIGHTARROW])
	{
		BUTTON_Delete(hDistortionItems[DISTORTION_RIGHTARROW]);
	}
	if(hDistortionItems[DISTORTION_PROGDRIVE])
	{
		MisaProgressbar_Delete(hDistortionItems[DISTORTION_PROGDRIVE]);
	}
	if(hDistortionItems[DISTORTION_PROGTONE])
	{
		MisaProgressbar_Delete(hDistortionItems[DISTORTION_PROGTONE]);
	}
	if(hDistortionItems[DISTORTION_PROGLEVEL])
	{
		MisaProgressbar_Delete(hDistortionItems[DISTORTION_PROGLEVEL]);
	}
	//if(hDistortionItems[DISTORTION_POPMENU])
	//{
	//	MENU_Delete(hDistortionItems[DISTORTION_POPMENU]);
	//}
	DeleteCommComponent(hCOMPcomm);
	return 0;
}

static int DistortionGetFX()
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

static void DistortionProc(WM_MESSAGE* pMsg)
{
	int x, y;
	int NCode,Id;
	ProcessCommComponent(pMsg, &hCOMPcomm);
	switch (pMsg->MsgId)
	{
	case WM_CREATE:
		DistortionCreateItems(pMsg->hWin);
		break;
	case WM_DELETE:
		DistortionDeleteItems();
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
			std::string title("Distortion");
			GUI_DispStringHCenterAt(GetTitleWithPreset(title).c_str(), x / 2, 5);
		}
		y += 20;
		GUI_DispStringHCenterAt(strDistype[disIndex], x / 2, y);
		break;
	case WM_NOTIFY_PARENT:
		if(DispatchCommComponent(pMsg,&hCOMPcomm))
		{
			Id = WM_GetId(pMsg->hWinSrc);
			NCode = pMsg->Data.v;
			switch(NCode)
			{
			case WM_NOTIFICATION_CLICKED:
				DEBUG_STDOUT("Distortion button clicked!");
				break;
			case WM_NOTIFICATION_RELEASED:
				switch(Id)
				{
				case DISTORTION_ID_LEFTARROW:
					--disIndex;
					if(0>disIndex)
					{
						disIndex = sizeof(strDistype)/4-1;
					}
					WM_InvalidateRect(pMsg->hWin, &rectDisType);
					DistortionSetType(DistortionGetFX(),disIndex);
					break;
				case DISTORTION_ID_RIGHTARROW:
					++disIndex;
					if(sizeof(strDistype)/4 <= disIndex)
					{
						disIndex = 0;
					}
					WM_InvalidateRect(pMsg->hWin, &rectDisType);
					DistortionSetType(DistortionGetFX(),disIndex);
					break;
				case COMMON_ID_CHECKON:
					DistortionOn(DistortionGetFX(), MisaCheckbox_GetStatus(hCOMPcomm.hCheckOn)?true:false);
					break;
				case COMMON_ID_FX1:
				case COMMON_ID_FX2:
				case COMMON_ID_CLOSE:
					UpdateSynthSettingEx(MISAMODULE_DISTORTION);
					break;
				case CTL_NONE_ID:
				case CTL_TOUCH_X_ID:
				case CTL_TOUCH_Y_ID:
				case CTL_DRAG_X_ID:
				case CTL_DRAG_Y_ID:
				case CTL_BALL_X_ID:
				case CTL_BALL_Y_ID:
				case CTL_VARIATION_ID:
					DistortionControlMenuProc(Id);
					break;
#if 0
				case DISTORTION_ID_PROGDRIVE:
					x = MisaProgressbar_GetPercent(pMsg->hWinSrc);
					DistortionSetDrive(DistortionGetFX(),x<127?x:127);
					break;
				case DISTORTION_ID_PROGTONE:
					x = MisaProgressbar_GetPercent(pMsg->hWinSrc);
					DistortionSetTone(DistortionGetFX(),x<127?x:127);
					break;
				case DISTORTION_ID_PROGLEVEL:
					x = MisaProgressbar_GetPercent(pMsg->hWinSrc);
					DistortionSetLevel(DistortionGetFX(),x<127?x:127);
					break;
#endif
				default:
					;
				}
				DEBUG_STDOUT("Distortion button released!");
				break;
			case WM_NOTIFICATION_VALUE_CHANGED:
				switch(Id)
				{
				case DISTORTION_ID_PROGDRIVE:
					x = MisaProgressbar_GetPercent(pMsg->hWinSrc);
					DistortionSetDrive(DistortionGetFX(),x<127?x:127);
					break;
				case DISTORTION_ID_PROGTONE:
					x = MisaProgressbar_GetPercent(pMsg->hWinSrc);
					DistortionSetTone(DistortionGetFX(),x<127?x:127);
					break;
				case DISTORTION_ID_PROGLEVEL:
					x = MisaProgressbar_GetPercent(pMsg->hWinSrc);
					DistortionSetLevel(DistortionGetFX(),x<127?x:127);
					break;
				case DISTORTION_ID_PROGNOISEREDUCTION:
					x = MisaProgressbar_GetPercent(pMsg->hWinSrc);
					DistortionNoiseReduction(DistortionGetFX(),x);
					break;
				case DISTORTION_ID_PROGBOOSTER:
					x = MisaProgressbar_GetPercent(pMsg->hWinSrc);
					DistortionSetBooster(DistortionGetFX(),x<127?x/32:3);
					break;
				default:
					;
				}
				break;
			case WM_NOTIFICATION_SEL_CHANGED:
				if(hDistortionItems[DISTORTION_PROGNOISEREDUCTION]!=pMsg->hWinSrc && hDistortionItems[DISTORTION_PROGBOOSTER]!=pMsg->hWinSrc)
				{
					std::vector<int> effect;
					x = WM_GetWindowOrgX(pMsg->hWinSrc)+50;
					y = WM_GetWindowOrgY(pMsg->hWinSrc)+340;
					MisaSetcontrolmenu_Popup(pMsg->hWin,pMsg->hWinSrc,x,y);
					effect = SynthFindEffect(SynthTranslateEffect(Id),DistortionGetFX());
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
			}
		}
		break;
	case WM_TOUCH:
		break;
	default:
		WM_DefaultProc(pMsg);
	}
}

static void DistortionControlMenuProc(int menuId)
{
	AssignEffectControlMenuProc(menuId,DistortionGetFX());
}


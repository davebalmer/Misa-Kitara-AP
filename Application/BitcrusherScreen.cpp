
#include <cstdlib>
#include <iostream>

#include "GUI.h"
#include "WM.h"
#include "BUTTON.h"

#include "MisaWidget.h"

#include "MisaAppInc.h"

#include "MainMenu.h"
#include "BitcrusherScreen.h"

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


///////////////////////////////////////////////////////////////////////////////
//

typedef enum __BITCRUSHERITEMS
{
	BITCRUSHER_BITRESOLUTION,
	BITCRUSHER_BRIGHTNESS,
	BITCRUSHER_DOWNSAMPLINGFACTOR,
	BITCRUSHER_MAX
} BITCRUSHERITEMS;

///////////////////////////////////////////////////////////////////////////////
//

static WM_HWIN hBitCrusher;
static void BitCrusherProc(WM_MESSAGE* pMsg);
static int BitCrusherGetFX();
static void BitCrusherControlMenuProc(int menuId);

static WM_HWIN hBitCrusherItems[BITCRUSHER_MAX];
static COMMCOMPONENT hCOMPcomm;


///////////////////////////////////////////////////////////////////////////////
//

U8 CreateBitCrusherScreen(void* pPara)
{
	int x,y;
	x=GUI_GetScreenSizeX();
	y=GUI_GetScreenSizeY();
	hBitCrusher = WM_CreateWindow(0, 0, x, y, /*WM_CF_SHOW | */WM_CF_MEMDEV, BitCrusherProc, 0);
	return 0;
}

U8 DeleteBitCrusherScreen()
{
	WM_DeleteWindow(hBitCrusher);
	return 0;
}

U8 TopBitCrusherScreen(WM_HWIN hPreWin)
{
	WM_HideWindow(hPreWin);
	WM_ShowWindow(hBitCrusher);
	//WM_BringToTop(hBitCrusher);
	SetWindowToUpdateIfPresetModified(hBitCrusher);
	return 0;
}

U8 UpdateBitCrusherInfo(PBITCRUSHER_INFO pInfo)
{
	std::vector<int> effect;
	if(pInfo->Fx != BitCrusherGetFX())
	{
		MisaRadio_SetStatus(hCOMPcomm.hFX1,pInfo->Fx?0:1);
		MisaRadio_SetStatus(hCOMPcomm.hFX2,pInfo->Fx?1:0);
	}
	MisaCheckbox_SetStatus(hCOMPcomm.hCheckOn,pInfo->On);
	MisaProgressbar_SetPercentEx(hBitCrusherItems[BITCRUSHER_BITRESOLUTION],pInfo->bitresolution,0);
	MisaProgressbar_SetPercentEx(hBitCrusherItems[BITCRUSHER_BRIGHTNESS],pInfo->brightness,0);
	MisaProgressbar_SetPercentEx(hBitCrusherItems[BITCRUSHER_DOWNSAMPLINGFACTOR],pInfo->downsamplingFactor,0);

	effect = SynthFindEffect(SynthTranslateEffect(BITCRUSHER_BITRESOLUTION),BitCrusherGetFX());
	MisaProgressbar_SetCheck(hBitCrusherItems[BITCRUSHER_BITRESOLUTION], effect.size()?1:0);
	effect = SynthFindEffect(SynthTranslateEffect(BITCRUSHER_BRIGHTNESS),BitCrusherGetFX());
	MisaProgressbar_SetCheck(hBitCrusherItems[BITCRUSHER_BRIGHTNESS], effect.size()?1:0);
	effect = SynthFindEffect(SynthTranslateEffect(BITCRUSHER_DOWNSAMPLINGFACTOR),BitCrusherGetFX());
	MisaProgressbar_SetCheck(hBitCrusherItems[BITCRUSHER_DOWNSAMPLINGFACTOR], effect.size()?1:0);

	SynthSetDragCentre(SynthTranslateEffect(BITCRUSHER_BITRESOLUTION),pInfo->Fx,pInfo->bitresolution);
	SynthSetDragCentre(SynthTranslateEffect(BITCRUSHER_BRIGHTNESS),pInfo->Fx,pInfo->brightness);
	SynthSetDragCentre(SynthTranslateEffect(BITCRUSHER_DOWNSAMPLINGFACTOR),pInfo->Fx,pInfo->downsamplingFactor);
	return 0;
}

U8 GetBitCrusherInfo(PBITCRUSHER_INFO pInfo)
{
	pInfo->Fx = BitCrusherGetFX();
	pInfo->On = MisaCheckbox_GetStatus(hCOMPcomm.hCheckOn);
	pInfo->bitresolution = MisaProgressbar_GetPercent(hBitCrusherItems[BITCRUSHER_BITRESOLUTION]);
	pInfo->brightness = MisaProgressbar_GetPercent(hBitCrusherItems[BITCRUSHER_BRIGHTNESS]);
	pInfo->downsamplingFactor = MisaProgressbar_GetPercent(hBitCrusherItems[BITCRUSHER_DOWNSAMPLINGFACTOR]);
	return 0;
}


static void BitResolutionDisplay(int value, char* buf)
{
	sprintf(buf,"%d", value < 127 ? value * 17 / 127 : 16);
}

static void DownsamplingFactorDisplay(int value, char* buf)
{
	sprintf(buf,"%d", (value >> 3) + 1);
}

static U8 BitCrusherCreateItems(WM_HWIN hParent)
{
	int x,y;
	//MENU_ITEM_DATA menuItem;
	hCOMPcomm = CreateCommComponent(hParent);
	memset(hBitCrusherItems,0,sizeof(hBitCrusherItems));
	x = 4;
	y = bmEMPTYTITLEBAR.YSize;

	y += bmDISTYPEAREA.YSize;
	//y += BITCRUSHER_YOFFSET;
	int xbar = 50;
	hBitCrusherItems[BITCRUSHER_BITRESOLUTION] = MisaProgressbar_CreateEx(xbar,y,0,0,hParent,BITCRUSHER_ID_BITRESOLUTION,"Level", "", MISAPROG_FONT24, BitResolutionDisplay);
	xbar += (bmSLIDER.XSize + 100);
	hBitCrusherItems[BITCRUSHER_BRIGHTNESS] = MisaProgressbar_Create(xbar,y,0,0,hParent,BITCRUSHER_ID_BRIGHTNESS,"Fc",MISAPROG_FONT24);
	xbar += (bmSLIDER.XSize + 100);
	hBitCrusherItems[BITCRUSHER_DOWNSAMPLINGFACTOR] = MisaProgressbar_CreateEx(xbar,y,0,0,hParent,BITCRUSHER_ID_DOWNSAMPLINGFACTOR,"Down", "Sampling",MISAPROG_FONT24, DownsamplingFactorDisplay);
	MisaProgressbar_SetCheck(hBitCrusherItems[BITCRUSHER_BITRESOLUTION], 2);		// not assignable to x/y axis
	MisaProgressbar_SetCheck(hBitCrusherItems[BITCRUSHER_DOWNSAMPLINGFACTOR], 2);		// not assignable to x/y axis

	return 0;
}

static U8 BitCrusherDeleteItems()
{
	if(hBitCrusherItems[BITCRUSHER_BITRESOLUTION])
	{
		MisaProgressbar_Delete(hBitCrusherItems[BITCRUSHER_BITRESOLUTION]);
	}
	if(hBitCrusherItems[BITCRUSHER_BRIGHTNESS])
	{
		MisaProgressbar_Delete(hBitCrusherItems[BITCRUSHER_BRIGHTNESS]);
	}
	if(hBitCrusherItems[BITCRUSHER_DOWNSAMPLINGFACTOR])
	{
		MisaProgressbar_Delete(hBitCrusherItems[BITCRUSHER_DOWNSAMPLINGFACTOR]);
	}

	DeleteCommComponent(hCOMPcomm);
	return 0;
}

static int BitCrusherGetFX()
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

static void BitCrusherProc(WM_MESSAGE* pMsg)
{
	int x, y;
	int NCode,Id;
	ProcessCommComponent(pMsg, &hCOMPcomm);
	switch (pMsg->MsgId)
	{
	case WM_CREATE:
		BitCrusherCreateItems(pMsg->hWin);
		break;
	case WM_DELETE:
		BitCrusherDeleteItems();
		break;
	case WM_PAINT:
		GUI_SetBkColor(GUI_BLACK);
		GUI_Clear();
		GUI_SetTextMode(GUI_TEXTMODE_TRANS);
		GUI_DrawBitmap(&bmEMPTYTITLEBAR, 0, 0);
		GUI_SetFont(&GUI_Font32B_ASCII);
		x = WM_GetWindowSizeX(pMsg->hWin);
		{
			std::string title("Bit Crusher");
			GUI_DispStringHCenterAt(GetTitleWithPreset(title).c_str(), x / 2, 5);
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
				DEBUG_STDOUT("BitCrusher button clicked!");
				break;
			case WM_NOTIFICATION_RELEASED:
				switch(Id)
				{
				case COMMON_ID_CHECKON:
					BitCrusherOn(BitCrusherGetFX(), MisaCheckbox_GetStatus(hCOMPcomm.hCheckOn)?true:false);
					break;
				case COMMON_ID_FX1:
				case COMMON_ID_FX2:
				case COMMON_ID_CLOSE:
					UpdateSynthSettingEx(MISAMODULE_BITCRUSHER);
					break;
				case CTL_NONE_ID:
				case CTL_TOUCH_X_ID:
				case CTL_TOUCH_Y_ID:
				case CTL_DRAG_X_ID:
				case CTL_DRAG_Y_ID:
				case CTL_BALL_X_ID:
				case CTL_BALL_Y_ID:
				case CTL_VARIATION_ID:
					BitCrusherControlMenuProc(Id);
					break;

				default:
					;
				}
				DEBUG_STDOUT("BitCrusher button released!");
				break;
			case WM_NOTIFICATION_VALUE_CHANGED:
				switch(Id)
				{
				case BITCRUSHER_ID_BITRESOLUTION:
					x = MisaProgressbar_GetPercent(pMsg->hWinSrc);
					BitCrusherSetBitResolution(BitCrusherGetFX(), x<127 ? x*17/127 : 16);
					break;
				case BITCRUSHER_ID_BRIGHTNESS:
					x = MisaProgressbar_GetPercent(pMsg->hWinSrc);
					BitCrusherSetBrightness(BitCrusherGetFX(), x<127?x:127);
					break;
				case BITCRUSHER_ID_DOWNSAMPLINGFACTOR:
					x = MisaProgressbar_GetPercent(pMsg->hWinSrc);
					BitCrusherSetDownsampling(BitCrusherGetFX(), x<127 ? (x>>3) + 1 : 16);
					break;
				default:
					;
				}
				break;
			case WM_NOTIFICATION_SEL_CHANGED:
				if(hBitCrusherItems[BITCRUSHER_BITRESOLUTION]!=pMsg->hWinSrc && hBitCrusherItems[BITCRUSHER_DOWNSAMPLINGFACTOR]!=pMsg->hWinSrc)
				{
					std::vector<int> effect;
					x = WM_GetWindowOrgX(pMsg->hWinSrc)+50;
					y = WM_GetWindowOrgY(pMsg->hWinSrc)+340;
					MisaSetcontrolmenu_Popup(pMsg->hWin,pMsg->hWinSrc,x,y);
					effect = SynthFindEffect(SynthTranslateEffect(Id),BitCrusherGetFX());
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

static void BitCrusherControlMenuProc(int menuId)
{
	AssignEffectControlMenuProc(menuId,BitCrusherGetFX());
}


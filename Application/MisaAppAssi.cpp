
#include <iostream>

#include "GUI.h"
#include "BUTTON.h"
#include "RADIO.h"
#include "CHECKBOX.h"

#include "MisaWidget.h"

#include "MisaAppInc.h"

#include "MainMenu.h"

#include "MisamenuConfig.h"

#include "Resource.h"

///////////////////////////////////////////////////////////////////////////////
//

#define COMMCOMPONENT_XOFFSET 2
#define COMMCOMPONENT_YOFFSET 2

#define SMOOTH_DX 80
///////////////////////////////////////////////////////////////////////////////
//


///////////////////////////////////////////////////////////////////////////////
//

static COMMON_STATUS com_status;

///////////////////////////////////////////////////////////////////////////////
//

COMMCOMPONENT CreateCommComponent(WM_HWIN hParent)
{
	int x,y;
	COMMCOMPONENT hComponents;
	memset(&hComponents,0,sizeof(hComponents));
	x=COMMCOMPONENT_XOFFSET;
	y=500;
	hComponents.hFX1 = MisaRadio_Create(x,y,bmFX1_NORMAL.XSize,bmFX1_NORMAL.YSize,hParent,
		COMMON_ID_FX1,WM_CF_SHOW | WM_CF_MEMDEV,&bmFX1_NORMAL,&bmFX1_PRESSED);
	x+=bmFX1_NORMAL.XSize;
	
	hComponents.hFX2 = MisaRadio_Create(x,y,bmFX2_NORMAL.XSize,bmFX2_NORMAL.YSize,hParent,
		COMMON_ID_FX2,WM_CF_SHOW | WM_CF_MEMDEV,&bmFX2_NORMAL,&bmFX2_PRESSED);
	x+=bmFX2_NORMAL.XSize;
	x+=COMMCOMPONENT_XOFFSET;
	
	hComponents.hCheckOn = MisaCheckbox_Create(x,y,bmCHECKON_NORMAL.XSize,bmCHECKON_NORMAL.YSize,hParent,COMMON_ID_CHECKON, WM_CF_SHOW | WM_CF_MEMDEV,&bmCHECKON_NORMAL,&bmCHECKON_PRESSED);
	x+=bmCHECKON_NORMAL.XSize;
	x+=COMMCOMPONENT_XOFFSET;
	y+=COMMCOMPONENT_YOFFSET;

	x++;
	hComponents.hClose = BUTTON_CreateAsChild(x,y,bmCLOSE_NORMAL.XSize,bmCLOSE_NORMAL.YSize,hParent,COMMON_ID_CLOSE,WM_CF_SHOW | WM_CF_MEMDEV);
	BUTTON_SetFocussable(hComponents.hClose,0);
	BUTTON_SetBitmap(hComponents.hClose,BUTTON_CI_UNPRESSED,&bmCLOSE_NORMAL);
	BUTTON_SetBitmap(hComponents.hClose,BUTTON_CI_PRESSED,&bmCLOSE_PRESSED);
	x+=bmCLOSE_NORMAL.XSize;
	x+=COMMCOMPONENT_XOFFSET;
	com_status.Fx_Sel = 0;
	com_status.OnCheck = 0;
	MisaRadio_SetStatus(hComponents.hFX1,1);
	MisaRadio_SetStatus(hComponents.hCheckOn,0);
	
	return hComponents;
}

COMMCOMPONENT CreateCommComponentEx(WM_HWIN hParent,U32 items)
{
	int x,y;
	COMMCOMPONENT hComponents;
	memset(&hComponents,0,sizeof(hComponents));
	x=COMMCOMPONENT_XOFFSET;
	y=500;
	if(items & COMMON_ITEM_FX1)
	{
		hComponents.hFX1 = MisaRadio_Create(x,y,bmFX1_NORMAL.XSize,bmFX1_NORMAL.YSize,hParent,
			COMMON_ID_FX1,WM_CF_SHOW | WM_CF_MEMDEV,&bmFX1_NORMAL,&bmFX1_PRESSED);
	}
	x+=bmFX1_NORMAL.XSize;
	if(items & COMMON_ITEM_FX2)
	{
		hComponents.hFX2 = MisaRadio_Create(x,y,bmFX2_NORMAL.XSize,bmFX2_NORMAL.YSize,hParent,
			COMMON_ID_FX2,WM_CF_SHOW | WM_CF_MEMDEV,&bmFX2_NORMAL,&bmFX2_PRESSED);
	}
	x+=bmFX2_NORMAL.XSize;
	x+=COMMCOMPONENT_XOFFSET;
	if(items & COMMON_ITEM_CHECK)
	{
		hComponents.hCheckOn = MisaCheckbox_Create(x,y,bmCHECKON_NORMAL.XSize,bmCHECKON_NORMAL.YSize,hParent,COMMON_ID_CHECKON, WM_CF_SHOW | WM_CF_MEMDEV,&bmCHECKON_NORMAL,&bmCHECKON_PRESSED);
	}
	x+=bmCHECKON_NORMAL.XSize;
	x+=COMMCOMPONENT_XOFFSET;
	y+=COMMCOMPONENT_YOFFSET;

	x++;
	if(items & COMMON_ITEM_CLOSE)
	{
		hComponents.hClose = BUTTON_CreateAsChild(x,y,bmCLOSE_NORMAL.XSize,bmCLOSE_NORMAL.YSize,hParent,COMMON_ID_CLOSE,WM_CF_SHOW | WM_CF_MEMDEV);
		BUTTON_SetFocussable(hComponents.hClose,0);
		BUTTON_SetBitmap(hComponents.hClose,BUTTON_CI_UNPRESSED,&bmCLOSE_NORMAL);
		BUTTON_SetBitmap(hComponents.hClose,BUTTON_CI_PRESSED,&bmCLOSE_PRESSED);
	}
	x+=bmCLOSE_NORMAL.XSize;
	x+=COMMCOMPONENT_XOFFSET;
	com_status.Fx_Sel = 0;
	com_status.OnCheck = 0;
	if(items & COMMON_ITEM_FX1)
	{
		MisaRadio_SetStatus(hComponents.hFX1,1);
	}
	if(items & COMMON_ITEM_CHECK)
	{
		MisaRadio_SetStatus(hComponents.hCheckOn,0);
	}
	
	return hComponents;
}

U8 DeleteCommComponent(COMMCOMPONENT hComponents)
{
	if(hComponents.hClose && WM_IsWindow(hComponents.hClose))
	{
		BUTTON_Delete(hComponents.hClose);
	}
	if(hComponents.hCheckOn && WM_IsWindow(hComponents.hCheckOn))
	{
		MisaCheckbox_Delete(hComponents.hCheckOn);
	}
	if(hComponents.hFX1 && WM_IsWindow(hComponents.hFX1))
	{
		MisaRadio_Delete(hComponents.hFX1);
	}
	if(hComponents.hFX2 && WM_IsWindow(hComponents.hFX2))
	{
		MisaRadio_Delete(hComponents.hFX2);
	}
	return 0;
}

U8 ProcessCommComponent(WM_MESSAGE* pMsg, LPCOMMCOMPONENT pCommComp)
{
#if 0
	switch (pMsg->MsgId)
	{
	case COMMON_ID_COMCHANGE:
		if(pMsg->hWinSrc != pMsg->hWin)
		{
			if(com_status.Fx_Sel)
			{
				MisaRadio_SetStatus(pCommComp->hFX1,0);
				MisaRadio_SetStatus(pCommComp->hFX2,1);
			}
			else
			{
				MisaRadio_SetStatus(pCommComp->hFX1,1);
				MisaRadio_SetStatus(pCommComp->hFX2,0);
			}
			MisaCheckbox_SetStatus(pCommComp->hCheckOn, com_status.OnCheck);
		}
		break;
	}
#endif
	return 0;
}

U8 GetSlidingBordercheck(WM_HWIN hSlide, WM_HWIN hParent, int IndicatorFrame /* = INDICATORFRAME */)
{
	U8 res;
	int x,y;
	GUI_RECT lrect,rrect;
	x = WM_GetWindowOrgX(hSlide);
	y = WM_GetWindowSizeX(hSlide);
	res = 0;
	if(IndicatorFrame <= x)
	{
		res = 1;
	}
	else if(x < IndicatorFrame && x+y > 800 - IndicatorFrame)
	{
		res = 3;
	}
	else if(x+y <= 800 - IndicatorFrame)
	{
		res = 2;
	}
	y = WM_GetWindowOrgY(hSlide);
	y += 170;
	x = 4;
	lrect.x0=x;
	lrect.y0=y;
	lrect.x1=x+bmLARROW.XSize;
	lrect.y1=y+bmLARROW.YSize;
	//GUI_DrawBitmap(&bmLARROW,x,y);
	WM_InvalidateRect(hParent, &lrect);
	x = 750 + 15;
	rrect.x0=x;
	rrect.y0=y;
	rrect.x1=x+bmRARROW.XSize;
	rrect.y1=y+bmRARROW.YSize;
	//GUI_DrawBitmap(&bmRARROW,x,y);
	WM_InvalidateRect(hParent, &rrect);
	return res;
}


U8 SlideGoNextPage(WM_HWIN hSlideWin, int IndicatorFrame /* = INDICATORFRAME */)
{
	int xRight, n;
	for (n = 0; n < 4; n++)
	{
		xRight = WM_GetWindowOrgX(hSlideWin) + WM_GetWindowSizeX(hSlideWin);
		int shiftValue = xRight < GUI_GetScreenSizeX()-IndicatorFrame - PAGE_NEGATIVE_FACTOR ? GUI_GetScreenSizeX()-IndicatorFrame - xRight : PAGE_NEGATIVE_FACTOR;
		WM_MoveWindow(hSlideWin, shiftValue, 0);
		GUI_Delay(10);
	}

	return GetSlidingBordercheck(hSlideWin, WM_GetParent(WM_GetParent(hSlideWin)), IndicatorFrame);
}

U8 SlideGoPreviousPage(WM_HWIN hSlideWin, int IndicatorFrame /* = INDICATORFRAME */)
{
	int x, n;
	for (n = 0; n < 4; n++)
	{
		x = WM_GetWindowOrgX(hSlideWin);
		int shiftValue = PAGE_POSITIVE_FACTOR > IndicatorFrame-x ? IndicatorFrame-x : PAGE_POSITIVE_FACTOR;
		WM_MoveWindow(hSlideWin, shiftValue, 0);
		GUI_Delay(10);
	}
	return GetSlidingBordercheck(hSlideWin, WM_GetParent(WM_GetParent(hSlideWin)), IndicatorFrame);
}

U8 DispatchCommComponent(WM_MESSAGE* pMsg, LPCOMMCOMPONENT pCommComp)
{
	U8 reBack;
	int NCode,Id;
	//WM_MESSAGE message;
	reBack = 0;
	Id = WM_GetId(pMsg->hWinSrc);
	NCode = pMsg->Data.v;
	switch(NCode)
	{
	case WM_NOTIFICATION_CLICKED:
		break;
	case WM_NOTIFICATION_RELEASED:
		switch(Id)
		{
		case COMMON_ID_CLOSE:
			TopMainMenu(pMsg->hWin);
			reBack = 1;
			break;
#if 0
		case COMMON_ID_CHECKON:
			com_status.OnCheck = MisaCheckbox_GetStatus(pCommComp->hCheckOn);
			message.MsgId = COMMON_ID_COMCHANGE;
			message.hWin = 0;
			message.hWinSrc = pMsg->hWin;
			message.Data.v = com_status.OnCheck|(com_status.Fx_Sel<<8);
			WM_BroadcastMessage(&message);
			break;
#endif
		case COMMON_ID_FX1:
			if(pCommComp->hFX2)
			{
				MisaRadio_SetStatus(pCommComp->hFX2,0);
				/*com_status.Fx_Sel = 0;
				message.MsgId = COMMON_ID_COMCHANGE;
				message.hWin = 0;
				message.hWinSrc = pMsg->hWin;
				message.Data.v = com_status.OnCheck|(com_status.Fx_Sel<<8);
				WM_BroadcastMessage(&message);*/
			}
			reBack = 1;
			break;
		case COMMON_ID_FX2:
			if(pCommComp->hFX1)
			{
				MisaRadio_SetStatus(pCommComp->hFX1,0);
				/*com_status.Fx_Sel = 1;
				message.MsgId = COMMON_ID_COMCHANGE;
				message.hWin = 0;
				message.hWinSrc = pMsg->hWin;
				message.Data.v = com_status.OnCheck|(com_status.Fx_Sel<<8);
				WM_BroadcastMessage(&message);*/
			}
			reBack = 1;
			break;
		default:
			reBack = 1;
		}
		break;
	default:
		reBack = 1;
		break;
	}
	return reBack;
}

void AssignEffectControlMenuProc(int menuId,int fx)
{
	int Id;
	WM_HWIN src;
	src = MisaSetcontrolmenu_GetSrcWin();
	Id = WM_GetId(src);
	switch(menuId)
	{
	case CTL_NONE_ID:
	default:
		SynthRemoveEffect(SynthTranslateEffect(Id),fx);
		MisaProgressbar_SetCheck(src,0);
		break;
	case CTL_TOUCH_X_ID:
	case CTL_TOUCH_Y_ID:
	case CTL_DRAG_X_ID:
	case CTL_DRAG_Y_ID:
	case CTL_BALL_X_ID:
	case CTL_BALL_Y_ID:
		SynthRemoveEffect(SynthTranslateEffect(Id),fx);
		SynthAssignEffect(SynthTranslateCtrl(menuId),SynthTranslateEffect(Id),fx,MisaSetcontrolmenu_GetInverse()?true:false,0,0);
		MisaProgressbar_SetCheck(src,1);
		break;
	case CTL_VARIATION_ID:
		SynthRemoveEffect(SynthTranslateEffect(Id),fx);
		SynthAssignEffect(SynthTranslateCtrl(menuId),SynthTranslateEffect(Id),fx,MisaSetcontrolmenu_GetInverse()?true:false,MisaSetcontrolmenu_GetVarMin(),MisaSetcontrolmenu_GetVarMax());
		MisaProgressbar_SetCheck(src,1);
	}
}

void AssignEffectControlMenuProc_SPC(int menuId,int fx)
{
	int Id;
	WM_HWIN src;
	src = MisaSetcontrolmenu_GetSrcWin();
	Id = WM_GetId(src);
	switch(menuId)
	{
	case CTL_NONE_ID:
	default:
		SynthRemoveEffect(SynthTranslateEffect(Id),fx);
		MisaSmallProgressCombo_SetCheck(src,0);
		break;
	case CTL_TOUCH_X_ID:
	case CTL_TOUCH_Y_ID:
	case CTL_DRAG_X_ID:
	case CTL_DRAG_Y_ID:
	case CTL_BALL_X_ID:
	case CTL_BALL_Y_ID:
		SynthRemoveEffect(SynthTranslateEffect(Id),fx);
		SynthAssignEffect(SynthTranslateCtrl(menuId),SynthTranslateEffect(Id),fx,MisaSetcontrolmenu_GetInverse()?true:false,0,0);
		MisaSmallProgressCombo_SetCheck(src,1);
		break;
	case CTL_VARIATION_ID:
		SynthRemoveEffect(SynthTranslateEffect(Id),fx);
		SynthAssignEffect(SynthTranslateCtrl(menuId),SynthTranslateEffect(Id),fx,MisaSetcontrolmenu_GetInverse()?true:false,MisaSetcontrolmenu_GetVarMin(),MisaSetcontrolmenu_GetVarMax());
		MisaSmallProgressCombo_SetCheck(src,1);
	}
}

void AssignVoiceControlMenuProc(int menuId, unsigned char str, unsigned char voice_index)
{
	int Id;
	WM_HWIN src;
	src = MisaSetcontrolmenu_GetSrcWin();
	Id = WM_GetId(src);
	switch(menuId)
	{
	case CTL_NONE_ID:
	default:
		VoiceRemoveEffect(SynthTranslateEffect(Id), str, voice_index);
		MisaSmallProgressCombo_SetCheck(src,0);
		break;
	case CTL_TOUCH_X_ID:
	case CTL_TOUCH_Y_ID:
	case CTL_DRAG_X_ID:
	case CTL_DRAG_Y_ID:
	case CTL_BALL_X_ID:
	case CTL_BALL_Y_ID:
		VoiceRemoveEffect(SynthTranslateEffect(Id), str, voice_index);
		VoiceAssignEffect(SynthTranslateCtrl(menuId),SynthTranslateEffect(Id), str, voice_index,MisaSetcontrolmenu_GetInverse()?true:false,0,0);
		MisaSmallProgressCombo_SetCheck(src,1);
		break;
	case CTL_VARIATION_ID:
		VoiceRemoveEffect(SynthTranslateEffect(Id), str, voice_index);
		VoiceAssignEffect(SynthTranslateCtrl(menuId),SynthTranslateEffect(Id), str, voice_index,MisaSetcontrolmenu_GetInverse()?true:false,MisaSetcontrolmenu_GetVarMin(),MisaSetcontrolmenu_GetVarMax());
		MisaSmallProgressCombo_SetCheck(src,1);
	}
}

void Misa_SmoothlyMoveWindow(WM_HWIN hWin,WM_HWIN hParent,int Dx)
{
#if 0
	int loop,dx;
	dx = Dx%10;
	WM_MoveWindow(hWin,dx,0);
	dx = Dx/10;
	loop = 10;
	while(loop--)
	{
		WM_MoveWindow(hWin,dx,0);
		if(WM_IsVisible(hParent))
		{
			#ifdef Linux
			RunFramework();
			#else
			GUI_Delay(10);
			#endif
		}
	}
#else
	int loop,dx;
	dx = Dx%SMOOTH_DX;
	WM_MoveWindow(hWin,dx,0);
	dx = Dx>0?SMOOTH_DX:-SMOOTH_DX;
	loop = abs(Dx/SMOOTH_DX);
	while(loop--)
	{
		WM_MoveWindow(hWin,dx,0);
		if(WM_IsVisible(hParent))
		{
			#ifdef Linux
			RunFramework();
			#else
			GUI_Delay(10);
			#endif
		}
	}
#endif
}

void Debug_stdout(char* content)
{
	std::cout<<content<<std::endl<<std::flush;
}


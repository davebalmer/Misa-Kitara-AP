
#include <cstdlib>
#include <iostream>
#include <stdlib.h>

#include "GUI.h"
#include "WM.h"
#include "TEXT.h"
#include "BUTTON.h"
#include "BUTTON_Private.h"

#include "MisaAppInc.h"
#include "VoiceScreen.h"

#include "MisaWidget.h"

#include "MisamenuConfig.h"

#include "Resource.h"

// OR : Drag & Drop  (for SynthGetStringIndex())
#include "SynthScreen.h"
DragVoice Drag;
#define START_DRAG_TIMER 2

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa checkbox
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

static WM_CALLBACK* DefaultCheckboxProc;
static void MisaCheckboxProc(WM_MESSAGE* pMsg);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa checkbox
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

BUTTON_Handle MisaCheckbox_Create(int x0, int y0, int xsize, int ysize, WM_HWIN hParent, int Id, int Flags,const GUI_BITMAP * pUncheck,const GUI_BITMAP * pCheck)
{
	BUTTON_Handle hMisaCeckbox = BUTTON_CreateAsChild(x0,y0,xsize,ysize,hParent,Id,WM_CF_SHOW | WM_CF_MEMDEV);
	BUTTON_SetFocussable(hMisaCeckbox,0);
	BUTTON_SetBitmap(hMisaCeckbox,BUTTON_CI_UNPRESSED,pUncheck);
	BUTTON_SetBitmap(hMisaCeckbox,BUTTON_CI_PRESSED,pCheck);
	DefaultCheckboxProc = WM_SetCallback(hMisaCeckbox,MisaCheckboxProc);
	return hMisaCeckbox;
}

void MisaCheckbox_Delete(BUTTON_Handle hMisaCheckbox)
{
	BUTTON_Delete(hMisaCheckbox);
}

void MisaCheckbox_SetStatus(BUTTON_Handle hMisaCheckbox,int State)
{
	BUTTON_SetState(hMisaCheckbox,State<<8);
}

U8 MisaCheckbox_GetStatus(BUTTON_Handle hMisaCheckbox)
{
	return BUTTON_IsPressed(hMisaCheckbox);
}

static void MisaCheckboxOntouch(BUTTON_Handle hObj, BUTTON_Obj* pObj, WM_MESSAGE*pMsg)
{
	const GUI_PID_STATE* pState = (const GUI_PID_STATE*)pMsg->Data.p;
	if (pMsg->Data.p)
	{
		if(pState->Pressed)
		{
			// OR : Drag & Drop
			if (Drag.IsDragged())
			{
				Drag.MoveOn(pState->x + WM_GetWindowOrgX(pMsg->hWin), pState->y + WM_GetWindowOrgY(pMsg->hWin), WM_GetParent(pMsg->hWin));
				return;
			}

			WM_NotifyParent(hObj, WM_NOTIFICATION_CLICKED);
		}
		else
		{
			if(pObj->Widget.State & BUTTON_STATE_PRESSED)
			{
				WIDGET_AndState(hObj, BUTTON_STATE_PRESSED);
			}
			else
			{
				WIDGET_OrState(hObj, BUTTON_STATE_PRESSED);
			}
			WM_NotifyParent(hObj, WM_NOTIFICATION_RELEASED);
		}
	}
}

static void MisaCheckboxProc(WM_MESSAGE* pMsg)
{
	BUTTON_Handle hObj = pMsg->hWin;
	BUTTON_Obj * pObj = (BUTTON_Obj *)GUI_ALLOC_h2p(hObj);
	switch (pMsg->MsgId)
	{
	case WM_TOUCH:
		MisaCheckboxOntouch(hObj, pObj, pMsg);
		break;
	default:
		if(DefaultCheckboxProc)
		{
			DefaultCheckboxProc(pMsg);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa radio button
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

static WM_CALLBACK* DefaultRadioProc;
static void MisaRadioProc(WM_MESSAGE* pMsg);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa radio button
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

BUTTON_Handle MisaRadio_Create(int x0, int y0, int xsize, int ysize, WM_HWIN hParent, int Id, int Flags,const GUI_BITMAP * pUncheck,const GUI_BITMAP * pCheck)
{	
	BUTTON_Handle hMisaRadio = BUTTON_CreateAsChild(x0,y0,xsize,ysize,hParent,Id,WM_CF_SHOW | WM_CF_MEMDEV);
	BUTTON_SetFocussable(hMisaRadio,0);
	if(pUncheck)
	{
		BUTTON_SetBitmap(hMisaRadio,BUTTON_CI_UNPRESSED,pUncheck);
	}
	else
	{
		BUTTON_SetBkColor(hMisaRadio, BUTTON_CI_UNPRESSED, GUI_BLACK);
	}
	if(pCheck)
	{
		BUTTON_SetBitmap(hMisaRadio,BUTTON_CI_PRESSED,pCheck);
	}
	else
	{
		BUTTON_SetBkColor(hMisaRadio,BUTTON_CI_PRESSED,GUI_GRAY);
	}
	DefaultRadioProc = WM_SetCallback(hMisaRadio,MisaRadioProc);
	return hMisaRadio;
}

BUTTON_Handle MisaRadio_CreateEx(int x0, int y0, int xsize, int ysize, WM_HWIN hParent, int Id, int Flags, const char* pName, const GUI_BITMAP * pUncheck,const GUI_BITMAP * pCheck)
{	
	BUTTON_Handle hMisaRadio = BUTTON_CreateAsChild(x0,y0,xsize,ysize,hParent,Id,WM_CF_SHOW | WM_CF_MEMDEV);
	BUTTON_SetFocussable(hMisaRadio,0);
	if(pUncheck)
	{
		BUTTON_SetBitmap(hMisaRadio,BUTTON_CI_UNPRESSED,pUncheck);
	}
	else
	{
		BUTTON_SetBkColor(hMisaRadio, BUTTON_CI_UNPRESSED, GUI_BLACK);
	}
	if(pCheck)
	{
		BUTTON_SetBitmap(hMisaRadio,BUTTON_CI_PRESSED,pCheck);
	}
	else
	{
		BUTTON_SetBkColor(hMisaRadio,BUTTON_CI_PRESSED,GUI_GRAY);
	}
	if(pName)
	{
		BUTTON_SetTextColor(hMisaRadio, BUTTON_CI_UNPRESSED, GUI_WHITE);
		BUTTON_SetTextColor(hMisaRadio, BUTTON_CI_PRESSED, GUI_WHITE);
		BUTTON_SetFont(hMisaRadio, &GUI_Font24_ASCII);
		BUTTON_SetText(hMisaRadio, pName);
	}
	DefaultRadioProc = WM_SetCallback(hMisaRadio,MisaRadioProc);
	return hMisaRadio;
}

void MisaRadio_Delete(BUTTON_Handle hMisaRadio)
{
	BUTTON_Delete(hMisaRadio);
}

void MisaRadio_SetGoup(MISARADIO_GROUP radioGroup)
{
}

void MisaRadio_SetStatus(BUTTON_Handle hMisaRadio,int State)
{
	BUTTON_SetState(hMisaRadio,State<<8);
}

U8 MisaRadio_GetStatus(BUTTON_Handle hMisaRadio)
{
	return BUTTON_IsPressed(hMisaRadio);
}

static void MisaRadioOntouch(BUTTON_Handle hObj, BUTTON_Obj* pObj, WM_MESSAGE*pMsg)
{
	const GUI_PID_STATE* pState = (const GUI_PID_STATE*)pMsg->Data.p;
	if (pMsg->Data.p)
	{
		if(pState->Pressed)
		{
			WM_NotifyParent(hObj, WM_NOTIFICATION_CLICKED);
		}
		else
		{
			if(pObj->Widget.State & BUTTON_STATE_PRESSED)
			{
			}
			else
			{
				WIDGET_OrState(hObj, BUTTON_STATE_PRESSED);
			}
			WM_NotifyParent(hObj, WM_NOTIFICATION_RELEASED);
		}
	}
}

static void MisaRadioProc(WM_MESSAGE* pMsg)
{
	BUTTON_Handle hObj = pMsg->hWin;
	BUTTON_Obj * pObj = (BUTTON_Obj *)GUI_ALLOC_h2p(hObj);
	switch (pMsg->MsgId)
	{
	case WM_TOUCH:
		MisaRadioOntouch(hObj, pObj, pMsg);
		break;
	default:
		if(DefaultRadioProc)
		{
			DefaultRadioProc(pMsg);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa progress bar
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#define MISAPROGRESS_MAX 127
#define MISAPROGRESS_MIN 0

typedef struct __MISAPROGRESSPROP
{
	int Id;
	int percent;
	int fontSize;
	int fontcolor;
	MISAPROGINFOCB ShowTips;
	char label[16];
	char seclab[16];
	bool IsVolumeSlider;
} MISAPROGRESSPROP,*LPMISAPROGRESSPROP;

typedef struct __MISAPROG_OBJ
{
	WM_Obj Win;
	MISAPROGRESSPROP progprop;
} MISAPROG_OBJ;

static WM_CALLBACK* DefaultProgressbarProc;
static void MisaProgressbarProc(WM_MESSAGE* pMsg);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa progress bar
// 312
// 315
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

WM_HWIN MisaProgressbar_Create(int x, int y, int width, int height, WM_HWIN hParent, int Id, const char* label, int fontSize)
{
	WM_HWIN hMisaProgress;
	MISAPROGRESSPROP prop;
	//hMisaProgress = WM_CreateWindowAsChild(x, y, width, height, WM_CF_SHOW | WM_CF_MEMDEV, MisaProgressbarProc, 0);
	hMisaProgress = WM_CreateWindowAsChild(x, y, bmSLIDER.XSize, bmSLIDER.YSize, hParent, WM_CF_SHOW | WM_CF_MEMDEV, MisaProgressbarProc, sizeof(MISAPROG_OBJ)-sizeof(WM_Obj));
	prop.percent = 0;
	prop.Id = Id;
	prop.fontSize = fontSize;
	prop.fontcolor = 0;
	prop.ShowTips = 0;
	prop.IsVolumeSlider = false;
	if(label)
	{
		strcpy(prop.label,label);
	}
	else
	{
		memset(prop.label,0,16);
	}
	memset(prop.seclab,0,16);
	WM_SetUserData(hMisaProgress, &prop, sizeof(MISAPROGRESSPROP));
	return hMisaProgress;
}

WM_HWIN MisaProgressbar_CreateEx(int x, int y, int width, int height, WM_HWIN hParent, int Id, const char* label, const char* seclab, int fontSize,MISAPROGINFOCB TipCB)
{
	WM_HWIN hMisaProgress;
	MISAPROGRESSPROP prop;
	//hMisaProgress = WM_CreateWindowAsChild(x, y, width, height, WM_CF_SHOW | WM_CF_MEMDEV, MisaProgressbarProc, 0);
	if (width == bmQS_VOLUME_BKG.XSize && height == bmQS_VOLUME_BKG.YSize)
	{
		prop.IsVolumeSlider = true;
		hMisaProgress = WM_CreateWindowAsChild(x, y, bmQS_VOLUME_BKG.XSize, bmQS_VOLUME_BKG.YSize, hParent, WM_CF_SHOW | WM_CF_MEMDEV, MisaProgressbarProc, sizeof(MISAPROG_OBJ)-sizeof(WM_Obj));
	}
	else
	{
		prop.IsVolumeSlider = false;
		hMisaProgress = WM_CreateWindowAsChild(x, y, bmSLIDER.XSize, bmSLIDER.YSize, hParent, WM_CF_SHOW | WM_CF_MEMDEV, MisaProgressbarProc, sizeof(MISAPROG_OBJ)-sizeof(WM_Obj));
	}
	prop.percent = 0;
	prop.Id = Id;
	prop.fontSize = fontSize;
	prop.fontcolor = 0;
	prop.ShowTips = TipCB;
	if(label)
	{
		strcpy(prop.label,label);
	}
	else
	{
		memset(prop.label,0,16);
	}
	if(seclab)
	{
		strcpy(prop.seclab,seclab);
	}
	else
	{
		memset(prop.seclab,0,16);
	}
	WM_SetUserData(hMisaProgress, &prop, sizeof(MISAPROGRESSPROP));
	return hMisaProgress;
}

void MisaProgressbar_Delete(WM_HWIN hMisaProgress)
{
	WM_DeleteWindow(hMisaProgress);
}

int MisaProgressbar_GetPercent(WM_HWIN hMisaProgress)
{
	int result;
	MISAPROGRESSPROP prop;
	WM_GetUserData(hMisaProgress, &prop, sizeof(MISAPROGRESSPROP));
	result = prop.percent;
	return result;
}

void MisaProgressbar_SetPercent(WM_HWIN hMisaProgress, int percent)
{
	U8 changed;
	MISAPROGRESSPROP prop;
	WM_GetUserData(hMisaProgress, &prop, sizeof(MISAPROGRESSPROP));
	if(percent>MISAPROGRESS_MAX)
	{
		percent = MISAPROGRESS_MAX;
	}
	else if(percent<0)
	{
		percent = 0;
	}
	changed = percent != prop.percent;
	prop.percent = percent;
	WM_SetUserData(hMisaProgress, &prop, sizeof(MISAPROGRESSPROP));
	WM_InvalidateWindow(hMisaProgress);
	if(changed)
	{
		WM_NotifyParent(hMisaProgress, WM_NOTIFICATION_VALUE_CHANGED);
	}
}

void MisaProgressbar_SetPercentEx(WM_HWIN hMisaProgress, int percent, U8 update)
{
	U8 changed;
	MISAPROGRESSPROP prop;
	WM_GetUserData(hMisaProgress, &prop, sizeof(MISAPROGRESSPROP));
	if(percent>MISAPROGRESS_MAX)
	{
		percent = MISAPROGRESS_MAX;
	}
	else if(percent<0)
	{
		percent = 0;
	}
	changed = percent != prop.percent;
	prop.percent = percent;
	WM_SetUserData(hMisaProgress, &prop, sizeof(MISAPROGRESSPROP));
	WM_InvalidateWindow(hMisaProgress);
	if(changed && update)
	{
		WM_NotifyParent(hMisaProgress, WM_NOTIFICATION_VALUE_CHANGED);
	}
}

void MisaProgressbar_Increase(WM_HWIN hMisaProgress)
{
	U8 changed;
	MISAPROGRESSPROP prop;
	WM_GetUserData(hMisaProgress, &prop, sizeof(MISAPROGRESSPROP));
	prop.percent++;
	changed = 1;
	if(MISAPROGRESS_MAX<=prop.percent)
	{
		prop.percent = MISAPROGRESS_MAX;
		changed = 0;
	}
	WM_SetUserData(hMisaProgress, &prop, sizeof(MISAPROGRESSPROP));
	WM_InvalidateWindow(hMisaProgress);
	if(changed)
	{
		WM_NotifyParent(hMisaProgress, WM_NOTIFICATION_VALUE_CHANGED);
	}
}

void MisaProgressbar_Decrease(WM_HWIN hMisaProgress)
{	
	U8 changed;
	MISAPROGRESSPROP prop;
	WM_GetUserData(hMisaProgress, &prop, sizeof(MISAPROGRESSPROP));
	prop.percent--;
	changed = 1;
	if(0>=prop.percent)
	{
		prop.percent = 0;
		changed = 0;
	}
	WM_SetUserData(hMisaProgress, &prop, sizeof(MISAPROGRESSPROP));
	WM_InvalidateWindow(hMisaProgress);
	if(changed)
	{
		WM_NotifyParent(hMisaProgress, WM_NOTIFICATION_VALUE_CHANGED);
	}
}

int MisaProgressbar_GetCheck(WM_HWIN hMisaProgress)
{
	MISAPROGRESSPROP prop;
	WM_GetUserData(hMisaProgress, &prop, sizeof(MISAPROGRESSPROP));
	return prop.fontcolor;
}

void MisaProgressbar_SetCheck(WM_HWIN hMisaProgress, int state)
{
	MISAPROGRESSPROP prop;
	GUI_RECT rect = {0,315,100,366};
	WM_GetUserData(hMisaProgress, &prop, sizeof(MISAPROGRESSPROP));
	prop.fontcolor = state;
	WM_SetUserData(hMisaProgress, &prop, sizeof(MISAPROGRESSPROP));
	WM_InvalidateRect(hMisaProgress, &rect);
	//WM_InvalidateWindow(hMisaProgress);
}

static void MisaProgressbarProc(WM_MESSAGE* pMsg)
{
	int i,x,y;
	MISAPROGRESSPROP prop;
	GUI_PID_STATE* pPID_State;

	WM_GetUserData(pMsg->hWin, &prop, sizeof(MISAPROGRESSPROP));
	switch (pMsg->MsgId)
	{
	case WM_PAINT:
		x = WM_GetWindowSizeX(pMsg->hWin);

		if (prop.IsVolumeSlider)
			GUI_DrawBitmap(&bmQS_VOLUME_BKG, 0, 0);
		else
			GUI_DrawBitmap(&bmSLIDER, 0, 0);

		GUI_SetTextMode(GUI_TEXTMODE_TRANS);
		GUI_SetColor(GUI_WHITE);
		// Debug version only.
		if(prop.ShowTips)
		{
			char buf[32];
			prop.ShowTips(prop.percent,buf);
			GUI_DispStringAt(buf,0,0);
		}
		else
		{
			GUI_DispDecAt(prop.percent,0,0,3);
		}
		// call customized unit display function
		//
		switch(prop.fontSize)
		{
		case MISAPROG_FONT8:
			GUI_SetFont(&GUI_Font8_ASCII);
			y=332;
			i=4;
			break;
		case MISAPROG_FONT16:
			GUI_SetFont(&GUI_Font16B_ASCII);
			y=328;
			i=8;
			break;
		case MISAPROG_FONT24:
			GUI_SetFont(&GUI_Font24B_ASCII);
			y=324;
			i=10;
			break;
		case MISAPROG_FONT32:
			GUI_SetFont(&GUI_Font32B_ASCII);
			y=320;
			i=10;
			break;			
		default:
			GUI_SetFont(&GUI_Font8_ASCII);
			y=332;
			i=4;
		}
		switch(prop.fontcolor)
		{
		case 0:
			break;
		case 1:
			GUI_SetColor(GUI_RED);
			break;
		case 2:
			GUI_SetColor(GUI_GRAY);
			break;
		default:
			;
		}
		/*if(prop.fontcolor)
		{
			GUI_SetColor(GUI_RED);
		}*/
		if(0 == prop.seclab[0])
		{
			if (prop.IsVolumeSlider)
				y = 533;
			GUI_DispStringHCenterAt(prop.label, x/2, y);
		}
		else
		{
			GUI_DispStringHCenterAt(prop.label, x/2, y-i);
			GUI_DispStringHCenterAt(prop.seclab, x/2, y+i);
		}

		if (prop.IsVolumeSlider)
		{
			x = (int) (prop.percent*1.941);
			for(i=0; i < x; i++)
				GUI_DrawBitmap(&bmQS_VOLUME_FILL, 10, 503 - i*2);

			if(x == 246)		// 246 = 127 * 1.941
				GUI_DrawBitmap(&bmQS_VOLUME_TOP, 10, 10);
		
		}
		else
		{
			x = (int) (prop.percent*1.182);
			for(i=0;i<x;i++)
				GUI_DrawBitmap(&bmSLIDERCENT,10,312-i*2);

			if(x == 150)			// 246 = 127 * 1.182
				GUI_DrawBitmap(&bmSLIDERTOP,10,10);
		}
		break;

	case WM_TOUCH:
		if(pMsg->Data.p)
		{
			int h;
			float fFactor;
			pPID_State = (GUI_PID_STATE*)pMsg->Data.p;
			if(pPID_State->Pressed)
			{
				int perc;
				
				if (prop.IsVolumeSlider)
				{
					h = 503;
					fFactor = 0.518;
				}
				else
				{
					h = 315;
					fFactor = 0.847;		// inverse(1.18)  (to have a multiplication rather than a division)
				}

				if(!WM_HasCaptured(pMsg->hWin) && h > pPID_State->y)
				{
					WM_SetCapture(pMsg->hWin, 0);
				}

				if(h <= pPID_State->y)
					perc = 0;
				else
					perc = (h-pPID_State->y)/2;

				if(WM_HasCaptured(pMsg->hWin))
					MisaProgressbar_SetPercent(pMsg->hWin,(int) (perc * fFactor));

			}
			else
			{
				if(WM_HasCaptured(pMsg->hWin))
				{
					WM_ReleaseCapture();
					WM_NotifyParent(pMsg->hWin, WM_NOTIFICATION_RELEASED);
				}
				else
				{
					WM_NotifyParent(pMsg->hWin, WM_NOTIFICATION_SEL_CHANGED);
				}
			}
		}
		break;
	case WM_GET_ID:
		pMsg->Data.v = prop.Id;
		break;
	default:
		WM_DefaultProc(pMsg);
	}
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa popup menu
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

typedef struct __MISAMENUPROP
{
	WM_HWIN hSrcWin;
	int Id;
	MISAPOPMENUINFOCB ShowMin;
	MISAPOPMENUINFOCB ShowMax;
} MISAMENUPROP,*LPMISAMENUPROP;

typedef struct __MISAMENU_OBJ
{
	WM_Obj Win;
	MISAMENUPROP progprop;
} MISAMENU_OBJ;

typedef struct __MISACTLMENU
{
	const char* name;
	GUI_RECT itemRect;
	GUI_RECT inverseRect;
	int status;
	static int inverse;
	int value;
} MISACTLMENU,*PMISACTLMENU;

static WM_CALLBACK* DefaultmenuProc;
static void MisaMenuProc(WM_MESSAGE* pMsg);

static WM_HWIN hMisaSetControlMenu = 0;
static WM_HWIN hCtlMenuItems[CTL_MAX];

#define DEFAULTITEM_X_BEGIN 1
#define DEFAULTITEM_X_END 398
#define DEFAULTITEM_Y_BEGIN 51
#define DEFAULTITEM_Y_END 98
#define DEFAULTITEM_HEIGHT 48
#define DEFAULTMENUITEM_WIDTH 400
#define DEFAULTMENUITEM_HEIGHT 50
#define MENUITEMS 9
#define DEFAULTVARBUTTON_WIDTH 100
#define DEFAULTVARBUTTON_HEIGHT 30
#define DEFAULTINVBUTTON_WIDTH 50
#define DEFAULTINVBUTTON_HEIGHT 30
#define DEFAULTBUTTON_MARGIN 10

#define DEFAULTITEM_STATUS 0
#define DEFAULTINVERSE_STATUS 0
#define DEFAULTITEM_VALUE 0

#if 1
#if 1 // enlarge the inverse button

static MISACTLMENU CtlItems[CTL_MAX]=
{
	{
		"none",
		{DEFAULTITEM_X_BEGIN,DEFAULTITEM_Y_BEGIN,DEFAULTITEM_X_END,DEFAULTITEM_Y_END},
		{DEFAULTBUTTON_MARGIN,DEFAULTITEM_Y_BEGIN+DEFAULTBUTTON_MARGIN,DEFAULTINVBUTTON_WIDTH+DEFAULTBUTTON_MARGIN,DEFAULTITEM_Y_END-DEFAULTBUTTON_MARGIN},
		DEFAULTITEM_STATUS,
		DEFAULTITEM_VALUE
	},
	{
		"touch x",
		{DEFAULTITEM_X_BEGIN,DEFAULTITEM_Y_BEGIN+DEFAULTMENUITEM_HEIGHT,DEFAULTITEM_X_END,DEFAULTITEM_Y_END+DEFAULTMENUITEM_HEIGHT},
		{DEFAULTBUTTON_MARGIN,DEFAULTITEM_Y_BEGIN+DEFAULTBUTTON_MARGIN+DEFAULTMENUITEM_HEIGHT,DEFAULTBUTTON_MARGIN+DEFAULTINVBUTTON_WIDTH,DEFAULTITEM_Y_END+DEFAULTMENUITEM_HEIGHT-DEFAULTBUTTON_MARGIN},
		DEFAULTITEM_STATUS,
		DEFAULTITEM_VALUE
	},
	{
		"touch y",
		{DEFAULTITEM_X_BEGIN,DEFAULTITEM_Y_BEGIN+DEFAULTMENUITEM_HEIGHT*2,DEFAULTITEM_X_END,DEFAULTITEM_Y_END+DEFAULTMENUITEM_HEIGHT*2},
		{DEFAULTBUTTON_MARGIN,DEFAULTITEM_Y_BEGIN+DEFAULTBUTTON_MARGIN+DEFAULTMENUITEM_HEIGHT*2,DEFAULTBUTTON_MARGIN+DEFAULTINVBUTTON_WIDTH,DEFAULTITEM_Y_END+DEFAULTMENUITEM_HEIGHT*2-DEFAULTBUTTON_MARGIN},
		DEFAULTITEM_STATUS,
		DEFAULTITEM_VALUE
	},
	{
		"drag x",
		{DEFAULTITEM_X_BEGIN,DEFAULTITEM_Y_BEGIN+DEFAULTMENUITEM_HEIGHT*3,DEFAULTITEM_X_END,DEFAULTITEM_Y_END+DEFAULTMENUITEM_HEIGHT*3},
		{DEFAULTBUTTON_MARGIN,DEFAULTITEM_Y_BEGIN+DEFAULTBUTTON_MARGIN+DEFAULTMENUITEM_HEIGHT*3,DEFAULTBUTTON_MARGIN+DEFAULTINVBUTTON_WIDTH,DEFAULTITEM_Y_END+DEFAULTMENUITEM_HEIGHT*3-DEFAULTBUTTON_MARGIN},
		DEFAULTITEM_STATUS,
		DEFAULTITEM_VALUE
	},
	{
		"drag y",
		{DEFAULTITEM_X_BEGIN,DEFAULTITEM_Y_BEGIN+DEFAULTMENUITEM_HEIGHT*4,DEFAULTITEM_X_END,DEFAULTITEM_Y_END+DEFAULTMENUITEM_HEIGHT*4},
		{DEFAULTBUTTON_MARGIN,DEFAULTITEM_Y_BEGIN+DEFAULTBUTTON_MARGIN+DEFAULTMENUITEM_HEIGHT*4,DEFAULTBUTTON_MARGIN+DEFAULTINVBUTTON_WIDTH,DEFAULTITEM_Y_END+DEFAULTMENUITEM_HEIGHT*4-DEFAULTBUTTON_MARGIN},
		DEFAULTITEM_STATUS,
		DEFAULTITEM_VALUE
	},
	{
		"ball x",
		{DEFAULTITEM_X_BEGIN,DEFAULTITEM_Y_BEGIN+DEFAULTMENUITEM_HEIGHT*5,DEFAULTITEM_X_END,DEFAULTITEM_Y_END+DEFAULTMENUITEM_HEIGHT*5},
		{DEFAULTBUTTON_MARGIN,DEFAULTITEM_Y_BEGIN+DEFAULTBUTTON_MARGIN+DEFAULTMENUITEM_HEIGHT*5,DEFAULTBUTTON_MARGIN+DEFAULTINVBUTTON_WIDTH,DEFAULTITEM_Y_END+DEFAULTMENUITEM_HEIGHT*5-DEFAULTBUTTON_MARGIN},
		DEFAULTITEM_STATUS,
		DEFAULTITEM_VALUE
	},
	{
		"ball y",
		{DEFAULTITEM_X_BEGIN,DEFAULTITEM_Y_BEGIN+DEFAULTMENUITEM_HEIGHT*6,DEFAULTITEM_X_END,DEFAULTITEM_Y_END+DEFAULTMENUITEM_HEIGHT*6},
		{DEFAULTBUTTON_MARGIN,DEFAULTITEM_Y_BEGIN+DEFAULTBUTTON_MARGIN+DEFAULTMENUITEM_HEIGHT*6,DEFAULTBUTTON_MARGIN+DEFAULTINVBUTTON_WIDTH,DEFAULTITEM_Y_END+DEFAULTMENUITEM_HEIGHT*6-DEFAULTBUTTON_MARGIN},
		DEFAULTITEM_STATUS,
		DEFAULTITEM_VALUE
	},
	{
		"variation                   ",
		{DEFAULTITEM_X_BEGIN,DEFAULTITEM_Y_BEGIN+DEFAULTMENUITEM_HEIGHT*7,DEFAULTITEM_X_END,DEFAULTITEM_Y_END+DEFAULTMENUITEM_HEIGHT*7},
		{DEFAULTBUTTON_MARGIN,DEFAULTITEM_Y_BEGIN+DEFAULTBUTTON_MARGIN+DEFAULTMENUITEM_HEIGHT*7,DEFAULTBUTTON_MARGIN+DEFAULTINVBUTTON_WIDTH,DEFAULTITEM_Y_END+DEFAULTMENUITEM_HEIGHT*7-DEFAULTBUTTON_MARGIN},
		DEFAULTITEM_STATUS,
		DEFAULTITEM_VALUE
	},
	{
		"NONE",
		{0,0,0,0},
		{0,0,0,0},
		DEFAULTITEM_STATUS,
		DEFAULTITEM_VALUE
	},
	{
		"min",
		{DEFAULTMENUITEM_WIDTH-(DEFAULTBUTTON_MARGIN+DEFAULTVARBUTTON_WIDTH)*2,410,DEFAULTMENUITEM_WIDTH-DEFAULTBUTTON_MARGIN*2-DEFAULTVARBUTTON_WIDTH,440},
		{DEFAULTMENUITEM_WIDTH-(DEFAULTBUTTON_MARGIN+DEFAULTVARBUTTON_WIDTH)*2,400,DEFAULTMENUITEM_WIDTH-DEFAULTBUTTON_MARGIN*2-DEFAULTVARBUTTON_WIDTH,450},
		DEFAULTITEM_STATUS,
		DEFAULTITEM_VALUE
	},
	{
		"max",
		{DEFAULTMENUITEM_WIDTH-DEFAULTBUTTON_MARGIN-DEFAULTVARBUTTON_WIDTH,410,DEFAULTMENUITEM_WIDTH-DEFAULTBUTTON_MARGIN,440},
		{DEFAULTMENUITEM_WIDTH-DEFAULTBUTTON_MARGIN-DEFAULTVARBUTTON_WIDTH,400,DEFAULTMENUITEM_WIDTH-DEFAULTBUTTON_MARGIN,450},
		DEFAULTITEM_STATUS,
		DEFAULTITEM_VALUE
	}
};

#else // below is the first release inverse button

static MISACTLMENU CtlItems[CTL_MAX]=
{
	{
		"none",
		{DEFAULTITEM_X_BEGIN,DEFAULTITEM_Y_BEGIN,DEFAULTITEM_X_END,DEFAULTITEM_Y_END},
		{DEFAULTBUTTON_MARGIN,DEFAULTITEM_Y_BEGIN+DEFAULTBUTTON_MARGIN,DEFAULTINVBUTTON_WIDTH+DEFAULTBUTTON_MARGIN,DEFAULTITEM_Y_END-DEFAULTBUTTON_MARGIN},
		DEFAULTITEM_STATUS,
		DEFAULTITEM_VALUE
	},
	{
		"touch x",
		{DEFAULTITEM_X_BEGIN,DEFAULTITEM_Y_BEGIN+DEFAULTMENUITEM_HEIGHT,DEFAULTITEM_X_END,DEFAULTITEM_Y_END+DEFAULTMENUITEM_HEIGHT},
		{DEFAULTBUTTON_MARGIN,DEFAULTITEM_Y_BEGIN+DEFAULTBUTTON_MARGIN+DEFAULTMENUITEM_HEIGHT,DEFAULTBUTTON_MARGIN+DEFAULTINVBUTTON_WIDTH,DEFAULTITEM_Y_END+DEFAULTMENUITEM_HEIGHT-DEFAULTBUTTON_MARGIN},
		DEFAULTITEM_STATUS,
		DEFAULTITEM_VALUE
	},
	{
		"touch y",
		{DEFAULTITEM_X_BEGIN,DEFAULTITEM_Y_BEGIN+DEFAULTMENUITEM_HEIGHT*2,DEFAULTITEM_X_END,DEFAULTITEM_Y_END+DEFAULTMENUITEM_HEIGHT*2},
		{DEFAULTBUTTON_MARGIN,DEFAULTITEM_Y_BEGIN+DEFAULTBUTTON_MARGIN+DEFAULTMENUITEM_HEIGHT*2,DEFAULTBUTTON_MARGIN+DEFAULTINVBUTTON_WIDTH,DEFAULTITEM_Y_END+DEFAULTMENUITEM_HEIGHT*2-DEFAULTBUTTON_MARGIN},
		DEFAULTITEM_STATUS,
		DEFAULTITEM_VALUE
	},
	{
		"drag x",
		{DEFAULTITEM_X_BEGIN,DEFAULTITEM_Y_BEGIN+DEFAULTMENUITEM_HEIGHT*3,DEFAULTITEM_X_END,DEFAULTITEM_Y_END+DEFAULTMENUITEM_HEIGHT*3},
		{DEFAULTBUTTON_MARGIN,DEFAULTITEM_Y_BEGIN+DEFAULTBUTTON_MARGIN+DEFAULTMENUITEM_HEIGHT*3,DEFAULTBUTTON_MARGIN+DEFAULTINVBUTTON_WIDTH,DEFAULTITEM_Y_END+DEFAULTMENUITEM_HEIGHT*3-DEFAULTBUTTON_MARGIN},
		DEFAULTITEM_STATUS,
		DEFAULTITEM_VALUE
	},
	{
		"drag y",
		{DEFAULTITEM_X_BEGIN,DEFAULTITEM_Y_BEGIN+DEFAULTMENUITEM_HEIGHT*4,DEFAULTITEM_X_END,DEFAULTITEM_Y_END+DEFAULTMENUITEM_HEIGHT*4},
		{DEFAULTBUTTON_MARGIN,DEFAULTITEM_Y_BEGIN+DEFAULTBUTTON_MARGIN+DEFAULTMENUITEM_HEIGHT*4,DEFAULTBUTTON_MARGIN+DEFAULTINVBUTTON_WIDTH,DEFAULTITEM_Y_END+DEFAULTMENUITEM_HEIGHT*4-DEFAULTBUTTON_MARGIN},
		DEFAULTITEM_STATUS,
		DEFAULTITEM_VALUE
	},
	{
		"ball x",
		{DEFAULTITEM_X_BEGIN,DEFAULTITEM_Y_BEGIN+DEFAULTMENUITEM_HEIGHT*5,DEFAULTITEM_X_END,DEFAULTITEM_Y_END+DEFAULTMENUITEM_HEIGHT*5},
		{DEFAULTBUTTON_MARGIN,DEFAULTITEM_Y_BEGIN+DEFAULTBUTTON_MARGIN+DEFAULTMENUITEM_HEIGHT*5,DEFAULTBUTTON_MARGIN+DEFAULTINVBUTTON_WIDTH,DEFAULTITEM_Y_END+DEFAULTMENUITEM_HEIGHT*5-DEFAULTBUTTON_MARGIN},
		DEFAULTITEM_STATUS,
		DEFAULTITEM_VALUE
	},
	{
		"ball y",
		{DEFAULTITEM_X_BEGIN,DEFAULTITEM_Y_BEGIN+DEFAULTMENUITEM_HEIGHT*6,DEFAULTITEM_X_END,DEFAULTITEM_Y_END+DEFAULTMENUITEM_HEIGHT*6},
		{DEFAULTBUTTON_MARGIN,DEFAULTITEM_Y_BEGIN+DEFAULTBUTTON_MARGIN+DEFAULTMENUITEM_HEIGHT*6,DEFAULTBUTTON_MARGIN+DEFAULTINVBUTTON_WIDTH,DEFAULTITEM_Y_END+DEFAULTMENUITEM_HEIGHT*6-DEFAULTBUTTON_MARGIN},
		DEFAULTITEM_STATUS,
		DEFAULTITEM_VALUE
	},
	{
		"variation                   ",
		{DEFAULTITEM_X_BEGIN,DEFAULTITEM_Y_BEGIN+DEFAULTMENUITEM_HEIGHT*7,DEFAULTITEM_X_END,DEFAULTITEM_Y_END+DEFAULTMENUITEM_HEIGHT*7},
		{DEFAULTBUTTON_MARGIN,DEFAULTITEM_Y_BEGIN+DEFAULTBUTTON_MARGIN+DEFAULTMENUITEM_HEIGHT*7,DEFAULTBUTTON_MARGIN+DEFAULTINVBUTTON_WIDTH,DEFAULTITEM_Y_END+DEFAULTMENUITEM_HEIGHT*7-DEFAULTBUTTON_MARGIN},
		DEFAULTITEM_STATUS,
		DEFAULTITEM_VALUE
	},
	{"min",{DEFAULTMENUITEM_WIDTH-(DEFAULTBUTTON_MARGIN+DEFAULTVARBUTTON_WIDTH)*2,410,DEFAULTMENUITEM_WIDTH-DEFAULTBUTTON_MARGIN*2-DEFAULTVARBUTTON_WIDTH,440},{0,0,0,0},DEFAULTITEM_STATUS,DEFAULTITEM_VALUE},
	{"max",{DEFAULTMENUITEM_WIDTH-DEFAULTBUTTON_MARGIN-DEFAULTVARBUTTON_WIDTH,410,DEFAULTMENUITEM_WIDTH-DEFAULTBUTTON_MARGIN,440},{0,0,0,0},DEFAULTITEM_STATUS,DEFAULTITEM_VALUE}
};

#endif

int MISACTLMENU::inverse = 0;

#else
static MISACTLMENU CtlItems[CTL_MAX]=
{
	{"none",{DEFAULTITEM_X_BEGIN,DEFAULTITEM_Y_BEGIN,DEFAULTITEM_X_END,DEFAULTITEM_Y_END},{DEFAULTBUTTON_MARGIN,DEFAULTITEM_Y_BEGIN+DEFAULTBUTTON_MARGIN,DEFAULTINVBUTTON_WIDTH+DEFAULTBUTTON_MARGIN,DEFAULTITEM_Y_END-DEFAULTBUTTON_MARGIN},DEFAULTITEM_STATUS,DEFAULTINVERSE_STATUS,DEFAULTITEM_VALUE},
	{"touch x",{DEFAULTITEM_X_BEGIN,DEFAULTITEM_Y_BEGIN+DEFAULTMENUITEM_HEIGHT,DEFAULTITEM_X_END,DEFAULTITEM_Y_END+DEFAULTMENUITEM_HEIGHT},{DEFAULTBUTTON_MARGIN,DEFAULTITEM_Y_BEGIN+DEFAULTBUTTON_MARGIN+DEFAULTMENUITEM_HEIGHT,DEFAULTBUTTON_MARGIN+DEFAULTINVBUTTON_WIDTH,DEFAULTITEM_Y_END+DEFAULTMENUITEM_HEIGHT-DEFAULTBUTTON_MARGIN},DEFAULTITEM_STATUS,DEFAULTINVERSE_STATUS,DEFAULTITEM_VALUE},
	{"touch y",{DEFAULTITEM_X_BEGIN,DEFAULTITEM_Y_BEGIN+DEFAULTMENUITEM_HEIGHT*2,DEFAULTITEM_X_END,DEFAULTITEM_Y_END+DEFAULTMENUITEM_HEIGHT*2},{DEFAULTBUTTON_MARGIN,DEFAULTITEM_Y_BEGIN+DEFAULTBUTTON_MARGIN+DEFAULTMENUITEM_HEIGHT*2,DEFAULTBUTTON_MARGIN+DEFAULTINVBUTTON_WIDTH,DEFAULTITEM_Y_END+DEFAULTMENUITEM_HEIGHT*2-DEFAULTBUTTON_MARGIN},DEFAULTITEM_STATUS,DEFAULTINVERSE_STATUS,DEFAULTITEM_VALUE},
	{"drag x",{DEFAULTITEM_X_BEGIN,DEFAULTITEM_Y_BEGIN+DEFAULTMENUITEM_HEIGHT*3,DEFAULTITEM_X_END,DEFAULTITEM_Y_END+DEFAULTMENUITEM_HEIGHT*3},{DEFAULTBUTTON_MARGIN,DEFAULTITEM_Y_BEGIN+DEFAULTBUTTON_MARGIN+DEFAULTMENUITEM_HEIGHT*3,DEFAULTBUTTON_MARGIN+DEFAULTINVBUTTON_WIDTH,DEFAULTITEM_Y_END+DEFAULTMENUITEM_HEIGHT*3-DEFAULTBUTTON_MARGIN},DEFAULTITEM_STATUS,DEFAULTINVERSE_STATUS,DEFAULTITEM_VALUE},
	{"drag y",{DEFAULTITEM_X_BEGIN,DEFAULTITEM_Y_BEGIN+DEFAULTMENUITEM_HEIGHT*4,DEFAULTITEM_X_END,DEFAULTITEM_Y_END+DEFAULTMENUITEM_HEIGHT*4},{DEFAULTBUTTON_MARGIN,DEFAULTITEM_Y_BEGIN+DEFAULTBUTTON_MARGIN+DEFAULTMENUITEM_HEIGHT*4,DEFAULTBUTTON_MARGIN+DEFAULTINVBUTTON_WIDTH,DEFAULTITEM_Y_END+DEFAULTMENUITEM_HEIGHT*4-DEFAULTBUTTON_MARGIN},DEFAULTITEM_STATUS,DEFAULTINVERSE_STATUS,DEFAULTITEM_VALUE},
	{"ball x",{DEFAULTITEM_X_BEGIN,DEFAULTITEM_Y_BEGIN+DEFAULTMENUITEM_HEIGHT*5,DEFAULTITEM_X_END,DEFAULTITEM_Y_END+DEFAULTMENUITEM_HEIGHT*5},{DEFAULTBUTTON_MARGIN,DEFAULTITEM_Y_BEGIN+DEFAULTBUTTON_MARGIN+DEFAULTMENUITEM_HEIGHT*5,DEFAULTBUTTON_MARGIN+DEFAULTINVBUTTON_WIDTH,DEFAULTITEM_Y_END+DEFAULTMENUITEM_HEIGHT*5-DEFAULTBUTTON_MARGIN},DEFAULTITEM_STATUS,DEFAULTINVERSE_STATUS,DEFAULTITEM_VALUE},
	{"ball y",{DEFAULTITEM_X_BEGIN,DEFAULTITEM_Y_BEGIN+DEFAULTMENUITEM_HEIGHT*6,DEFAULTITEM_X_END,DEFAULTITEM_Y_END+DEFAULTMENUITEM_HEIGHT*6},{DEFAULTBUTTON_MARGIN,DEFAULTITEM_Y_BEGIN+DEFAULTBUTTON_MARGIN+DEFAULTMENUITEM_HEIGHT*6,DEFAULTBUTTON_MARGIN+DEFAULTINVBUTTON_WIDTH,DEFAULTITEM_Y_END+DEFAULTMENUITEM_HEIGHT*6-DEFAULTBUTTON_MARGIN},DEFAULTITEM_STATUS,DEFAULTINVERSE_STATUS,DEFAULTITEM_VALUE},
	{"variation        ",{DEFAULTITEM_X_BEGIN,DEFAULTITEM_Y_BEGIN+DEFAULTMENUITEM_HEIGHT*7,DEFAULTITEM_X_END,DEFAULTITEM_Y_END+DEFAULTMENUITEM_HEIGHT*7},{DEFAULTBUTTON_MARGIN,DEFAULTITEM_Y_BEGIN+DEFAULTBUTTON_MARGIN+DEFAULTMENUITEM_HEIGHT*7,DEFAULTBUTTON_MARGIN+DEFAULTINVBUTTON_WIDTH,DEFAULTITEM_Y_END+DEFAULTMENUITEM_HEIGHT*7-DEFAULTBUTTON_MARGIN},DEFAULTITEM_STATUS,DEFAULTINVERSE_STATUS,DEFAULTITEM_VALUE},
	{"min",{DEFAULTMENUITEM_WIDTH-(DEFAULTBUTTON_MARGIN+DEFAULTVARBUTTON_WIDTH)*2,410,DEFAULTMENUITEM_WIDTH-DEFAULTBUTTON_MARGIN*2-DEFAULTVARBUTTON_WIDTH,440},{0,0,0,0},DEFAULTITEM_STATUS,DEFAULTINVERSE_STATUS,DEFAULTITEM_VALUE},
	{"max",{DEFAULTMENUITEM_WIDTH-DEFAULTBUTTON_MARGIN-DEFAULTVARBUTTON_WIDTH,410,DEFAULTMENUITEM_WIDTH-DEFAULTBUTTON_MARGIN,440},{0,0,0,0},DEFAULTITEM_STATUS,DEFAULTINVERSE_STATUS,DEFAULTITEM_VALUE}
};

static MISACTLMENU CtlItems[CTL_MAX]=
{
	{"none",{1,50,398,98},{10,61,60,88},0,0,0},
	{"touch x",{1,100,398,148},{10,111,60,138},0,0,0},
	{"touch y",{1,150,398,198},{10,161,60,188},0,0,0},
	{"drag x",{1,200,398,248},{10,211,60,238},0,0,0},
	{"drag y",{1,250,398,298},{10,261,60,288},0,0,0},
	{"ball x",{1,300,398,348},{10,311,60,338},0,0,0},
	{"ball y",{1,350,398,398},{10,361,60,388},0,0,0},
	{"variation        ",{1,400,398,448},{10,411,60,438},0,0,0},
	{"min",{240,410,310,440},{0,0,0,0},0,0,0},
	{"max",{320,410,390,440},{0,0,0,0},0,0,0}
};
#endif

static const GUI_RECT VarMin={DEFAULTMENUITEM_WIDTH-(DEFAULTBUTTON_MARGIN+DEFAULTVARBUTTON_WIDTH)*2+20,100,DEFAULTMENUITEM_WIDTH-(DEFAULTBUTTON_MARGIN+DEFAULTVARBUTTON_WIDTH)*2+51,380};
static const GUI_RECT VarMax={DEFAULTMENUITEM_WIDTH-DEFAULTBUTTON_MARGIN-DEFAULTVARBUTTON_WIDTH+20,100,DEFAULTMENUITEM_WIDTH-DEFAULTBUTTON_MARGIN-DEFAULTVARBUTTON_WIDTH+51,380};

WM_HWIN MisaMenu_Create(int x, int y, int width, int height, WM_HWIN hParent, int Id, const char* label, int fontSize)
{
	MISAMENUPROP prop;
	WM_HWIN hMisaMenu = WM_CreateWindowAsChild(x,y,width,height,hParent,
		WM_CF_MEMDEV,MisaMenuProc, sizeof(MISAMENU_OBJ)-sizeof(WM_Obj));	
	memset(hCtlMenuItems,0,sizeof(hCtlMenuItems));
	hCtlMenuItems[CTL_VARMIN] = TEXT_CreateAsChild(CtlItems[CTL_VARMIN].itemRect.x0,height-40,DEFAULTVARBUTTON_WIDTH,DEFAULTVARBUTTON_HEIGHT,hMisaMenu,CTL_VARMIN_ID,WM_CF_SHOW|WM_CF_MEMDEV,"min:0",GUI_TA_HCENTER|GUI_TA_VCENTER);
	TEXT_SetFont(hCtlMenuItems[CTL_VARMIN],&GUI_Font16_ASCII);
	TEXT_SetTextColor(hCtlMenuItems[CTL_VARMIN],GUI_GRAY);
	hCtlMenuItems[CTL_VARMAX] = TEXT_CreateAsChild(CtlItems[CTL_VARMAX].itemRect.x0,height-40,DEFAULTVARBUTTON_WIDTH,DEFAULTVARBUTTON_HEIGHT,hMisaMenu,CTL_VARMAX_ID,WM_CF_SHOW|WM_CF_MEMDEV,"max:0",GUI_TA_HCENTER|GUI_TA_VCENTER);
	TEXT_SetFont(hCtlMenuItems[CTL_VARMAX],&GUI_Font16_ASCII);
	TEXT_SetTextColor(hCtlMenuItems[CTL_VARMAX],GUI_GRAY);
	CtlItems[CTL_VARMIN].status = 0;
	CtlItems[CTL_VARMAX].status = 0;
	CtlItems[CTL_VARMIN].value = 0;
	CtlItems[CTL_VARMAX].value = 0;
	memset(&prop,0,sizeof(MISAMENUPROP));
	WM_SetUserData(hMisaMenu, &prop, sizeof(MISAMENUPROP));
	//WM_SetCapture(hMisaMenu,0);
	return hMisaMenu;
}

void MisaMenu_Delete(WM_HWIN hMisaMenu)
{
	WM_DeleteWindow(hMisaMenu);
}

void MisaMenu_SetStatus(U32 index, int status,int val,int inverse)
{
	char buf[32];
	char buff[16];
	MISAMENUPROP prop;
	if(index < CTL_MAX)
	{
		CtlItems[index].status = status;
		CtlItems[index].value = val;
		CtlItems[index].inverse = inverse;
	}
	WM_GetUserData(hMisaSetControlMenu, &prop, sizeof(MISAMENUPROP));
	if(CTL_VARMIN==index)
	{
		if(prop.ShowMin)
		{
			prop.ShowMin(CtlItems[CTL_VARMIN].value,buff);
			sprintf(buf,"min:%s",buff);
		}
		else
		{
			sprintf(buf,"min:%d",val);
		}
		TEXT_SetText(hCtlMenuItems[CTL_VARMIN],buf);
	}
	if(CTL_VARMAX==index)
	{
		if(prop.ShowMax)
		{
			prop.ShowMax(CtlItems[CTL_VARMAX].value,buff);
			sprintf(buf,"max:%s",buff);
		}
		else
		{
			sprintf(buf,"max:%d",val);
		}
		TEXT_SetText(hCtlMenuItems[CTL_VARMAX],buf);
	}
}

void MisaMenu_Popup(WM_HWIN hMisaMenu,WM_HWIN hParent,WM_HWIN hSrcWin,int x, int y)
{
	MisaMenu_PopupEx(hMisaMenu,hParent,hSrcWin,x, y, 0, 0, 0, 0);
}

void MisaMenu_PopupEx(WM_HWIN hMisaMenu,WM_HWIN hParent,WM_HWIN hSrcWin,int x, int y, int min, int max, MISAPOPMENUINFOCB MinTip, MISAPOPMENUINFOCB MaxTip)
{
	char buf[32];
	char buff[16];
	int sx,sy,width,height;
	MISAMENUPROP prop;
	sx = WM_GetWindowSizeX(hMisaMenu);
	sy = WM_GetWindowSizeY(hMisaMenu);
	width = GUI_GetScreenSizeX();
	height = GUI_GetScreenSizeY();
	#if 1
	if(width-x >= sx && height-y >= sy)
	{
		WM_MoveTo(hMisaMenu, x, y);
	}
	else
	{
		if(x <= width/2 && y <= height/2)
		{
			WM_MoveTo(hMisaMenu, width-x>sx?x:width-sx, height-y>sy?y:height-sy);
		}
		else if(x > width/2 && y <= height/2)
		{
			WM_MoveTo(hMisaMenu, x-sx>0?x-sx:0, height-y>sy?y:height-sy);
		}
		else if(x <= width/2 && y > height/2)
		{
			WM_MoveTo(hMisaMenu, width-x>sx?x:width-sx, y-sy>0?y-sy:0);
		}
		else if(x > width/2 && y > height/2)
		{
			WM_MoveTo(hMisaMenu, x-sx>0?x-sx:0, y-sy>0?y-sy:0);
		}
		else
		{
			WM_MoveTo(hMisaMenu, (width-sx)/2, (height-sy)/2);
		}
	}	
	#else
	WM_MoveTo(hMisaMenu, (width-sx)/2, (height-sy)/2);
	#endif
	WM_GetUserData(hMisaMenu, &prop, sizeof(MISAMENUPROP));
	prop.hSrcWin = hSrcWin;
	prop.ShowMin = MinTip;
	prop.ShowMax = MaxTip;
	WM_SetUserData(hMisaMenu, &prop, sizeof(MISAMENUPROP));
	WM_AttachWindow(hMisaMenu, hParent);
	WM_ShowWindow(hMisaMenu);
	WM_SetCapture(hMisaMenu,0);
	for(sx=0;sx<CTL_MAX;sx++)
	{
		CtlItems[sx].status=0;
		CtlItems[sx].inverse = 0;
	}
	if(max<min)
	{
		max = min;
	}
	CtlItems[CTL_VARMIN].value=min;
	CtlItems[CTL_VARMAX].value=max;
	if(prop.ShowMin)
	{
		prop.ShowMin(CtlItems[CTL_VARMIN].value,buff);
		sprintf(buf,"min:%s",buff);
	}
	else
	{
		sprintf(buf,"min:%d",min);
	}
	TEXT_SetText(hCtlMenuItems[CTL_VARMIN],buf);
	if(prop.ShowMax)
	{
		prop.ShowMax(CtlItems[CTL_VARMAX].value,buff);
		sprintf(buf,"max:%s",buff);
	}
	else
	{
		sprintf(buf,"max:%d",max);
	}
	TEXT_SetText(hCtlMenuItems[CTL_VARMAX],buf);
	//WM_BringToTop(hMisaMenu);
}

void MisaMenu_Close(WM_HWIN hMisaMenu)
{
	MISAMENUPROP prop;
	WM_GetUserData(hMisaMenu, &prop, sizeof(MISAMENUPROP));
	prop.ShowMin = 0;
	prop.ShowMax = 0;
	WM_SetUserData(hMisaMenu, &prop, sizeof(MISAMENUPROP));
	WM_ReleaseCapture();
	WM_HideWindow(hMisaMenu);
	WM_DetachWindow(hMisaMenu);
	TEXT_SetTextColor(hCtlMenuItems[CTL_VARMIN],GUI_GRAY);
	TEXT_SetTextColor(hCtlMenuItems[CTL_VARMAX],GUI_GRAY);
	CtlItems[CTL_VARMIN].status = 0;
	CtlItems[CTL_VARMAX].status = 0;
}

static void MisaMenuProc(WM_MESSAGE* pMsg)
{
	static U8 Captured = 0;
	int i,x;//,y;
	GUI_PID_STATE* pPID_State;
	MISAMENUPROP prop;
	WM_GetUserData(pMsg->hWin, &prop, sizeof(MISAMENUPROP));
	switch (pMsg->MsgId)
	{
	case WM_PAINT:
		x = WM_GetWindowSizeX(pMsg->hWin);
		GUI_SetBkColor(GUI_GRAY);
		GUI_Clear();
		GUI_SetColor(GUI_BLACK);
		for(i=1;i<=8;i++)
		{
			switch(CtlItems[i-1].status)
			{
			case 0:
			default:
				GUI_FillRectEx(&CtlItems[i-1].itemRect);
				break;
			case 1:
				GUI_DrawBitmap(&bmMENU_SE,CtlItems[i-1].itemRect.x0,CtlItems[i-1].itemRect.y0);
				break;
			case 2:
				GUI_DrawBitmap(&bmMENU_LD,CtlItems[i-1].itemRect.x0,CtlItems[i-1].itemRect.y0);
			}
		}
		//GUI_FillRect(401, 51, 598, 448);
		GUI_SetTextMode(GUI_TEXTMODE_TRANS);
		GUI_SetColor(GUI_WHITE);
		GUI_SetFont(&GUI_Font32B_ASCII);
		GUI_DrawBitmap(&bmMENU_TL, 1, 1);
		GUI_DispStringHCenterAt("SET CONTROL",x/2,9);
		GUI_SetFont(&GUI_Font32_ASCII);
		for(i=0;i<8;i++)
		{
			GUI_DispStringInRect(CtlItems[i].name, &CtlItems[i].itemRect, GUI_TA_HCENTER|GUI_TA_VCENTER);
			if(i && CtlItems[i].status)
			{
				GUI_SetColor(GUI_GRAY);
				GUI_SetFont(&GUI_Font16_ASCII);
				if(CtlItems[i].inverse)
				{
					GUI_SetColor(GUI_YELLOW);
				}
				GUI_DrawRectEx(&CtlItems[i].inverseRect);
				GUI_DispStringInRect("inverse", &CtlItems[i].inverseRect, GUI_TA_HCENTER|GUI_TA_VCENTER);
				GUI_SetColor(GUI_WHITE);
				GUI_SetFont(&GUI_Font32_ASCII);
			}
		}
		GUI_SetColor(GUI_LIGHTBLUE/*0xC08030*/);
		if(CtlItems[CTL_VARMIN].status)
		{
			GUI_DrawRectEx(&CtlItems[CTL_VARMIN].itemRect);
			GUI_DrawBitmap(&bmMENU_VLINE,VarMin.x0+13,VarMin.y0);
			GUI_DrawBitmap(&bmMENU_CUBE,VarMin.x0,VarMax.y0+(127-CtlItems[CTL_VARMIN].value)*2);
		}
		if(CtlItems[CTL_VARMAX].status)
		{
			GUI_DrawRectEx(&CtlItems[CTL_VARMAX].itemRect);
			GUI_DrawBitmap(&bmMENU_VLINE,VarMax.x0+13,VarMax.y0);
			GUI_DrawBitmap(&bmMENU_CUBE,VarMax.x0,VarMax.y0+(127-CtlItems[CTL_VARMAX].value)*2);
		}
		break;
	case WM_TOUCH:
		if(pMsg->Data.p)
		{
			GUI_RECT selfRect,ptRect;
			pPID_State = (GUI_PID_STATE*)pMsg->Data.p;
			WM_GetClientRectEx(pMsg->hWin,&selfRect);
			ptRect.x0 = pPID_State->x;
			ptRect.y0 = pPID_State->y;
			ptRect.x1 = ptRect.x0+1;
			ptRect.y1 = ptRect.y0+1;
			if(pPID_State->Pressed)
			{
				if(CtlItems[CTL_VARMIN].status && GUI_RectsIntersect(&VarMin,&ptRect))
				{
					Captured = 1;
				}
				if(CtlItems[CTL_VARMAX].status && GUI_RectsIntersect(&VarMax,&ptRect))
				{
					Captured = 1;
				}
				if(Captured)
				{
					char buf[32];
					char buff[16];
					if(CtlItems[CTL_VARMIN].status)
					{
						if(pPID_State->y <= VarMin.y0+12)
						{
							CtlItems[CTL_VARMIN].value = 127;
						}
						else if(pPID_State->y >= VarMin.y1-12)
						{
							CtlItems[CTL_VARMIN].value = 0;
						}
						else
						{
							CtlItems[CTL_VARMIN].value = (VarMin.y1-12-pPID_State->y)/2;
						}
						if(prop.ShowMin)
						{
							prop.ShowMin(CtlItems[CTL_VARMIN].value,buff);
							sprintf(buf,"min:%s",buff);
						}
						else
						{
							sprintf(buf,"min:%d",CtlItems[CTL_VARMIN].value);
						}
						TEXT_SetText(hCtlMenuItems[CTL_VARMIN],buf);
						if(CtlItems[CTL_VARMAX].value<CtlItems[CTL_VARMIN].value)
						{
							CtlItems[CTL_VARMAX].value=CtlItems[CTL_VARMIN].value;
							if(prop.ShowMax)
							{
								prop.ShowMax(CtlItems[CTL_VARMAX].value,buff);
								sprintf(buf,"max:%s",buff);
							}
							else
							{
								sprintf(buf,"max:%d",CtlItems[CTL_VARMAX].value);
							}
							TEXT_SetText(hCtlMenuItems[CTL_VARMAX],buf);
						}
						WM_InvalidateRect(pMsg->hWin, &VarMin);
					}
					if(CtlItems[CTL_VARMAX].status)
					{
						if(pPID_State->y <= VarMax.y0+12)
						{
							CtlItems[CTL_VARMAX].value = 127;
						}
						else if(pPID_State->y >= VarMax.y1-12)
						{
							CtlItems[CTL_VARMAX].value = 0;
						}
						else
						{
							CtlItems[CTL_VARMAX].value = (VarMax.y1-12-pPID_State->y)/2;
						}
						if(prop.ShowMax)
						{
							prop.ShowMax(CtlItems[CTL_VARMAX].value,buff);
							sprintf(buf,"max:%s",buff);
						}
						else
						{
							sprintf(buf,"max:%d",CtlItems[CTL_VARMAX].value);
						}
						TEXT_SetText(hCtlMenuItems[CTL_VARMAX],buf);
						if(CtlItems[CTL_VARMAX].value<CtlItems[CTL_VARMIN].value)
						{
							CtlItems[CTL_VARMIN].value=CtlItems[CTL_VARMAX].value;
							if(prop.ShowMin)
							{
								prop.ShowMin(CtlItems[CTL_VARMIN].value,buff);
								sprintf(buf,"min:%s",buff);
							}
							else
							{
								sprintf(buf,"min:%d",CtlItems[CTL_VARMIN].value);
							}
							TEXT_SetText(hCtlMenuItems[CTL_VARMIN],buf);
						}
						WM_InvalidateRect(pMsg->hWin, &VarMax);
					}
				}
				/*else
				{
					if(CtlItems[CTL_VARMIN].status && GUI_RectsIntersect(&VarMin,&ptRect))
					{
						Captured = 1;
					}
					if(CtlItems[CTL_VARMAX].status && GUI_RectsIntersect(&VarMax,&ptRect))
					{
						Captured = 1;
					}
				}*/
			}
			else
			{
				if(!GUI_RectsIntersect(&selfRect,&ptRect))
				{
					Captured = 0;
					MisaMenu_Close(pMsg->hWin);
				}
				else
				{
					if(Captured)
					{
						Captured = 0;
						break;
					}
					if(CtlItems[CTL_VARMIN].status && GUI_RectsIntersect(&VarMin,&ptRect))
					{
						break;
					}
					if(CtlItems[CTL_VARMAX].status && GUI_RectsIntersect(&VarMax,&ptRect))
					{
						break;
					}
					for(i=0;i<8;i++)
					{
						switch(CtlItems[i].status)
						{
						case 0:
							if(GUI_RectsIntersect(&CtlItems[i].itemRect,&ptRect))
							{
								CtlItems[i].status = 1;
								if(7 == i)
								{
									TEXT_SetTextColor(hCtlMenuItems[CTL_VARMIN],GUI_WHITE);
									TEXT_SetTextColor(hCtlMenuItems[CTL_VARMAX],GUI_WHITE);
								}
								WM_InvalidateRect(pMsg->hWin, &CtlItems[i].itemRect);
							}
							break;
						case 1:
							if(GUI_RectsIntersect(&CtlItems[i].itemRect,&ptRect))
							{
								//if(i && GUI_RectsIntersect(&CtlItems[i].inverseRect,&ptRect))
								if(i && ptRect.x1 < CtlItems[i].inverseRect.x1+10)
								{
									CtlItems[i].inverse = !CtlItems[i].inverse;
								}
								else if(7 == i)
								{
									if(GUI_RectsIntersect(&CtlItems[CTL_VARMIN].inverseRect,&ptRect))
									{
										CtlItems[CTL_VARMIN].status = 1;
										CtlItems[CTL_VARMAX].status = 0;
										TEXT_SetTextColor(hCtlMenuItems[CTL_VARMIN],GUI_LIGHTBLUE/*0xC08030*/);
										TEXT_SetTextColor(hCtlMenuItems[CTL_VARMAX],GUI_WHITE);
									}
									else if(GUI_RectsIntersect(&CtlItems[CTL_VARMAX].inverseRect,&ptRect))
									{
										CtlItems[CTL_VARMIN].status = 0;
										CtlItems[CTL_VARMAX].status = 1;
										TEXT_SetTextColor(hCtlMenuItems[CTL_VARMAX],GUI_LIGHTBLUE/*0xC08030*/);
										TEXT_SetTextColor(hCtlMenuItems[CTL_VARMIN],GUI_WHITE);
									}
									else
									{
										CtlItems[i].status = 2;
									}
								}
								else
								{
									CtlItems[i].status = 2;
								}
							}
							else
							{
								CtlItems[i].status = 0;
								//CtlItems[i].inverse = 0;
								if(7 == i)
								{
									TEXT_SetTextColor(hCtlMenuItems[CTL_VARMIN],GUI_GRAY);
									TEXT_SetTextColor(hCtlMenuItems[CTL_VARMAX],GUI_GRAY);
									CtlItems[CTL_VARMIN].status = 0;
									CtlItems[CTL_VARMAX].status = 0;
								}
							}
							//WM_InvalidateRect(pMsg->hWin, &CtlItems[i].itemRect);
							WM_InvalidateWindow(pMsg->hWin);
							if(2 == CtlItems[i].status)
							{
								WM_DisableWindow(pMsg->hWin);
								prop.Id = CTL_NONE_ID+i;
								WM_SetUserData(pMsg->hWin, &prop, sizeof(MISAMENUPROP));
								WM_NotifyParent(pMsg->hWin, WM_NOTIFICATION_RELEASED);
								Misa_Delay(300);
								MisaMenu_Close(pMsg->hWin);
								CtlItems[i].status = 0;
								WM_EnableWindow(pMsg->hWin);
								break;
							}
							break;
						case 2:
							if(GUI_RectsIntersect(&CtlItems[i].itemRect,&ptRect))
							{
								CtlItems[i].status = 1;
								WM_InvalidateRect(pMsg->hWin, &CtlItems[i].itemRect);
							}
							else
							{
								CtlItems[i].status = 0;
								WM_InvalidateRect(pMsg->hWin, &CtlItems[i].itemRect);
							}
							if(2 == CtlItems[i].status)
							{
								WM_DisableWindow(pMsg->hWin);
								prop.Id = CTL_NONE_ID+i;
								WM_SetUserData(pMsg->hWin, &prop, sizeof(MISAMENUPROP));
								WM_NotifyParent(pMsg->hWin, WM_NOTIFICATION_RELEASED);
								Misa_Delay(300);
								MisaMenu_Close(pMsg->hWin);
								CtlItems[i].status = 0;
								WM_EnableWindow(pMsg->hWin);
								break;
							}
						}
					}
				}
			}
		}
		break;
	case WM_GET_ID:
		pMsg->Data.v = prop.Id;
		break;
	case WM_CUST_GET_MISAMENUSRCID:
		pMsg->Data.v = prop.hSrcWin;
		break;
	case WM_CUST_GET_MISAMENUVARMIN:
		pMsg->Data.v = CtlItems[CTL_VARMIN].value;
		break;
	case WM_CUST_GET_MISAMENUVARMAX:
		pMsg->Data.v = CtlItems[CTL_VARMAX].value;
		break;
	default:
		WM_DefaultProc(pMsg);
	}
}

U8 MisaSetcontrolmenu_Create()
{
	hMisaSetControlMenu = MisaMenu_Create(0,0,DEFAULTMENUITEM_WIDTH,DEFAULTMENUITEM_HEIGHT*MENUITEMS,WM_GetDesktopWindow(),0,"Menu",0);
	return 0;
}

U8 MisaSetcontrolmenu_Delete()
{
	if(hMisaSetControlMenu)
	{
		MisaMenu_Delete(hMisaSetControlMenu);
	}
	return 0;
}

U8 MisaSetcontrolmenu_Popup(WM_HWIN hParent,WM_HWIN hSrcWin,int x,int y)
{
	MisaMenu_Popup(hMisaSetControlMenu,hParent,hSrcWin,x,y);
	return 0;
}

U8 MisaSetcontrolmenu_PopupEx(WM_HWIN hParent,WM_HWIN hSrcWin,int x, int y, int min, int max, MISAPOPMENUINFOCB MinTip, MISAPOPMENUINFOCB MaxTip)
{
	MisaMenu_PopupEx(hMisaSetControlMenu,hParent,hSrcWin,x,y,min,max,MinTip,MaxTip);
	return 0;
}

WM_HWIN MisaSetcontrolmenu_GetSrcWin()
{
  WM_MESSAGE Msg;
  Msg.MsgId  = WM_CUST_GET_MISAMENUSRCID;
  WM_SendMessage(hMisaSetControlMenu, &Msg);
  return Msg.Data.v;
}

int MisaSetcontrolmenu_GetVarMin()
{
  WM_MESSAGE Msg;
  Msg.MsgId  = WM_CUST_GET_MISAMENUVARMIN;
  WM_SendMessage(hMisaSetControlMenu, &Msg);
  return Msg.Data.v;
}

int MisaSetcontrolmenu_GetVarMax()
{
  WM_MESSAGE Msg;
  Msg.MsgId  = WM_CUST_GET_MISAMENUVARMAX;
  WM_SendMessage(hMisaSetControlMenu, &Msg);
  return Msg.Data.v;
}

int MisaSetcontrolmenu_GetInverse()
{
	return MISACTLMENU::inverse;
}

///////////////////////////////////////////////////////////////////////////////
// menu case
#if 0
case CTL_NONE_ID:
	break;
case CTL_TOUCH_X_ID:
	break;
case CTL_TOUCH_Y_ID:
	break;
case CTL_DRAG_X_ID:
	break;
case CTL_DRAG_Y_ID:
	break;
case CTL_BALL_X_ID:
	break;
case CTL_BALL_Y_ID:
	break;
case CTL_VARIATION_ID:
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

case WM_NOTIFICATION_SEL_CHANGED:
	x = WM_GetWindowOrgX(pMsg->hWinSrc)+50;
	y = WM_GetWindowOrgY(pMsg->hWinSrc)+340;
	MisaSetcontrolmenu_Popup(hxxxMisaxxx,pMsg->hWinSrc,x,y);
#endif

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa Synth string panel
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#define STRINGPANEL_MODE_SYNTH 0
#define STRINGPANEL_MODE_MIDI 1

#define STRINGPANEL_SLA_ID 0xF001
#define STRINGPANEL_SRA_ID 0xF002
#define STRINGPANEL_SYNTH_ID 0xF003
#define STRINGPANEL_MIDI_ID 0xF004
#define STRINGPANEL_MLA_ID 0xF005
#define STRINGPANEL_MRA_ID 0xF006

#define STRING_COMBO_WIDTH 201
#define STRING_COMBO_HEIGHT 70
#define STRING_COMBO_BTWIDTH 62
#define STRING_COMBO_CTWIDTH 77

#define STRING_SWITCH_WIDTH 226
#define STRING_SWITCH_HEIGHT 72
#define STRING_SWITCH_BTWIDTH 113

typedef struct __MISASTRING
{
	U8 tune;
	U32 Voice;
	U8 MIDIchanel;
	U8 Synth_MIDI;
	PVOICE_INFO aVoicept[5];
	MISACOMBO rectTuneButton;
	MISASWITCH rectSwitch;
	MISACOMBO rectMIDIButton;
} MISASTRING,*PMISASTRING;

///////////////////////////////////////////////////////////////////////////////
// Combo button
///////////////////////////////////////////////////////////////////////////////

static const char* strTune[]=
{
	"C",
	"C#",
	"D",
	"D#",
	"E",
	"F",
	"F#",
	"G",
	"G#",
	"A",
	"A#",
	"B",
};

static void InitSynthCombo(PMISACOMBO pItem,int x, int y)
{
	// whole body
	pItem->body.x0 = x;
	pItem->body.y0 = y;
	pItem->body.x1 = x+STRING_COMBO_WIDTH;
	pItem->body.y1 = y+STRING_COMBO_HEIGHT;
	// left button
	pItem->leftbt.x0 = x;
	pItem->leftbt.y0 = y;
	pItem->leftbt.x1 = x+STRING_COMBO_BTWIDTH;
	pItem->leftbt.y1 = y+STRING_COMBO_HEIGHT;
	// centre part
	pItem->centre.x0 = x+STRING_COMBO_BTWIDTH;
	pItem->centre.y0 = y;
	pItem->centre.x1 = x+STRING_COMBO_BTWIDTH+STRING_COMBO_CTWIDTH;
	pItem->centre.y1 = y+STRING_COMBO_HEIGHT;
	// right button
	pItem->rightbt.x0 = x+STRING_COMBO_BTWIDTH+STRING_COMBO_CTWIDTH;
	pItem->rightbt.y0 = y;
	pItem->rightbt.x1 = x+STRING_COMBO_BTWIDTH*2+STRING_COMBO_CTWIDTH;
	pItem->rightbt.y1 = y+STRING_COMBO_HEIGHT;
}

static void DrawSynthCombo(PMISASTRING pItem)
{
	int textmode;
	char buf[16];
	const GUI_FONT* font;
	GUI_DrawBitmap(&bmSM_SCENTRE, pItem->rectTuneButton.centre.x0, pItem->rectTuneButton.centre.y0);
	textmode = GUI_GetTextMode();
	GUI_SetTextMode(GUI_TEXTMODE_TRANS);
	font = GUI_GetFont();
	GUI_SetFont(&GUI_Font24B_ASCII);
	sprintf(buf,"%s  %d",strTune[pItem->tune%12],pItem->tune/12-1);
	GUI_DispStringInRect(buf,&pItem->rectTuneButton.centre,GUI_TA_HCENTER|GUI_TA_VCENTER);
	GUI_SetFont(font);
	GUI_SetTextMode(textmode);
}

///////////////////////////////////////////////////////////////////////////////
// Switch button
///////////////////////////////////////////////////////////////////////////////

static void InitSynthSwitch(PMISASWITCH pItem,int x, int y)
{
	x = STRING_PANEL_WIDTH-STRING_SWITCH_WIDTH;
	// whole body
	pItem->body.x0 = x;
	pItem->body.y0 = y;
	pItem->body.x1 = x+STRING_SWITCH_WIDTH;
	pItem->body.y1 = y+STRING_SWITCH_HEIGHT;
	// left button
	pItem->leftbt.x0 = x;
	pItem->leftbt.y0 = y;
	pItem->leftbt.x1 = x+STRING_SWITCH_BTWIDTH;
	pItem->leftbt.y1 = y+STRING_SWITCH_HEIGHT;
	// right button
	pItem->rightbt.x0 = x+STRING_SWITCH_BTWIDTH;
	pItem->rightbt.y0 = y;
	pItem->rightbt.x1 = x+STRING_SWITCH_WIDTH;
	pItem->rightbt.y1 = y+STRING_SWITCH_HEIGHT;
}

///////////////////////////////////////////////////////////////////////////////
// Voice control
///////////////////////////////////////////////////////////////////////////////

#define STRINGPANEL_DRAWVOICE_START 201
#define STRINGPANEL_DRAWVOICE_WIDTH 73
#define STRINGPANEL_DRAWVOICE_HEIGHT 73

static const GUI_RECT rectVoiceArea=
{
	STRINGPANEL_DRAWVOICE_START,
	0,
	STRINGPANEL_DRAWVOICE_START+STRINGPANEL_DRAWVOICE_WIDTH*5,
	STRINGPANEL_DRAWVOICE_HEIGHT
};

static const GUI_RECT rectVoice[5]=
{
	{STRINGPANEL_DRAWVOICE_START,0,STRINGPANEL_DRAWVOICE_START+STRINGPANEL_DRAWVOICE_WIDTH,STRINGPANEL_DRAWVOICE_HEIGHT},
	{STRINGPANEL_DRAWVOICE_START+STRINGPANEL_DRAWVOICE_WIDTH,0,STRINGPANEL_DRAWVOICE_START+STRINGPANEL_DRAWVOICE_WIDTH*2,STRINGPANEL_DRAWVOICE_HEIGHT},
	{STRINGPANEL_DRAWVOICE_START+STRINGPANEL_DRAWVOICE_WIDTH*2,0,STRINGPANEL_DRAWVOICE_START+STRINGPANEL_DRAWVOICE_WIDTH*3,STRINGPANEL_DRAWVOICE_HEIGHT},
	{STRINGPANEL_DRAWVOICE_START+STRINGPANEL_DRAWVOICE_WIDTH*3,0,STRINGPANEL_DRAWVOICE_START+STRINGPANEL_DRAWVOICE_WIDTH*4,STRINGPANEL_DRAWVOICE_HEIGHT},
	{STRINGPANEL_DRAWVOICE_START+STRINGPANEL_DRAWVOICE_WIDTH*4,0,STRINGPANEL_DRAWVOICE_START+STRINGPANEL_DRAWVOICE_WIDTH*5,STRINGPANEL_DRAWVOICE_HEIGHT}
};

static void InitSynthoice(PMISASTRING string)
{
	string->Voice = 0;
}

static U8 AddSynthoice(PMISASTRING string)
{
	int i;
	U8 Ret;
	U32 mask;
	Ret = 0;
	mask = 0x00000001;
	for(i=0;i<5;i++)
	{
		if(!(string->Voice & mask))
		{
			string->Voice |= mask;
			Ret = 1;
			break;
		}
		mask <<= 2;
	}
	return Ret;
}

static U8 DelSynthoice(PMISASTRING string)
{
	int i;
	U8 Ret;
	U32 mask;
	Ret = 0;
	mask = 0x00000300;
	for(i=0;i<5;i++)
	{
		if(string->Voice & mask)
		{
			string->Voice &= ~mask;
			Ret = 1;
			break;
		}
		mask >>= 2;
	}
	return Ret;
}

static U8 SelSynthoice(PMISASTRING string,U8 index,U8 sel)
{
	U8 Ret;
	U32 mask;
	Ret = 0;
	mask = 0x00000001;
	mask <<= index*2;
	if(string->Voice&mask)
	{
		mask <<= 1;
		if(sel)
		{
			string->Voice |= mask;
		}
		else
		{
			string->Voice &= ~mask;
		}
		Ret = 1;
	}
	else
	{
		Ret = 0;
	}
	return Ret;
}

static U8 GetAvailableSynthoice(PMISASTRING string)
{
	U32 mask,index;
	mask = 0x00000001;
	for(index=0;index<5;index++)
	{
		if(string->Voice&mask)
		{
			break;
		}
		mask <<= index*2;
	}
	return index;
}

static U8 GetFirstUnavailableSynthoice(PMISASTRING string)
{
	U32 mask,index;
	mask = 0x00000001;
	for(index=0;index<5;index++)
	{
		if(!(string->Voice&mask))
		{
			break;
		}
		mask <<= 2;
	}
	return index;
}
static U8 GetSelSynthoice(PMISASTRING string,U8 index)
{
	U32 mask;
	mask = 0x00000002;
	mask <<= index*2;
	return string->Voice&mask?1:0;
}

static void DrawSynthVoice(U32 voice)
{
	int i;
	U32 mask;
	mask = 0x00000001;
	for(i=0;i<5;i++)
	{
		if(voice & mask)
		{
			mask <<= 1;
			if(voice & mask)
			{
				GUI_DrawBitmap(&bmSM_WF_SE, rectVoice[i].x0, rectVoice[i].y0);
			}
			else
			{
				GUI_DrawBitmap(&bmSM_WF_UN, rectVoice[i].x0, rectVoice[i].y0);
			}
			mask <<= 1;
		}
		else
		{
			GUI_DrawBitmap(&bmSM_WF_OP, rectVoice[i].x0, rectVoice[i].y0);
			break;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
// MIDI control
///////////////////////////////////////////////////////////////////////////////

#define STRINGPANEL_MIDICOMBO_START 201
#define STRINGPANEL_MIDICOMBO_BODY 365
#define STRINGPANEL_MIDICOMBO_WIDTH 62
#define STRINGPANEL_MIDICOMBO_HEIGHT 72
#define STRINGPANEL_MIDICOMBO_CENTRE 241

static void InitMIDICombo(PMISACOMBO pItem,int x, int y)
{
	// whole body
	pItem->body.x0 = x+STRINGPANEL_MIDICOMBO_START;
	pItem->body.y0 = y;
	pItem->body.x1 = x+STRINGPANEL_MIDICOMBO_START+STRINGPANEL_MIDICOMBO_BODY;
	pItem->body.y1 = y+STRINGPANEL_MIDICOMBO_HEIGHT;
	// left button
	pItem->leftbt.x0 = x+STRINGPANEL_MIDICOMBO_START;
	pItem->leftbt.y0 = y;
	pItem->leftbt.x1 = x+STRINGPANEL_MIDICOMBO_START+STRINGPANEL_MIDICOMBO_WIDTH;
	pItem->leftbt.y1 = y+STRINGPANEL_MIDICOMBO_HEIGHT;
	// centre part
	pItem->centre.x0 = x+STRINGPANEL_MIDICOMBO_START+STRINGPANEL_MIDICOMBO_WIDTH;
	pItem->centre.y0 = y+2;
	pItem->centre.x1 = x+STRINGPANEL_MIDICOMBO_START+STRINGPANEL_MIDICOMBO_WIDTH+STRINGPANEL_MIDICOMBO_CENTRE;
	pItem->centre.y1 = y+STRINGPANEL_MIDICOMBO_HEIGHT-1;
	// right button
	pItem->rightbt.x0 = x+STRINGPANEL_MIDICOMBO_START+STRINGPANEL_MIDICOMBO_WIDTH+STRINGPANEL_MIDICOMBO_CENTRE;
	pItem->rightbt.y0 = y;
	pItem->rightbt.x1 = x+STRINGPANEL_MIDICOMBO_START+STRINGPANEL_MIDICOMBO_WIDTH*2+STRINGPANEL_MIDICOMBO_CENTRE;
	pItem->rightbt.y1 = y+STRINGPANEL_MIDICOMBO_HEIGHT;
}

static void DrawSynthMIDI(PMISASTRING pItem)
{
	int textmode;
	char buf[24];
	const GUI_FONT* font;
	GUI_DrawBitmap(&bmSM_MCENTRE, pItem->rectMIDIButton.centre.x0, pItem->rectMIDIButton.centre.y0);
	textmode = GUI_GetTextMode();
	GUI_SetTextMode(GUI_TEXTMODE_TRANS);
	font = GUI_GetFont();
	GUI_SetFont(&GUI_Font24B_ASCII);
	sprintf(buf,"MIDI Channel %d",pItem->MIDIchanel+1);
	GUI_DispStringInRect(buf,&pItem->rectMIDIButton.centre,GUI_TA_HCENTER|GUI_TA_VCENTER);
	GUI_SetFont(font);
	GUI_SetTextMode(textmode);
}

///////////////////////////////////////////////////////////////////////////////
// Layout
///////////////////////////////////////////////////////////////////////////////

typedef struct __MISASYNTHSTRING
{
	int Id;
	BUTTON_Handle sla;
	BUTTON_Handle sra;
	BUTTON_Handle mla;
	BUTTON_Handle mra;
	BUTTON_Handle synth;
	BUTTON_Handle midi;
	MISASTRING string;
} MISASYNTHSTRING,*LPMISASYNTHSTRING;

typedef struct __MISASTRING_OBJ
{
	WM_Obj Win;
	MISASYNTHSTRING progprop;
} MISASTRING_OBJ;

static WM_CALLBACK* DefaultSynthStringProc;
static void MisaSynthStringProc(WM_MESSAGE* pMsg);
static void MisaSynthStringOntouch(WM_MESSAGE*pMsg, LPMISASYNTHSTRING pProp);	// OR : Drag & Drop

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa Synth string panel
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

WM_HWIN MisaSynthString_Create(int x, int y, int width, int height, WM_HWIN hParent, int Id)
{
	int i;
	MISASYNTHSTRING prop;
	WM_HWIN hMisaSynthString = WM_CreateWindowAsChild(x,y,STRING_PANEL_WIDTH,STRING_PANEL_HEIGHT,hParent,
		WM_CF_SHOW|WM_CF_MEMDEV,MisaSynthStringProc, sizeof(MISASTRING_OBJ)-sizeof(WM_Obj));
	WM_GetUserData(hMisaSynthString, &prop, sizeof(MISASYNTHSTRING));
	prop.Id = Id;
	i=sizeof(VOICE_INFO);
	for(i=0;i<5;i++)
	{
		prop.string.aVoicept[i] = (PVOICE_INFO)malloc(sizeof(VOICE_INFO));
	}
	WM_SetUserData(hMisaSynthString, &prop, sizeof(MISASYNTHSTRING));
	return hMisaSynthString;
}

void MisaSynthString_CreateItems(WM_HWIN hMisaSynthString)
{
	MISASYNTHSTRING prop;
	WM_GetUserData(hMisaSynthString, &prop, sizeof(MISASYNTHSTRING));
	// Combo
	InitSynthCombo(&prop.string.rectTuneButton, 0, 2);
	prop.sla = BUTTON_CreateAsChild(prop.string.rectTuneButton.leftbt.x0,prop.string.rectTuneButton.leftbt.y0,
		bmSM_SLA_UN.XSize,bmSM_SLA_UN.YSize,hMisaSynthString,STRINGPANEL_SLA_ID,WM_CF_SHOW|WM_CF_MEMDEV);
	BUTTON_SetFocussable(prop.sla,0);
	BUTTON_SetBitmap(prop.sla,BUTTON_CI_UNPRESSED,&bmSM_SLA_UN);
	BUTTON_SetBitmap(prop.sla,BUTTON_CI_PRESSED,&bmSM_SLA_SE);
	prop.sra = BUTTON_CreateAsChild(prop.string.rectTuneButton.rightbt.x0,prop.string.rectTuneButton.rightbt.y0,
		bmSM_SRA_UN.XSize,bmSM_SRA_UN.YSize,hMisaSynthString,STRINGPANEL_SRA_ID,WM_CF_SHOW|WM_CF_MEMDEV);
	BUTTON_SetFocussable(prop.sra,0);
	BUTTON_SetBitmap(prop.sra,BUTTON_CI_UNPRESSED,&bmSM_SRA_UN);
	BUTTON_SetBitmap(prop.sra,BUTTON_CI_PRESSED,&bmSM_SRA_SE);
	// Switch
	InitSynthSwitch(&prop.string.rectSwitch, 0, 0);
	prop.synth = MisaRadio_Create(prop.string.rectSwitch.leftbt.x0,prop.string.rectSwitch.leftbt.y0,
		bmSM_SYNTH_UN.XSize,bmSM_SYNTH_UN.YSize,hMisaSynthString,STRINGPANEL_SYNTH_ID,WM_CF_SHOW|WM_CF_MEMDEV,
		&bmSM_SYNTH_UN,&bmSM_SYNTH_SE);
	prop.midi = MisaRadio_Create(prop.string.rectSwitch.rightbt.x0,prop.string.rectSwitch.rightbt.y0,
		bmSM_MIDI_UN.XSize,bmSM_MIDI_UN.YSize,hMisaSynthString,STRINGPANEL_MIDI_ID,WM_CF_SHOW|WM_CF_MEMDEV,
		&bmSM_MIDI_UN,&bmSM_MIDI_SE);
	MisaRadio_SetStatus(prop.synth,1);
	MisaRadio_SetStatus(prop.midi,0);
	// Voice
	InitSynthoice(&prop.string);
	// MIDI
	InitMIDICombo(&prop.string.rectMIDIButton, 0, 0);
	prop.mla = BUTTON_CreateAsChild(prop.string.rectMIDIButton.leftbt.x0,prop.string.rectMIDIButton.leftbt.y0,
		bmSM_MLA_UN.XSize,bmSM_MLA_UN.YSize,hMisaSynthString,STRINGPANEL_MLA_ID,WM_CF_MEMDEV);
	BUTTON_SetFocussable(prop.mla,0);
	BUTTON_SetBitmap(prop.mla,BUTTON_CI_UNPRESSED,&bmSM_MLA_UN);
	BUTTON_SetBitmap(prop.mla,BUTTON_CI_PRESSED,&bmSM_MLA_SE);
	prop.mra = BUTTON_CreateAsChild(prop.string.rectMIDIButton.rightbt.x0,prop.string.rectMIDIButton.rightbt.y0,
		bmSM_MRA_UN.XSize,bmSM_MRA_UN.YSize,hMisaSynthString,STRINGPANEL_MRA_ID,WM_CF_MEMDEV);
	BUTTON_SetFocussable(prop.mra,0);
	BUTTON_SetBitmap(prop.mra,BUTTON_CI_UNPRESSED,&bmSM_MRA_UN);
	BUTTON_SetBitmap(prop.mra,BUTTON_CI_PRESSED,&bmSM_MRA_SE);
	WM_SetUserData(hMisaSynthString, &prop, sizeof(MISASYNTHSTRING));
}

void MisaSynthString_DeleteItems(WM_HWIN hMisaSynthString)
{
	int i;
	MISASYNTHSTRING prop;
	if(WM_IsWindow(hMisaSynthString))
	{
		WM_GetUserData(hMisaSynthString, &prop, sizeof(MISASYNTHSTRING));
		BUTTON_Delete(prop.sla);
		BUTTON_Delete(prop.sra);
		BUTTON_Delete(prop.mla);
		BUTTON_Delete(prop.mra);
		BUTTON_Delete(prop.synth);
		BUTTON_Delete(prop.midi);
	}
	for(i=0;i<5;i++)
	{
		free(prop.string.aVoicept[i]);
	}
}

void MisaSynthString_Delete(WM_HWIN hMisaSynthString)
{
	MisaSynthString_DeleteItems(hMisaSynthString);
	WM_DeleteWindow(hMisaSynthString);
}

void MisaSynthString_SetTune(WM_HWIN hMisaSynthString, U8 tune)
{
	MISASYNTHSTRING prop;
	WM_GetUserData(hMisaSynthString, &prop, sizeof(MISASYNTHSTRING));
	prop.string.tune = tune;
	WM_SetUserData(hMisaSynthString, &prop, sizeof(MISASYNTHSTRING));
	WM_InvalidateRect(hMisaSynthString,&prop.string.rectTuneButton.centre);
}

U8 MisaSynthString_GetTune(WM_HWIN hMisaSynthString)
{
	MISASYNTHSTRING prop;
	WM_GetUserData(hMisaSynthString, &prop, sizeof(MISASYNTHSTRING));
	return prop.string.tune;
}

U8 MisaSynthString_GetChannel(WM_HWIN hMisaSynthString)
{
	MISASYNTHSTRING prop;
	WM_GetUserData(hMisaSynthString, &prop, sizeof(MISASYNTHSTRING));
	return prop.string.MIDIchanel;
}

U8 MisaSynthString_DelSynthoice(WM_HWIN hMisaSynthString)
{
	U32 i,index = 0;
	MISASYNTHSTRING prop;
	WM_GetUserData(hMisaSynthString, &prop, sizeof(MISASYNTHSTRING));
	index = prop.string.Voice>>16;
	if(index<5)
	{
		DelSynthoice(&prop.string);
		for(i=index;i<4;i++)
		{
			if(prop.string.aVoicept[i+1]->available)
			{
				*prop.string.aVoicept[i] = *prop.string.aVoicept[i+1];
			}
			else
			{
				break;
			}
		}
		memset(prop.string.aVoicept[GetFirstUnavailableSynthoice(&prop.string)],0,sizeof(VOICE_INFO));
	}
	else
	{
		// Should never get here
		prop.string.Voice = 0;
		for(i=0;i<5;i++)
		{
			memset(prop.string.aVoicept[index],0,sizeof(VOICE_INFO));
		}
	}
	WM_SetUserData(hMisaSynthString, &prop, sizeof(MISASYNTHSTRING));
	return 0;
}

U32 MisaSynthString_GetVoiceIndex(WM_HWIN hMisaSynthString)
{
	U32 index = 0;
	MISASYNTHSTRING prop;
	WM_GetUserData(hMisaSynthString, &prop, sizeof(MISASYNTHSTRING));
	index = prop.string.Voice>>16;
	return index;
}

U8 MisaSynthString_SetVoice(WM_HWIN hMisaSynthString,int voice)
{
	U32 index = 0;
	MISASYNTHSTRING prop;
	WM_GetUserData(hMisaSynthString, &prop, sizeof(MISASYNTHSTRING));
	prop.string.Voice = voice;
	WM_SetUserData(hMisaSynthString, &prop, sizeof(MISASYNTHSTRING));
	return 0;
}

void* MisaSynthString_GetVoice(WM_HWIN hMisaSynthString,int index)
{
	MISASYNTHSTRING prop;
	PVOICE_INFO pVoice;
	pVoice = 0;
	WM_GetUserData(hMisaSynthString, &prop, sizeof(MISASYNTHSTRING));
	if(index<5)
	{
		pVoice = prop.string.aVoicept[index];
	}
	return (void*)pVoice;
}

U8 MisaSynthString_SwitchSynthMode(WM_HWIN hMisaSynthString,U8 mode, U8 channel)
{
	MISASYNTHSTRING prop;
	WM_GetUserData(hMisaSynthString, &prop, sizeof(MISASYNTHSTRING));
	switch(mode)
	{
	case STRINGPANEL_MODE_SYNTH:
	default:
		WM_HideWindow(prop.mla);
		WM_HideWindow(prop.mra);
		MisaRadio_SetStatus(prop.midi,0);
		MisaRadio_SetStatus(prop.synth,1);
		break;
	case STRINGPANEL_MODE_MIDI:
		WM_ShowWindow(prop.mla);
		WM_ShowWindow(prop.mra);
		MisaRadio_SetStatus(prop.synth,0);
		MisaRadio_SetStatus(prop.midi,1);
		prop.string.MIDIchanel = channel;
	}
	SynthTurnNotesOff();
	prop.string.Synth_MIDI = mode;
	WM_SetUserData(hMisaSynthString, &prop, sizeof(MISASYNTHSTRING));
	return 0;
}

static void SwitchSynthMode(U8 mode,LPMISASYNTHSTRING pProp)
{
	switch(mode)
	{
	case STRINGPANEL_MODE_SYNTH:
	default:
		WM_HideWindow(pProp->mla);
		WM_HideWindow(pProp->mra);
		break;
	case STRINGPANEL_MODE_MIDI:
		WM_ShowWindow(pProp->mla);
		WM_ShowWindow(pProp->mra);
	}
	pProp->string.Synth_MIDI = mode;
}


// OR : Drag & Drop
static void DragAndDropWindowProc(WM_MESSAGE* pMsg)
{
	GUI_PID_STATE* pState = (GUI_PID_STATE*)pMsg->Data.p;
	WM_HWIN hWinString = 0;

	switch (pMsg->MsgId)
	{
		MISASYNTHSTRING prop;

	case WM_PAINT:
		if (Drag.Moving())
			GUI_DrawBitmap(&bmSM_DRAG, 0 , 0);
		break;

	case WM_TOUCH:
		//WM_SendMessage(

		if (pState)
		{
			GUI_RECT rect;
			WM_GetWindowRectEx(pMsg->hWin, &rect);

			// update the coordinates, relatively to the parent
			pState->x  += rect.x0;
			pState->y += rect.y0;

			hWinString = SynthScreen_GetStringHWin(pState->x, pState->y);
			if (hWinString)
			{
				// the dragged box is over a string window (hWinString)
				// the coordinates pointed by pState are now relative to String window..
				WM_GetUserData(hWinString, &prop, sizeof(MISASYNTHSTRING));
				pMsg->hWin = hWinString;
				MisaSynthStringOntouch(pMsg, &prop);
				return;
			}
		}

		// Here is the case if the dragged box is not over a string window
		MisaSynthStringOntouch(pMsg, NULL);
		break;

	case WM_CAPTURE_RELEASED:
		Drag.Finish();
		break;

	default:
		WM_DefaultProc(pMsg);
	};

}

void DragVoice::MoveOn(int x, int y, GUI_HWIN parent)
{
	int X = x - bmSM_DRAG.XSize / 2;
	if (x < 0)
		x = 0;

	int Y = y - bmSM_DRAG.YSize / 2;
	if (y < 0)
		y = 0;

	// clip to rectVoiceArea
	if (X < rectVoiceArea.x0)
		X = rectVoiceArea.x0;
	if (X > rectVoiceArea.x1 - bmSM_DRAG.YSize)
		X = rectVoiceArea.x1 - bmSM_DRAG.YSize;

	if (!hWinOverlay)
	{
		hWinOverlay = WM_CreateWindowAsChild(X, Y, bmSM_DRAG.XSize, bmSM_DRAG.YSize, parent, WM_CF_SHOW|WM_CF_MEMDEV, DragAndDropWindowProc, 0);
		//WM_BringToTop(hWinOverlay);
		WM_SetCapture(hWinOverlay, false);
	}



	WM_MoveTo(hWinOverlay, X, Y);
	WM_InvalidateWindow(hWinOverlay);
}
	
void DragVoice::Finish()
{
	strIdx = -1; 
	if (hWinOverlay)
	{
		WM_DetachWindow(hWinOverlay); 
		WM_DeleteWindow(hWinOverlay);
		WM_InvalidateWindow(hWinOverlay);
		hWinOverlay = 0;
	}

}

// OR : Drag & Drop
int GetTouchedVoice(const GUI_PID_STATE* pState, LPMISASYNTHSTRING pProp, bool &NewVoice)
{
	int i;
	GUI_RECT ptRect;
	ptRect.x0 = pState->x;
	ptRect.y0 = pState->y;
	ptRect.x1 = ptRect.x0+1;
	ptRect.y1 = ptRect.y0+1;
	NewVoice = true;
	i = GetFirstUnavailableSynthoice(&pProp->string);
	if(i >= 5)
	{
		NewVoice = false;
		i = 4;	// no empty voice. Check all voice boxes.

	}

	do
	{
		if(GUI_RectsIntersect(&rectVoice[i],&ptRect))
			break;
		NewVoice = false;
	}
	while(i--);

	return i;
}

// OR : Drag & Drop
static void MisaSynthStringOntouch(WM_MESSAGE*pMsg, LPMISASYNTHSTRING pProp)
{
   const GUI_PID_STATE* pState = (const GUI_PID_STATE*)pMsg->Data.p;
   if (pState)
   {
       bool NewVoice = false;
       int touchedVoice = -1;
       if(pProp)
           touchedVoice = GetTouchedVoice(pState, pProp, NewVoice);

       if(pState->Pressed)
       {
           if (Drag.IsDragged())
           {
               GUI_RECT rect;
               WM_GetWindowRectEx(pMsg->hWin, &rect);
               Drag.MoveOn(pState->x + rect.x0, pState->y + rect.y0, WM_GetParent(pMsg->hWin));
               return;
           }

           // Drag
           if (touchedVoice >= 0 && !NewVoice && !Drag.IsDragged())
           {
               // OR : Drag & Drop
               // Timer (LONG_TOUCH)
               // WM_CreateTimer(pMsg->hWin, START_DRAG_TIMER, 2, 0); /* not to be documented (may change in future version) */

               //Drag.Start(pProp->string.aVoicept[touchedVoice]);
               int str = SynthGetStringIndex(pMsg->hWin);
               Drag.Start(str, touchedVoice);
           }

       }
       else
       {
           // Drop ?
           if (Drag.IsDragged())
           {
               int SrcStringIdx = Drag.GetStringIdx();
               int SrcVoiceIdx = Drag.GetVoiceIdx();
               int str = SynthGetStringIndex(pMsg->hWin);
               if (NewVoice)
               {
                   AddSynthoice(&pProp->string);
                   SelSynthoice(&pProp->string, touchedVoice, 1);
                   SynthInsertVoice(str, 0);
                   struct voice *v = &GetCurrentSettingPtr()->voices[str].at(touchedVoice);
                   int lChannel = v->channel;    // save allocated channel
                   *v = *Drag.GetVoiceDataPtr();
                   v->channel = lChannel;        // restore allocated channel

                   // it's dangerous to rely on the ordering of the list of parameter definition and its coherency.
                   //    --> I can duplicate code for all param PARAM_VOICE_XXX (But it's heavy!!)
                   // --> we can have an enum that embeds all PARAM_VOICE_ values, finished by a PARAM_VOICE_END.
                   // I finally do the second option in MisaMenuConfig.h & Synth.h
                   for (int synth_param = PARAM_VOICE_AMP_ENV_ATTACK; synth_param < PARAM_VOICE_END; synth_param++)
                   {
                       std::vector<int> SrcEffectVect = VoiceFindEffect(synth_param, SrcStringIdx, SrcVoiceIdx);
                       std::vector<int>::iterator it;
                       for (it = SrcEffectVect.begin(); it != SrcEffectVect.end(); it++)
                       {
                           int entry = *it;
                           VoiceAssignEffect(entry & 0xff, synth_param, str, touchedVoice, (entry&0x100)?true:false, (entry >> 16) & 0xff, (entry >> 24) & 0xff);
                       }
                   }
                   SendVoiceParamToSynth(str, touchedVoice);

                   WM_SetUserData(pMsg->hWin, pProp, sizeof(MISASYNTHSTRING));
                   UpdateSynthSettingEx(MISAMODULE_SYNTH);
                   WM_InvalidateRect(pMsg->hWin,&rectVoiceArea);
                   Drag.Finish();
                   return;
               }

               Drag.Finish();
               if (touchedVoice != SrcVoiceIdx || str != SrcStringIdx)
                   return;
           }

           if (NewVoice)
           {
               AddSynthoice(&pProp->string);
               SelSynthoice(&pProp->string, touchedVoice, 1);
               pProp->string.Voice &= 0x0000FFFF;
               pProp->string.Voice |= touchedVoice << 16;
               pProp->string.aVoicept[touchedVoice]->available = 1;
               WM_SetUserData(pMsg->hWin, pProp, sizeof(MISASYNTHSTRING));
               WM_NotifyParent(pMsg->hWin, WM_NOTIFICATION_ADDVOICE);
           }
           else if (touchedVoice >= 0)
           {
               pProp->string.Voice &= 0x0000FFFF;
               pProp->string.Voice |= touchedVoice << 16;
               WM_SetUserData(pMsg->hWin, pProp, sizeof(MISASYNTHSTRING));
               WM_NotifyParent(pMsg->hWin, WM_NOTIFICATION_RELEASED);
           }
           WM_InvalidateRect(pMsg->hWin,&rectVoiceArea);
       }
   }
}

static void MisaSynthStringProc(WM_MESSAGE* pMsg)
{
	int NCode,Id;
	MISASYNTHSTRING prop;
	WM_GetUserData(pMsg->hWin, &prop, sizeof(MISASYNTHSTRING));
	switch (pMsg->MsgId)
	{
	case WM_CREATE:
		MisaSynthString_CreateItems(pMsg->hWin);
		break;
	case WM_PAINT:
		GUI_Clear();
		DrawSynthCombo(&prop.string);
		switch(prop.string.Synth_MIDI)
		{
		case STRINGPANEL_MODE_SYNTH:
		default:
			DrawSynthVoice(prop.string.Voice);
			break;
		case STRINGPANEL_MODE_MIDI:
			DrawSynthMIDI(&prop.string);
		}
		break;
	case WM_TOUCH:
		if(STRINGPANEL_MODE_SYNTH==prop.string.Synth_MIDI)
		{
			MisaSynthStringOntouch(pMsg,&prop);
		}
		break;

	// OR : Drag & Drop
	//case WM_TIMER:
	//	WM_DeleteTimer(pMsg->hWin, START_DRAG_TIMER); /* not to be documented (may change in future version) */
	//	break;

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
			case STRINGPANEL_SLA_ID:
				//DelSynthoice(&prop.string);
				//WM_InvalidateRect(pMsg->hWin,&rectVoiceArea);
				--prop.string.tune;
				if(127 < prop.string.tune)
				{
					prop.string.tune = 127;
				}
				WM_SetUserData(pMsg->hWin, &prop, sizeof(MISASYNTHSTRING));
				WM_InvalidateRect(pMsg->hWin,&prop.string.rectTuneButton.centre);
				WM_NotifyParent(pMsg->hWin, WM_NOTIFICATION_VALUE_CHANGED);
				break;
			case STRINGPANEL_SRA_ID:
				++prop.string.tune;
				if(127 < prop.string.tune)
				{
					prop.string.tune = 0;
				}
				WM_SetUserData(pMsg->hWin, &prop, sizeof(MISASYNTHSTRING));
				WM_InvalidateRect(pMsg->hWin,&prop.string.rectTuneButton.centre);
				WM_NotifyParent(pMsg->hWin, WM_NOTIFICATION_VALUE_CHANGED);
				break;
			case STRINGPANEL_MLA_ID:
				--prop.string.MIDIchanel;
				if(15 < prop.string.MIDIchanel)
				{
					prop.string.MIDIchanel = 15;
				}
				WM_SetUserData(pMsg->hWin, &prop, sizeof(MISASYNTHSTRING));
				WM_InvalidateRect(pMsg->hWin,&prop.string.rectMIDIButton.centre);
				WM_NotifyParent(pMsg->hWin, WM_NOTIFICATION_SEL_CHANGED);
				break;
			case STRINGPANEL_MRA_ID:
				++prop.string.MIDIchanel;
				if(15 < prop.string.MIDIchanel)
				{
					prop.string.MIDIchanel = 0;
				}
				WM_SetUserData(pMsg->hWin, &prop, sizeof(MISASYNTHSTRING));
				WM_InvalidateRect(pMsg->hWin,&prop.string.rectMIDIButton.centre);
				WM_NotifyParent(pMsg->hWin, WM_NOTIFICATION_SEL_CHANGED);
				break;
			case STRINGPANEL_SYNTH_ID:
				MisaRadio_SetStatus(prop.midi,0);
				SwitchSynthMode(STRINGPANEL_MODE_SYNTH,&prop);
				WM_SetUserData(pMsg->hWin, &prop, sizeof(MISASYNTHSTRING));
				WM_InvalidateRect(pMsg->hWin,&rectVoiceArea);
				WM_NotifyParent(pMsg->hWin, WM_NOTIFICATION_STRINGPANEL_SYNTH);
				break;
			case STRINGPANEL_MIDI_ID:
				MisaRadio_SetStatus(prop.synth,0);
				SwitchSynthMode(STRINGPANEL_MODE_MIDI,&prop);
				WM_SetUserData(pMsg->hWin, &prop, sizeof(MISASYNTHSTRING));
				WM_InvalidateRect(pMsg->hWin,&rectVoiceArea);
				WM_NotifyParent(pMsg->hWin, WM_NOTIFICATION_SEL_CHANGED);
			default:
				;
			}
		}
		break;
	case WM_GET_ID:
		pMsg->Data.v = prop.Id;
		break;
	case WM_CUST_GET_MISASYNTHVOICE:
		pMsg->Data.v = prop.string.Voice;
		break;
	default:
		WM_DefaultProc(pMsg);
	}
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa progress combo
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#define MISAPROGRESSCOMBO_MAX 300
#define MISAPROGRESSCOMBO_MIN 0

#define MISAPROGRESSCOMBO_TEXTHEIGHT 32
#define MISAPROGRESSCOMBO_WIDTH 500
#define MISAPROGRESSCOMBO_HEIGHT 70
#define MISAPROGRESSCOMBO_BUTTON_WIDTH 86
#define MISAPROGRESSCOMBO_CENTRE_WIDTH 328

#define MISAPROGRESSCOMBO_CLA_ID 0xF001
#define MISAPROGRESSCOMBO_CRA_ID 0xF002

typedef struct __MISAPROGCOMBOPROP
{
	int Id;
	int percent;
	int orgoffset;
	int max;
	float scale;
	int fontcolor;
	int displayoffset;
	MISAPROGINFOCB ShowTips;
	BUTTON_Handle cla;
	BUTTON_Handle cra;
	MISACOMBO progcombo;
	char label[32];
} MISAPROGCOMBOPROP,*LPMISAPROGCOMBOPROP;

typedef struct __MISAPROGCOMBO_OBJ
{
	WM_Obj Win;
	MISAPROGCOMBOPROP comboprop;
} MISAPROGCOMBO_OBJ;

static void MisaProgressComboProc(WM_MESSAGE* pMsg);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa progress combo
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

static void InitProgressCombo(PMISACOMBO pItem,int x, int y)
{
	// whole body
	pItem->body.x0 = x;
	pItem->body.y0 = y;
	pItem->body.x1 = x+MISAPROGRESSCOMBO_WIDTH;
	pItem->body.y1 = y+MISAPROGRESSCOMBO_HEIGHT+MISAPROGRESSCOMBO_TEXTHEIGHT;
	// left button
	pItem->leftbt.x0 = x;
	pItem->leftbt.y0 = y+MISAPROGRESSCOMBO_TEXTHEIGHT;
	pItem->leftbt.x1 = x+MISAPROGRESSCOMBO_BUTTON_WIDTH;
	pItem->leftbt.y1 = y+MISAPROGRESSCOMBO_HEIGHT+MISAPROGRESSCOMBO_TEXTHEIGHT;
	// centre part
	pItem->centre.x0 = x+MISAPROGRESSCOMBO_BUTTON_WIDTH;
	pItem->centre.y0 = y;
	pItem->centre.x1 = x+MISAPROGRESSCOMBO_BUTTON_WIDTH+MISAPROGRESSCOMBO_CENTRE_WIDTH;
	pItem->centre.y1 = y+MISAPROGRESSCOMBO_HEIGHT+MISAPROGRESSCOMBO_TEXTHEIGHT;
	// right button
	pItem->rightbt.x0 = x+MISAPROGRESSCOMBO_BUTTON_WIDTH+MISAPROGRESSCOMBO_CENTRE_WIDTH;
	pItem->rightbt.y0 = y+MISAPROGRESSCOMBO_TEXTHEIGHT;
	pItem->rightbt.x1 = x+MISAPROGRESSCOMBO_BUTTON_WIDTH*2+MISAPROGRESSCOMBO_CENTRE_WIDTH;
	pItem->rightbt.y1 = y+MISAPROGRESSCOMBO_HEIGHT+MISAPROGRESSCOMBO_TEXTHEIGHT;
}

U8 MisaProgressCombo_CreateItem(WM_HWIN hParent)
{
	MISAPROGCOMBOPROP prop;
	memset(&prop, 0, sizeof(MISAPROGCOMBOPROP));
	// Combo
	InitProgressCombo(&prop.progcombo, 0, 0);
	prop.cla = BUTTON_CreateAsChild(prop.progcombo.leftbt.x0,prop.progcombo.leftbt.y0,
		bmMC_CHLA_UN.XSize,bmMC_CHLA_UN.YSize,hParent,MISAPROGRESSCOMBO_CLA_ID,WM_CF_SHOW|WM_CF_MEMDEV);
	BUTTON_SetFocussable(prop.cla,0);
	BUTTON_SetBitmap(prop.cla,BUTTON_CI_UNPRESSED,&bmMC_CHLA_UN);
	BUTTON_SetBitmap(prop.cla,BUTTON_CI_PRESSED,&bmMC_CHLA_SE);
	prop.cra = BUTTON_CreateAsChild(prop.progcombo.rightbt.x0,prop.progcombo.rightbt.y0,
		bmMC_CHRA_UN.XSize,bmMC_CHRA_UN.YSize,hParent,MISAPROGRESSCOMBO_CRA_ID,WM_CF_SHOW|WM_CF_MEMDEV);
	BUTTON_SetFocussable(prop.cra,0);
	BUTTON_SetBitmap(prop.cra,BUTTON_CI_UNPRESSED,&bmMC_CHRA_UN);
	BUTTON_SetBitmap(prop.cra,BUTTON_CI_PRESSED,&bmMC_CHRA_SE);
	WM_SetUserData(hParent, &prop, sizeof(MISAPROGCOMBOPROP));
	return 0;
}

U8 MisaProgressCombo_DeleteItem(WM_HWIN hParent)
{
	MISAPROGCOMBOPROP prop;
	if(WM_IsWindow(hParent))
	{
		WM_GetUserData(hParent, &prop, sizeof(MISAPROGCOMBOPROP));
		BUTTON_Delete(prop.cla);
		BUTTON_Delete(prop.cra);
	}
	return 0;
}

WM_HWIN MisaProgressCombo_Create(int x, int y, int width, int height, WM_HWIN hParent, int Flags, int Id, const char* label)
{
	return MisaProgressCombo_CreateEx(x,y,width,height,hParent,Flags,Id,label,0);
}

WM_HWIN MisaProgressCombo_CreateEx(int x, int y, int width, int height, WM_HWIN hParent, int Flags, int Id, const char* label, MISAPROGINFOCB TipCB)
{
	WM_HWIN hMisaProgcombo;
	MISAPROGCOMBOPROP prop;
	hMisaProgcombo = WM_CreateWindowAsChild(x, y, MISAPROGRESSCOMBO_WIDTH, MISAPROGRESSCOMBO_HEIGHT+MISAPROGRESSCOMBO_TEXTHEIGHT, hParent, Flags, MisaProgressComboProc, sizeof(MISAPROGCOMBO_OBJ)-sizeof(WM_Obj));
	WM_GetUserData(hMisaProgcombo, &prop, sizeof(MISAPROGCOMBOPROP));
	prop.percent = 0;
	prop.orgoffset = 0;
	prop.max = 127;
	prop.scale = (float)MISAPROGRESSCOMBO_MAX/127;
	prop.Id = Id;
	prop.fontcolor = 0;
	prop.displayoffset = 0;
	prop.ShowTips = TipCB;
	if(label)
	{
		strcpy(prop.label,label);
	}
	else
	{
		memset(prop.label,0,32);
	}
	WM_SetUserData(hMisaProgcombo, &prop, sizeof(MISAPROGCOMBOPROP));
	return hMisaProgcombo;
}

void MisaProgressCombo_Delete(WM_HWIN hMisaProgcombo)
{
	MisaProgressCombo_DeleteItem(hMisaProgcombo);
	WM_DeleteWindow(hMisaProgcombo);
}

int MisaProgressCombo_GetPercent(WM_HWIN hMisaProgcombo)
{
	int result;
	MISAPROGCOMBOPROP prop;
	WM_GetUserData(hMisaProgcombo, &prop, sizeof(MISAPROGCOMBOPROP));
	result = prop.percent;
	return result;
}

void MisaProgressCombo_SetRange(WM_HWIN hMisaProgcombo, int max, int offset)
{
	MISAPROGCOMBOPROP prop;
	WM_GetUserData(hMisaProgcombo, &prop, sizeof(MISAPROGCOMBOPROP));
	prop.max = max;
	prop.scale = (float)MISAPROGRESSCOMBO_MAX/max;
	prop.displayoffset = offset;
	WM_SetUserData(hMisaProgcombo, &prop, sizeof(MISAPROGCOMBOPROP));
}

void MisaProgressCombo_SetPercent(WM_HWIN hMisaProgcombo, int percent)
{
	U8 changed;
	MISAPROGCOMBOPROP prop;
	WM_GetUserData(hMisaProgcombo, &prop, sizeof(MISAPROGCOMBOPROP));
	if(percent>prop.max)
	{
		percent = prop.max;
	}
	else if(percent<0)
	{
		percent = 0;
	}
	changed = percent != prop.percent;
	prop.percent = percent;
	prop.orgoffset = (int) (prop.percent*prop.scale);
	WM_SetUserData(hMisaProgcombo, &prop, sizeof(MISAPROGCOMBOPROP));
	WM_InvalidateWindow(hMisaProgcombo);
	if(changed)
	{
		WM_NotifyParent(hMisaProgcombo, WM_NOTIFICATION_VALUE_CHANGED);
	}
}

void MisaProgressCombo_SetPercentEx(WM_HWIN hMisaProgcombo, int percent, U8 update)
{
	U8 changed;
	MISAPROGCOMBOPROP prop;
	WM_GetUserData(hMisaProgcombo, &prop, sizeof(MISAPROGCOMBOPROP));
	if(percent>prop.max)
	{
		percent = prop.max;
	}
	else if(percent<0)
	{
		percent = 0;
	}
	changed = percent != prop.percent;
	prop.percent = percent;
	prop.orgoffset = (int) (prop.percent*prop.scale);
	WM_SetUserData(hMisaProgcombo, &prop, sizeof(MISAPROGCOMBOPROP));
	WM_InvalidateWindow(hMisaProgcombo);
	if(changed && update)
	{
		WM_NotifyParent(hMisaProgcombo, WM_NOTIFICATION_VALUE_CHANGED);
	}
}

void MisaProgressCombo_SetText(WM_HWIN hMisaProgcombo, char* str)
{
	MISAPROGCOMBOPROP prop;
	WM_GetUserData(hMisaProgcombo, &prop, sizeof(MISAPROGCOMBOPROP));
	strcpy(prop.label,str);
	WM_SetUserData(hMisaProgcombo, &prop, sizeof(MISAPROGCOMBOPROP));
	WM_InvalidateWindow(hMisaProgcombo);
}

void MisaProgressCombo_Increase(WM_HWIN hMisaProgcombo)
{
	U8 changed;
	MISAPROGCOMBOPROP prop;
	WM_GetUserData(hMisaProgcombo, &prop, sizeof(MISAPROGCOMBOPROP));
	prop.percent++;
	changed = 1;
	if(prop.max<prop.percent)
	{
		prop.percent = prop.max;
		changed = 0;
	}
	prop.orgoffset = (int) (prop.percent*prop.scale);
	WM_SetUserData(hMisaProgcombo, &prop, sizeof(MISAPROGCOMBOPROP));
	WM_InvalidateWindow(hMisaProgcombo);
	if(changed)
	{
		WM_NotifyParent(hMisaProgcombo, WM_NOTIFICATION_VALUE_CHANGED);
	}
}

void MisaProgressCombo_Decrease(WM_HWIN hMisaProgcombo)
{	
	U8 changed;
	MISAPROGCOMBOPROP prop;
	WM_GetUserData(hMisaProgcombo, &prop, sizeof(MISAPROGCOMBOPROP));
	prop.percent--;
	changed = 1;
	if(0>prop.percent)
	{
		prop.percent = 0;
		changed = 0;
	}
	prop.orgoffset = (int) (prop.percent*prop.scale);
	WM_SetUserData(hMisaProgcombo, &prop, sizeof(MISAPROGCOMBOPROP));
	WM_InvalidateWindow(hMisaProgcombo);
	if(changed)
	{
		WM_NotifyParent(hMisaProgcombo, WM_NOTIFICATION_VALUE_CHANGED);
	}
}

void MisaProgressCombo_SetCheck(WM_HWIN hMisaProgcombo, int state)
{
	MISAPROGCOMBOPROP prop;
	GUI_RECT rect = {0,315,100,366};
	WM_GetUserData(hMisaProgcombo, &prop, sizeof(MISAPROGCOMBOPROP));
	prop.fontcolor = state;
	WM_SetUserData(hMisaProgcombo, &prop, sizeof(MISAPROGCOMBOPROP));
	WM_InvalidateRect(hMisaProgcombo, &rect);
	//WM_InvalidateWindow(hMisaProgcombo);
}

static void MisaProgressComboDraw(PMISACOMBO pItem, LPMISAPROGCOMBOPROP pProp)
{
	int textmode;
	const GUI_FONT* font;
	GUI_Clear();
	GUI_DrawBitmap(&bmMC_CHANNEL_CENTRE, pItem->centre.x0, pItem->centre.y0+MISAPROGRESSCOMBO_TEXTHEIGHT);
	GUI_DrawBitmap(&bmMC_CHANNEL_SLIDER, pItem->centre.x0+pProp->orgoffset,pItem->centre.y0+MISAPROGRESSCOMBO_TEXTHEIGHT+9);
	textmode = GUI_GetTextMode();
	GUI_SetTextMode(GUI_TEXTMODE_TRANS);
	font = GUI_GetFont();

	GUI_SetFont(&GUI_Font24B_ASCII);
	//GUI_SetColor(GUI_WHITE);
	// Debug version only.
	if(pProp->ShowTips)
	{
		char buf[32];
		pProp->ShowTips(pProp->percent,buf);
		GUI_DispStringInRect(buf,&pItem->centre,GUI_TA_HCENTER|GUI_TA_TOP);
	}
	else
	{
		char buf[32];
		sprintf(buf,"%s  %d",pProp->label,pProp->percent+pProp->displayoffset);
		GUI_DispStringInRect(buf,&pItem->centre,GUI_TA_HCENTER|GUI_TA_TOP);
	}
	// call customized unit display function
	//if(pProp->fontcolor)
	//{
	//	GUI_SetColor(GUI_RED);
	//}

	GUI_SetFont(font);
	GUI_SetTextMode(textmode);
}

static void MisaProgressComboProc(WM_MESSAGE* pMsg)
{
	int NCode,Id;
	MISAPROGCOMBOPROP prop;
	GUI_PID_STATE* pPID_State;
	WM_GetUserData(pMsg->hWin, &prop, sizeof(MISAPROGCOMBOPROP));
	switch (pMsg->MsgId)
	{
	case WM_CREATE:
		MisaProgressCombo_CreateItem(pMsg->hWin);
		break;
	case WM_PAINT:
		MisaProgressComboDraw(&prop.progcombo,&prop);
		//x = prop.percent*1.182;
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
			case MISAPROGRESSCOMBO_CLA_ID:
				MisaProgressCombo_Decrease(pMsg->hWin);
				break;
			case MISAPROGRESSCOMBO_CRA_ID:
				MisaProgressCombo_Increase(pMsg->hWin);
			}
		}
		break;
	case WM_TOUCH:
		if(pMsg->Data.p)
		{
			pPID_State = (GUI_PID_STATE*)pMsg->Data.p;
			if(pPID_State->Pressed)
			{
				int perc;
				U8 changed;
				if(!WM_HasCaptured(pMsg->hWin))
				{
					WM_SetCapture(pMsg->hWin, 1);
					/*if(prop.progcombo.centre.x0+14 > pPID_State->x)
					{
						perc = MISAPROGRESSCOMBO_MAX;
					}
					else if(prop.progcombo.centre.x1-14 < pPID_State->x)
					{
						perc = MISAPROGRESSCOMBO_MAX;
					}
					else
					{
						perc = (pPID_State->x-prop.progcombo.centre.x0-14);
					}
					prop.orgoffset = perc;
					perc/=prop.scale;
					changed = prop.percent!=perc;
					prop.percent=perc;
					WM_SetUserData(pMsg->hWin, &prop, sizeof(MISAPROGCOMBOPROP));
					WM_InvalidateWindow(pMsg->hWin);
					if(changed)
					{
						WM_NotifyParent(pMsg->hWin, WM_NOTIFICATION_VALUE_CHANGED);
					}*/
				}
				else
				{
					if(prop.progcombo.centre.x0+14 > pPID_State->x)
					{
						perc = MISAPROGRESSCOMBO_MIN;
					}
					else if(prop.progcombo.centre.x1-14 < pPID_State->x)
					{
						perc = MISAPROGRESSCOMBO_MAX;
					}
					else
					{
						perc = (pPID_State->x-prop.progcombo.centre.x0-14);
					}
					prop.orgoffset = perc;
					perc = (int) (perc / prop.scale);
					changed = prop.percent!=perc;
					if(MISAPROGRESSCOMBO_MAX == prop.orgoffset)
					{
						prop.percent = prop.max;
					}
					else
					{
						prop.percent=perc;
					}
					WM_SetUserData(pMsg->hWin, &prop, sizeof(MISAPROGCOMBOPROP));
					WM_InvalidateWindow(pMsg->hWin);
					if(changed)
					{
						WM_NotifyParent(pMsg->hWin, WM_NOTIFICATION_VALUE_CHANGED);
					}
				}
			}
			else
			{
				//WM_ReleaseCapture();
				WM_NotifyParent(pMsg->hWin, WM_NOTIFICATION_RELEASED);
			}
		}
		break;
	case WM_GET_ID:
		pMsg->Data.v = prop.Id;
		break;
	default:
		WM_DefaultProc(pMsg);
	}
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa small progress combo
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#define MISASMALLPROGRESSCOMBO_MAX 125
#define MISASMALLPROGRESSCOMBO_MIN 0

#define MISASMALLPROGRESSCOMBO_TEXTHEIGHT 32
#define MISASMALLPROGRESSCOMBO_WIDTH 328
#define MISASMALLPROGRESSCOMBO_HEIGHT 34
#define MISASMALLPROGRESSCOMBO_BUTTON_WIDTH 86
#define MISASMALLPROGRESSCOMBO_CENTRE_WIDTH 156
#define MISASMALLPROGRESSCOMBO_SLIDER_OFFSET 15

static void MisaSmallProgressComboProc(WM_MESSAGE* pMsg);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa small progress combo
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

static void InitSmallProgressCombo(PMISACOMBO pItem,int x, int y)
{
	// whole body
	pItem->body.x0 = x;
	pItem->body.y0 = y;
	pItem->body.x1 = x+MISASMALLPROGRESSCOMBO_WIDTH;
	pItem->body.y1 = y+MISASMALLPROGRESSCOMBO_HEIGHT+MISASMALLPROGRESSCOMBO_TEXTHEIGHT;
	// left button
	pItem->leftbt.x0 = x;
	pItem->leftbt.y0 = y+MISASMALLPROGRESSCOMBO_TEXTHEIGHT;
	pItem->leftbt.x1 = x+MISASMALLPROGRESSCOMBO_BUTTON_WIDTH;
	pItem->leftbt.y1 = y+MISASMALLPROGRESSCOMBO_HEIGHT+MISASMALLPROGRESSCOMBO_TEXTHEIGHT;
	// centre part
	pItem->centre.x0 = x+MISASMALLPROGRESSCOMBO_BUTTON_WIDTH;
	pItem->centre.y0 = y+MISASMALLPROGRESSCOMBO_TEXTHEIGHT;
	pItem->centre.x1 = x+MISASMALLPROGRESSCOMBO_BUTTON_WIDTH+MISASMALLPROGRESSCOMBO_CENTRE_WIDTH;
	pItem->centre.y1 = y+MISASMALLPROGRESSCOMBO_HEIGHT+MISASMALLPROGRESSCOMBO_TEXTHEIGHT;
	// right button
	pItem->rightbt.x0 = x+MISASMALLPROGRESSCOMBO_BUTTON_WIDTH+MISASMALLPROGRESSCOMBO_CENTRE_WIDTH;
	pItem->rightbt.y0 = y+MISASMALLPROGRESSCOMBO_TEXTHEIGHT;
	pItem->rightbt.x1 = x+MISASMALLPROGRESSCOMBO_BUTTON_WIDTH*2+MISASMALLPROGRESSCOMBO_CENTRE_WIDTH;
	pItem->rightbt.y1 = y+MISASMALLPROGRESSCOMBO_HEIGHT+MISASMALLPROGRESSCOMBO_TEXTHEIGHT;
}

U8 MisaSmallProgressCombo_CreateItem(WM_HWIN hParent)
{
	MISAPROGCOMBOPROP prop;
	memset(&prop, 0, sizeof(MISAPROGCOMBOPROP));
	// Combo
	InitSmallProgressCombo(&prop.progcombo, 0, 0);
	prop.cla = BUTTON_CreateAsChild(prop.progcombo.leftbt.x0,prop.progcombo.leftbt.y0,
		bmCP_LEFTARROW_UN.XSize,bmCP_LEFTARROW_UN.YSize,hParent,MISAPROGRESSCOMBO_CLA_ID,WM_CF_SHOW|WM_CF_MEMDEV);
	BUTTON_SetFocussable(prop.cla,0);
	BUTTON_SetBitmap(prop.cla,BUTTON_CI_UNPRESSED,&bmCP_LEFTARROW_UN);
	BUTTON_SetBitmap(prop.cla,BUTTON_CI_PRESSED,&bmCP_LEFTARROW_SE);
	prop.cra = BUTTON_CreateAsChild(prop.progcombo.rightbt.x0,prop.progcombo.rightbt.y0,
		bmCP_RIGHTARROW_UN.XSize,bmCP_RIGHTARROW_UN.YSize,hParent,MISAPROGRESSCOMBO_CRA_ID,WM_CF_SHOW|WM_CF_MEMDEV);
	BUTTON_SetFocussable(prop.cra,0);
	BUTTON_SetBitmap(prop.cra,BUTTON_CI_UNPRESSED,&bmCP_RIGHTARROW_UN);
	BUTTON_SetBitmap(prop.cra,BUTTON_CI_PRESSED,&bmCP_RIGHTARROW_SE);
	WM_SetUserData(hParent, &prop, sizeof(MISAPROGCOMBOPROP));
	return 0;
}

U8 MisaSmallProgressCombo_DeleteItem(WM_HWIN hParent)
{
	MISAPROGCOMBOPROP prop;
	if(WM_IsWindow(hParent))
	{
		WM_GetUserData(hParent, &prop, sizeof(MISAPROGCOMBOPROP));
		BUTTON_Delete(prop.cla);
		BUTTON_Delete(prop.cra);
	}
	return 0;
}

WM_HWIN MisaSmallProgressCombo_Create(int x, int y, int width, int height, WM_HWIN hParent, int Flags, int Id, const char* label)
{
	return MisaSmallProgressCombo_CreateEx(x,y,width,height,hParent,Flags,Id,label,0);
}

WM_HWIN MisaSmallProgressCombo_CreateEx(int x, int y, int width, int height, WM_HWIN hParent, int Flags, int Id, const char* label, MISAPROGINFOCB TipCB)
{
	WM_HWIN hMisaProgcombo;
	MISAPROGCOMBOPROP prop;
	hMisaProgcombo = WM_CreateWindowAsChild(x, y, MISASMALLPROGRESSCOMBO_WIDTH, MISASMALLPROGRESSCOMBO_HEIGHT+MISASMALLPROGRESSCOMBO_TEXTHEIGHT, hParent, Flags, MisaSmallProgressComboProc, sizeof(MISAPROGCOMBO_OBJ)-sizeof(WM_Obj));
	WM_GetUserData(hMisaProgcombo, &prop, sizeof(MISAPROGCOMBOPROP));
	prop.percent = 0;
	prop.orgoffset = 0;
	prop.max = 127;
	prop.scale = (float)MISASMALLPROGRESSCOMBO_MAX/127;
	prop.Id = Id;
	prop.fontcolor = 0;
	prop.displayoffset = 0;
	prop.ShowTips = TipCB;
	if(label)
	{
		strcpy(prop.label,label);
	}
	else
	{
		memset(prop.label,0,32);
	}
	WM_SetUserData(hMisaProgcombo, &prop, sizeof(MISAPROGCOMBOPROP));
	return hMisaProgcombo;
}

void MisaSmallProgressCombo_Delete(WM_HWIN hMisaProgcombo)
{
	MisaProgressCombo_DeleteItem(hMisaProgcombo);
	WM_DeleteWindow(hMisaProgcombo);
}

int MisaSmallProgressCombo_GetPercent(WM_HWIN hMisaProgcombo)
{
	int result;
	MISAPROGCOMBOPROP prop;
	WM_GetUserData(hMisaProgcombo, &prop, sizeof(MISAPROGCOMBOPROP));
	result = prop.percent;
	return result;
}

void MisaSmallProgressCombo_SetRange(WM_HWIN hMisaProgcombo, int max, int offset)
{
	MISAPROGCOMBOPROP prop;
	WM_GetUserData(hMisaProgcombo, &prop, sizeof(MISAPROGCOMBOPROP));
	prop.max = max;
	prop.scale = (float)MISASMALLPROGRESSCOMBO_MAX/max;
	prop.displayoffset = offset;
	WM_SetUserData(hMisaProgcombo, &prop, sizeof(MISAPROGCOMBOPROP));
}

void MisaSmallProgressCombo_SetPercent(WM_HWIN hMisaProgcombo, int percent)
{
	U8 changed;
	MISAPROGCOMBOPROP prop;
	WM_GetUserData(hMisaProgcombo, &prop, sizeof(MISAPROGCOMBOPROP));
	if(percent>prop.max)
	{
		percent = prop.max;
	}
	else if(percent<0)
	{
		percent = 0;
	}
	changed = percent != prop.percent;
	prop.percent = percent;
	prop.orgoffset = (int) (prop.percent*prop.scale);
	WM_SetUserData(hMisaProgcombo, &prop, sizeof(MISAPROGCOMBOPROP));
	WM_InvalidateWindow(hMisaProgcombo);
	if(changed)
	{
		WM_NotifyParent(hMisaProgcombo, WM_NOTIFICATION_VALUE_CHANGED);
	}
}

void MisaSmallProgressCombo_SetPercentEx(WM_HWIN hMisaProgcombo, int percent, U8 update)
{
	U8 changed;
	MISAPROGCOMBOPROP prop;
	WM_GetUserData(hMisaProgcombo, &prop, sizeof(MISAPROGCOMBOPROP));
	if(percent>prop.max)
	{
		percent = prop.max;
	}
	else if(percent<0)
	{
		percent = 0;
	}
	changed = percent != prop.percent;
	prop.percent = percent;
	prop.orgoffset = (int) (prop.percent*prop.scale);
	WM_SetUserData(hMisaProgcombo, &prop, sizeof(MISAPROGCOMBOPROP));
	WM_InvalidateWindow(hMisaProgcombo);
	if(changed && update)
	{
		WM_NotifyParent(hMisaProgcombo, WM_NOTIFICATION_VALUE_CHANGED);
	}
}

void MisaSmallProgressCombo_Increase(WM_HWIN hMisaProgcombo)
{
	U8 changed;
	MISAPROGCOMBOPROP prop;
	WM_GetUserData(hMisaProgcombo, &prop, sizeof(MISAPROGCOMBOPROP));
	prop.percent++;
	changed = 1;
	if(prop.max<prop.percent)
	{
		prop.percent = prop.max;
		changed = 0;
	}
	prop.orgoffset = (int) (prop.percent*prop.scale);
	WM_SetUserData(hMisaProgcombo, &prop, sizeof(MISAPROGCOMBOPROP));
	WM_InvalidateWindow(hMisaProgcombo);
	if(changed)
	{
		WM_NotifyParent(hMisaProgcombo, WM_NOTIFICATION_VALUE_CHANGED);
	}
}

void MisaSmallProgressCombo_Decrease(WM_HWIN hMisaProgcombo)
{	
	U8 changed;
	MISAPROGCOMBOPROP prop;
	WM_GetUserData(hMisaProgcombo, &prop, sizeof(MISAPROGCOMBOPROP));
	prop.percent--;
	changed = 1;
	if(0>prop.percent)
	{
		prop.percent = 0;
		changed = 0;
	}
	prop.orgoffset = (int) (prop.percent*prop.scale);
	WM_SetUserData(hMisaProgcombo, &prop, sizeof(MISAPROGCOMBOPROP));
	WM_InvalidateWindow(hMisaProgcombo);
	if(changed)
	{
		WM_NotifyParent(hMisaProgcombo, WM_NOTIFICATION_VALUE_CHANGED);
	}
}

void MisaSmallProgressCombo_SetCheck(WM_HWIN hMisaProgcombo, int state)
{
	MISAPROGCOMBOPROP prop;
	WM_GetUserData(hMisaProgcombo, &prop, sizeof(MISAPROGCOMBOPROP));
	prop.fontcolor = state;
	WM_SetUserData(hMisaProgcombo, &prop, sizeof(MISAPROGCOMBOPROP));
	WM_InvalidateWindow(hMisaProgcombo);
}

static void MisaSmallProgressComboDraw(PMISACOMBO pItem, LPMISAPROGCOMBOPROP pProp)
{
	int textmode;
	const GUI_FONT* font;
	GUI_Clear();
	GUI_DrawBitmap(&bmCP_CENTRE, pItem->centre.x0, pItem->centre.y0);
	GUI_DrawBitmap(&bmCP_SMALLSLIDER, pItem->centre.x0+pProp->orgoffset,pItem->centre.y0+7);
	textmode = GUI_GetTextMode();
	GUI_SetTextMode(GUI_TEXTMODE_TRANS);
	font = GUI_GetFont();

	GUI_SetFont(&GUI_Font24B_ASCII);
	//GUI_SetColor(GUI_WHITE);
	// Debug version only.
	// call customized unit display function
	if(pProp->fontcolor)
	{
		GUI_SetColor(GUI_RED);
	}

	if(pProp->ShowTips)
	{
		char buf[32];
		pProp->ShowTips(pProp->percent,buf);
		GUI_DispStringInRect(buf,&pItem->body,GUI_TA_LEFT|GUI_TA_TOP);
	}
	else
	{
		char buf[32];
		sprintf(buf,"%s  %d",pProp->label,pProp->percent+pProp->displayoffset);
		GUI_DispStringInRect(buf,&pItem->body,GUI_TA_LEFT|GUI_TA_TOP);
	}

	GUI_SetFont(font);
	GUI_SetTextMode(textmode);
}

static void MisaSmallProgressComboProc(WM_MESSAGE* pMsg)
{
	int NCode,Id;
	MISAPROGCOMBOPROP prop;
	GUI_PID_STATE* pPID_State;
	WM_GetUserData(pMsg->hWin, &prop, sizeof(MISAPROGCOMBOPROP));
	switch (pMsg->MsgId)
	{
	case WM_CREATE:
		MisaSmallProgressCombo_CreateItem(pMsg->hWin);
		break;
	case WM_PAINT:
		MisaSmallProgressComboDraw(&prop.progcombo,&prop);
		//x = prop.percent*1.182;
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
			case MISAPROGRESSCOMBO_CLA_ID:
				MisaSmallProgressCombo_Decrease(pMsg->hWin);
				break;
			case MISAPROGRESSCOMBO_CRA_ID:
				MisaSmallProgressCombo_Increase(pMsg->hWin);
			}
		}
		break;
	case WM_TOUCH:
		if(pMsg->Data.p)
		{
			pPID_State = (GUI_PID_STATE*)pMsg->Data.p;
			if(pPID_State->Pressed)
			{
				int perc;
				U8 changed;
				GUI_RECT ptRect;
				ptRect.x0 = pPID_State->x;
				ptRect.y0 = pPID_State->y;
				ptRect.x1 = ptRect.x0+1;
				ptRect.y1 = ptRect.y0+1;
				if(!WM_HasCaptured(pMsg->hWin) && GUI_RectsIntersect(&prop.progcombo.centre,&ptRect))
				{
					WM_SetCapture(pMsg->hWin, 0);
					/*if(prop.progcombo.centre.x0+MISASMALLPROGRESSCOMBO_SLIDER_OFFSET > pPID_State->x)
					{
						perc = MISASMALLPROGRESSCOMBO_MAX;
					}
					else if(prop.progcombo.centre.x1-MISASMALLPROGRESSCOMBO_SLIDER_OFFSET < pPID_State->x)
					{
						perc = MISASMALLPROGRESSCOMBO_MAX;
					}
					else
					{
						perc = (pPID_State->x-prop.progcombo.centre.x0-MISASMALLPROGRESSCOMBO_SLIDER_OFFSET);
					}
					prop.orgoffset = perc;
					perc/=prop.scale;
					changed = prop.percent!=perc;
					prop.percent=perc;
					WM_SetUserData(pMsg->hWin, &prop, sizeof(MISAPROGCOMBOPROP));
					WM_InvalidateWindow(pMsg->hWin);
					if(changed)
					{
						WM_NotifyParent(pMsg->hWin, WM_NOTIFICATION_VALUE_CHANGED);
					}*/
				}
				else
				{
					if(WM_HasCaptured(pMsg->hWin))
					{
						if(prop.progcombo.centre.x0+MISASMALLPROGRESSCOMBO_SLIDER_OFFSET > pPID_State->x)
						{
							perc = MISASMALLPROGRESSCOMBO_MIN;
						}
						else if(prop.progcombo.centre.x1-MISASMALLPROGRESSCOMBO_SLIDER_OFFSET < pPID_State->x)
						{
							perc = MISASMALLPROGRESSCOMBO_MAX;
						}
						else
						{
							perc = (pPID_State->x-prop.progcombo.centre.x0-MISASMALLPROGRESSCOMBO_SLIDER_OFFSET);
						}
						prop.orgoffset = perc;
						perc = (int) (perc / prop.scale);
						changed = prop.percent!=perc;
						if(MISASMALLPROGRESSCOMBO_MAX == prop.orgoffset)
						{
							prop.percent = prop.max;
						}
						else
						{
							prop.percent=perc;
						}
						WM_SetUserData(pMsg->hWin, &prop, sizeof(MISAPROGCOMBOPROP));
						WM_InvalidateWindow(pMsg->hWin);
						if(changed)
						{
							WM_NotifyParent(pMsg->hWin, WM_NOTIFICATION_VALUE_CHANGED);
						}
					}
				}
			}
			else
			{
				if(WM_HasCaptured(pMsg->hWin))
				{
					WM_ReleaseCapture();
					WM_NotifyParent(pMsg->hWin, WM_NOTIFICATION_RELEASED);
				}
				else
				{
					WM_NotifyParent(pMsg->hWin, WM_NOTIFICATION_SEL_CHANGED);
				}
			}
		}
		break;
	case WM_GET_ID:
		pMsg->Data.v = prop.Id;
		break;
	default:
		WM_DefaultProc(pMsg);
	}
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa item button
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

static WM_CALLBACK* DefaultItemProc;
static void MisaItemProc(WM_MESSAGE* pMsg);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa item button
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

BUTTON_Handle MisaItem_Create(int x0, int y0, int xsize, int ysize, WM_HWIN hParent, int Id, int Flags,const GUI_BITMAP * pUncheck,const GUI_BITMAP * pCheck)
{	
	BUTTON_Handle hMisaItem = BUTTON_CreateAsChild(x0,y0,xsize,ysize,hParent,Id,WM_CF_SHOW | WM_CF_MEMDEV);
	BUTTON_SetFocussable(hMisaItem,0);
	if(pUncheck)
	{
		BUTTON_SetBitmap(hMisaItem,BUTTON_CI_UNPRESSED,pUncheck);
	}
	else
	{
		BUTTON_SetBkColor(hMisaItem, BUTTON_CI_UNPRESSED, GUI_BLACK);
	}
	if(pCheck)
	{
		BUTTON_SetBitmap(hMisaItem,BUTTON_CI_PRESSED,pCheck);
	}
	else
	{
		BUTTON_SetBkColor(hMisaItem,BUTTON_CI_PRESSED,GUI_GRAY);
	}
	DefaultItemProc = WM_SetCallback(hMisaItem,MisaItemProc);
	return hMisaItem;
}

BUTTON_Handle MisaItem_CreateEx(int x0, int y0, int xsize, int ysize, WM_HWIN hParent, int Id, int Flags, const char* pName, const GUI_BITMAP * pUncheck,const GUI_BITMAP * pCheck)
{	
	BUTTON_Handle hMisaItem = BUTTON_CreateAsChild(x0,y0,xsize,ysize,hParent,Id,WM_CF_SHOW | WM_CF_MEMDEV);
	//BUTTON_SetFocussable(hMisaItem,0);
	BUTTON_SetBkColor(hMisaItem, BUTTON_CI_UNPRESSED, GUI_BLACK);
	BUTTON_SetBkColor(hMisaItem,BUTTON_CI_PRESSED,GUI_BLACK);
	BUTTON_SetBkColor(hMisaItem, BUTTON_CI_DISABLED, GUI_BLACK);
	if(pUncheck)
	{
		BUTTON_SetBitmap(hMisaItem,BUTTON_CI_UNPRESSED,pUncheck);
	}
	if(pCheck)
	{
		BUTTON_SetBitmap(hMisaItem,BUTTON_CI_PRESSED,pCheck);
	}
	if(pName)
	{
		BUTTON_SetTextColor(hMisaItem, BUTTON_CI_UNPRESSED, GUI_WHITE);
		BUTTON_SetTextColor(hMisaItem, BUTTON_CI_PRESSED, GUI_WHITE);
		BUTTON_SetFont(hMisaItem, &GUI_Font32B_ASCII);
		BUTTON_SetText(hMisaItem, pName);
	}
	DefaultItemProc = WM_SetCallback(hMisaItem,MisaItemProc);
	return hMisaItem;
}

void MisaItem_Delete(BUTTON_Handle hMisaItem)
{
	BUTTON_Delete(hMisaItem);
}

void MisaItem_SetGoup(MISARADIO_GROUP radioGroup)
{
}

void MisaItem_SetStatus(BUTTON_Handle hMisaItem,int State)
{
	BUTTON_SetState(hMisaItem,State<<8);
}

U8 MisaItem_GetStatus(BUTTON_Handle hMisaItem)
{
	return WIDGET_GetState(hMisaItem)&WIDGET_STATE_FOCUS?1:0;
}

static void MisaItemOntouch(BUTTON_Handle hObj, BUTTON_Obj* pObj, WM_MESSAGE*pMsg)
{
	const GUI_PID_STATE* pState = (const GUI_PID_STATE*)pMsg->Data.p;
	if (pMsg->Data.p)
	{
		if(pState->Pressed)
		{
			WM_NotifyParent(hObj, WM_NOTIFICATION_CLICKED);
		}
		else
		{
			if(pObj->Widget.State & BUTTON_STATE_PRESSED)
			{
			}
			else
			{
				WIDGET_OrState(hObj, BUTTON_STATE_PRESSED);
			}
			WM_NotifyParent(hObj, WM_NOTIFICATION_RELEASED);
		}
	}
}

static void MisaItemPaint(BUTTON_Obj* pObj, BUTTON_Handle hObj)
{
	const char* s = NULL;
	unsigned int Index;
	int State, PressedState, ColorIndex;
	GUI_RECT rClient, rInside;
	State = pObj->Widget.State;
	PressedState = (State & BUTTON_STATE_PRESSED) ? 1 : 0;
	ColorIndex   = (WM__IsEnabled(hObj)) ? PressedState : 2;
	GUI_SetFont(pObj->Props.pFont);

	if (pObj->hpText)
	{
		s = (const char*) GUI_ALLOC_h2p(pObj->hpText);
	}
	GUI_GetClientRect(&rClient);
	/* Start drawing */
	rInside = rClient;

	/* Draw background */
	LCD_SetBkColor (pObj->Props.aBkColor[ColorIndex]);
	LCD_SetColor   (pObj->Props.aTextColor[ColorIndex]);
	WM_SetUserClipRect(&rInside);
	GUI_Clear();
	/* Draw bitmap.
	 If we have only one, we will use it.
	 If we have to we will use the second one (Index 1) for the pressed state
	*/
	if (ColorIndex < 2)
	{
		Index = State&BUTTON_STATE_FOCUS ? BUTTON_BI_PRESSED : BUTTON_BI_UNPRESSED;
	}
	else
	{
		Index = BUTTON_BI_UNPRESSED;
	}
	GUI_DRAW__Draw(pObj->ahDrawObj[Index], 0, 0);
	/* Draw the actual button (background and text) */  
	{
		GUI_RECT r;
		r = rInside;
		GUI_SetTextMode(GUI_TM_TRANS);
		if('\t'!=s[0])
		{
			GUI_DispStringInRect(s, &r, pObj->Props.Align);
		}
		else
		{
			char* pDest;
			char buf[32];
			pDest = const_cast<char*>(strchr(++s,'\t'));	// OR 28-06-11 : make it compile with Visual Studio 2008
			if(32 > strlen(s) && pDest)
			{
				strcpy(buf,s);
				pDest = strchr(buf,'\t');
				if(pDest)
				{
					*pDest = 0;
				}
				GUI_MoveRect(&r, 45, 0);
				GUI_DispStringInRect(buf, &r, GUI_TA_LEFT|GUI_TA_VCENTER);
				s = ++pDest;
				pDest = const_cast<char*>(strchr(s,'\t'));	// OR 28-06-11 : make it compile with Visual Studio 2008
				if(pDest)
				{
					*pDest = 0;
				}
				GUI_MoveRect(&r, 328-45, 0);
				GUI_DispStringInRect(s, &r, GUI_TA_LEFT|GUI_TA_VCENTER);
				s = ++pDest;
				pDest = const_cast<char*>(strchr(s,'\t'));	// OR 28-06-11 : make it compile with Visual Studio 2008
				if(pDest)
				{
					*pDest = 0;
				}
				GUI_MoveRect(&r, 490-328, 0);
				GUI_DispStringInRect(s, &r, GUI_TA_LEFT|GUI_TA_VCENTER);
				s = ++pDest;
				pDest = const_cast<char*>(strchr(s,'\t'));	// OR 28-06-11 : make it compile with Visual Studio 2008
				if(pDest)
				{
					*pDest = 0;
				}
				GUI_MoveRect(&r, 675-490, 0);
				GUI_DispStringInRect(s, &r, GUI_TA_LEFT|GUI_TA_VCENTER);
			}
		}
	}
	/* Draw focus */
	//if (State & BUTTON_STATE_FOCUS)
	//{
	//	LCD_SetColor(pObj->Props.FocusColor);
	//	GUI_DrawFocusRect(&rClient, 2);
	//}
	WM_SetUserClipRect(NULL);
}

static void MisaItemProc(WM_MESSAGE* pMsg)
{
	BUTTON_Handle hObj = pMsg->hWin;
	BUTTON_Obj * pObj = (BUTTON_Obj *)GUI_ALLOC_h2p(hObj);
	GUI_PID_STATE* pPID_State;
	int X, Y;

	switch (pMsg->MsgId)
	{
	case WM_TOUCH:
		pPID_State = (GUI_PID_STATE*)pMsg->Data.p;
		if (pPID_State)
		{
			X = pPID_State->x + WM_GetWindowOrgX(hObj);
			Y = pPID_State->y + WM_GetWindowOrgY(hObj) - WM_GetWindowOrgY(WM_GetParent(hObj));
		}
		DefaultItemProc(pMsg);
		if (pPID_State)
		{
			pPID_State->x = X;
			pPID_State->y = Y;
			WM_SendToParent(hObj, pMsg);
		}
		break;

	case WM_PAINT:
		MisaItemPaint(pObj,hObj);
		break;
	//case WM_SET_FOCUS:
	//	pMsg->hWin = pMsg->hWin;
	//	break;
	default:
		if(DefaultItemProc)
		{
			DefaultItemProc(pMsg);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa vertical slide bar
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#define MISAVERSLIDER_MAX 127
#define MISAVERSLIDER_MIN 0

typedef struct __MISAVERSLIDERSPROP
{
	int Id;
	int percent;
	MISAPROGINFOCB ShowTips;
} MISAVERSLIDERSPROP,*LPMISAVERSLIDERSPROP;

typedef struct __MISAVERSLIDER_OBJ
{
	WM_Obj Win;
	MISAVERSLIDERSPROP progprop;
} MISAVERSLIDER_OBJ;

static WM_CALLBACK* DefaultVerSliderProc;
static void MisaVerSliderProc(WM_MESSAGE* pMsg);
static const GUI_RECT tipRect=
{
	0,279,69,299
};

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa progress bar
// 312
// 315
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

WM_HWIN MisaVerSlider_CreateEx(int x, int y, int width, int height, WM_HWIN hParent, int Id, MISAPROGINFOCB TipCB)
{
	WM_HWIN hVerslider;
	MISAVERSLIDERSPROP prop;
	hVerslider = WM_CreateWindowAsChild(x, y, 70/*bmMENU_CUBE.XSize*/, 300/*bmMENU_VLINE.YSize*/, hParent, /*WM_CF_SHOW |*/ WM_CF_MEMDEV, MisaVerSliderProc, sizeof(MISAVERSLIDER_OBJ)-sizeof(WM_Obj));
	prop.percent = 0;
	prop.Id = Id;
	prop.ShowTips = TipCB;
	WM_SetUserData(hVerslider, &prop, sizeof(MISAVERSLIDERSPROP));
	return hVerslider;
}

WM_HWIN MisaVerSlider_Create(int x, int y, int width, int height, WM_HWIN hParent, int Id)
{
	return MisaVerSlider_CreateEx(x,y,width,height,hParent,Id,0);
}

void MisaVerSlider_Delete(WM_HWIN hVerslider)
{
	WM_DeleteWindow(hVerslider);
}

int MisaVerSlider_GetPercent(WM_HWIN hVerslider)
{
	int result;
	MISAVERSLIDERSPROP prop;
	WM_GetUserData(hVerslider, &prop, sizeof(MISAVERSLIDERSPROP));
	result = prop.percent;
	return result;
}

void MisaVerSlider_SetPercent(WM_HWIN hVerslider, int percent)
{
	U8 changed;
	MISAVERSLIDERSPROP prop;
	WM_GetUserData(hVerslider, &prop, sizeof(MISAVERSLIDERSPROP));
	if(percent>MISAVERSLIDER_MAX)
	{
		percent = MISAVERSLIDER_MAX;
	}
	else if(percent<0)
	{
		percent = 0;
	}
	changed = percent != prop.percent;
	prop.percent = percent;
	WM_SetUserData(hVerslider, &prop, sizeof(MISAVERSLIDERSPROP));
	WM_InvalidateWindow(hVerslider);
	if(changed)
	{
		WM_NotifyParent(hVerslider, WM_NOTIFICATION_VALUE_CHANGED);
	}
}

void MisaVerSlider_SetPercentEx(WM_HWIN hVerslider, int percent, U8 update)
{
	U8 changed;
	MISAVERSLIDERSPROP prop;
	WM_GetUserData(hVerslider, &prop, sizeof(MISAVERSLIDERSPROP));
	if(percent>MISAVERSLIDER_MAX)
	{
		percent = MISAVERSLIDER_MAX;
	}
	else if(percent<0)
	{
		percent = 0;
	}
	changed = percent != prop.percent;
	prop.percent = percent;
	WM_SetUserData(hVerslider, &prop, sizeof(MISAVERSLIDERSPROP));
	WM_InvalidateWindow(hVerslider);
	if(changed && update)
	{
		WM_NotifyParent(hVerslider, WM_NOTIFICATION_VALUE_CHANGED);
	}
}

void MisaVerSlider_Increase(WM_HWIN hVerslider)
{
	U8 changed;
	MISAVERSLIDERSPROP prop;
	WM_GetUserData(hVerslider, &prop, sizeof(MISAVERSLIDERSPROP));
	prop.percent++;
	changed = 1;
	if(MISAVERSLIDER_MAX<=prop.percent)
	{
		prop.percent = MISAVERSLIDER_MAX;
		changed = 0;
	}
	WM_SetUserData(hVerslider, &prop, sizeof(MISAVERSLIDERSPROP));
	WM_InvalidateWindow(hVerslider);
	if(changed)
	{
		WM_NotifyParent(hVerslider, WM_NOTIFICATION_VALUE_CHANGED);
	}
}

void MisaVerSlider_Decrease(WM_HWIN hVerslider)
{	
	U8 changed;
	MISAVERSLIDERSPROP prop;
	WM_GetUserData(hVerslider, &prop, sizeof(MISAVERSLIDERSPROP));
	prop.percent--;
	changed = 1;
	if(0>=prop.percent)
	{
		prop.percent = 0;
		changed = 0;
	}
	WM_SetUserData(hVerslider, &prop, sizeof(MISAVERSLIDERSPROP));
	WM_InvalidateWindow(hVerslider);
	if(changed)
	{
		WM_NotifyParent(hVerslider, WM_NOTIFICATION_VALUE_CHANGED);
	}
}

static void MisaVerSliderProc(WM_MESSAGE* pMsg)
{
	int x;
	char buf[32];
	MISAVERSLIDERSPROP prop;
	GUI_PID_STATE* pPID_State;
	WM_GetUserData(pMsg->hWin, &prop, sizeof(MISAVERSLIDERSPROP));
	switch (pMsg->MsgId)
	{
	case WM_PAINT:
		x = WM_GetWindowSizeX(pMsg->hWin);
		//
		GUI_SetColor(GUI_BLACK);
		GUI_Clear();
		GUI_DrawBitmap(&bmMENU_VLINE,33,0);
		GUI_DrawBitmap(&bmMENU_CUBE,20,(127-prop.percent)*2);
		GUI_FillRect(tipRect.x0, tipRect.y0, tipRect.x1, tipRect.y1);
		GUI_SetColor(GUI_LIGHTBLUE);
		GUI_DrawRect(tipRect.x0, tipRect.y0, tipRect.x1, tipRect.y1);
		//
		GUI_SetTextMode(GUI_TEXTMODE_TRANS);
		GUI_SetColor(GUI_LIGHTBLUE);
		GUI_SetFont(&GUI_Font16_ASCII);
		if(prop.ShowTips)
		{
			prop.ShowTips(prop.percent,buf);
			GUI_DispStringInRect(buf,(GUI_RECT*)&tipRect,GUI_TA_HCENTER|GUI_TA_VCENTER);
		}
		else
		{
			sprintf(buf,"%d",prop.percent);
			GUI_DispStringInRect(buf, (GUI_RECT*)&tipRect, GUI_TA_HCENTER|GUI_TA_VCENTER);
		}
		break;
	case WM_TOUCH:
		if(pMsg->Data.p)
		{
			pPID_State = (GUI_PID_STATE*)pMsg->Data.p;
			if(pPID_State->Pressed)
			{
				int perc;
				if(!WM_HasCaptured(pMsg->hWin) && 280 > pPID_State->y)
				{
					WM_SetCapture(pMsg->hWin, 1);
				}
				if(pPID_State->y <= 12)
				{
					perc = 127;
				}
				else if(pPID_State->y >= 280-12)
				{
					perc = 0;
				}
				else
				{
					perc = (280-12-pPID_State->y)/2;
				}
				if(WM_HasCaptured(pMsg->hWin))
				{
					MisaVerSlider_SetPercent(pMsg->hWin,perc);
				}
			}
			else
			{
				WM_ReleaseCapture();
				WM_NotifyParent(pMsg->hWin, WM_NOTIFICATION_RELEASED);
			}
		}
		break;
	case WM_GET_ID:
		pMsg->Data.v = prop.Id;
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

#if 0

typedef struct __SLIDEPANELPROP
{
	WM_HWIN hPanel;
	WM_HWIN hSliding;
	int width;
	int height;
} SLIDEPANELPROP,*LPSLIDEPANELPROP;

typedef struct __SLIDEPANEL_OBJ
{
	WM_Obj Win;
	SLIDEPANELPROP spprop;
} SLIDEPANEL_OBJ;

#define INDICATORFRAME 50

static WM_HWIN hSlideWindow;
static void SlideWindowProc(WM_MESSAGE* pMsg);
static void IndicatorProc(WM_MESSAGE* pMsg);

WM_HWIN CreateSlidePanel(int x0, int y0, int width, int height, WM_HWIN hParent, U16 Style)
{
	WM_HWIN hWin;
	SLIDEPANELPROP spwinprop;
	hWin = WM_CreateWindowAsChild(x0,y0,width,height,hParent,Style,IndicatorProc,sizeof(SLIDEPANEL_OBJ)-sizeof(WM_Obj));
	spwinprop.hPanel = hWin;
	spwinprop.hSliding = 0;
	WM_SetUserData(hWin, &spwinprop, sizeof(SLIDEPANELPROP));
	return hWin;
}

static U8 DeleteSlidePanel(WM_HWIN hSlidePanel)
{
	WM_DeleteWindow(hSlidePanel);
	return 0;
}

static WM_HWIN IndicatorCreateItems(WM_HWIN hParent)
{
	return WM_CreateWindowAsChild(0,0,1500,WM_GetWindowSizeY(hParent),hParent,WM_CF_SHOW|WM_CF_MEMDEV,SlideWindowProc,0);
}

static U8 IndicatorDeleteItems()
{
	if(hSystemItems[SYSTEM_SLIDEWINDOW])
	{
		WM_DeleteWindow(hSystemItems[SYSTEM_SLIDEWINDOW]);
	}
	return 0;
}

static void IndicatorProc(WM_MESSAGE* pMsg)
{
	SLIDEPANELPROP spwinprop;
	WM_GetUserData(pMsg->hWin, &spwinprop, sizeof(SLIDEPANELPROP));
	switch (pMsg->MsgId)
	{
	case WM_CREATE:
		IndicatorCreateItems(pMsg->hWin);
		break;
	case WM_DELETE:	
		IndicatorDeleteItems();
		break;
	case WM_PAINT:
		//GUI_SetBkColor(GUI_BLACK);
		//GUI_Clear();
		break;
	default:
		WM_DefaultProc(pMsg);
	}
}

static U8 SlideCreateItems(WM_HWIN hParent)
{
	int x,y;
	memset(hSystemItems,0,sizeof(hSystemItems));
	x=0;
	y = 0;
	hSystemItems[SYSTEM_PROGDRIVE] = MisaProgressbar_Create(50,y,0,0,hParent,SYSTEM_ID_PROGDRIVE,"Drive",MISAPROG_FONT24);
	hSystemItems[SYSTEM_PROGTONE] = MisaProgressbar_Create(200,y,0,0,hParent,SYSTEM_ID_PROGTONE,"Tone",MISAPROG_FONT24);
	hSystemItems[SYSTEM_PROGLEVEL] = MisaProgressbar_Create(1400,y,0,0,hParent,SYSTEM_ID_PROGLEVEL,"Level",MISAPROG_FONT24);
	return 0;
}

static U8 SlideDeleteItems()
{
	MisaProgressbar_Delete(hSystemItems[SYSTEM_PROGDRIVE]);
	MisaProgressbar_Delete(hSystemItems[SYSTEM_PROGTONE]);
	MisaProgressbar_Delete(hSystemItems[SYSTEM_PROGLEVEL]);
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
		GUI_SetTextMode(GUI_TEXTMODE_TRANS);
		x = WM_GetWindowSizeX(pMsg->hWin);
		y=0;
		GUI_DrawBitmap(&bmEMPTYTITLEBAR, (x-bmEMPTYTITLEBAR.XSize)/2+1, 0);
		GUI_SetFont(&GUI_Font32B_ASCII);
		GUI_DispStringHCenterAt("System slide window", x / 2, 5);
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
			case SYSTEM_ID_PROGDRIVE:
				break;
			default:
				;
			}
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
					WM_MoveWindow(pMsg->hWin,50-x,0);
				}
				else if(GUI_GetScreenSizeX()-INDICATORFRAME-y > x)
				{
					WM_MoveWindow(pMsg->hWin,750-y-x,0);
				}
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

#endif

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa initialize
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void MisaInitWidget()
{
	DefaultCheckboxProc = 0;
	DefaultRadioProc = 0;
	DefaultProgressbarProc = 0;
	DefaultmenuProc = 0;
	DefaultItemProc = 0;
}


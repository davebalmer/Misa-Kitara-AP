
#include <cstdlib>
#include <iostream>
#include <stdlib.h>

#include "GUI.h"
#include "WM.h"
#include "BUTTON.h"
#include "CHECKBOX.h"
#include "RADIO.h"

#include "MisaWidget.h"

#include "MisaAppInc.h"

#include "SystemScreen.h"
#include "PresetsScreen.h"
#include "SynthScreen.h"
#include "DistortionScreen.h"
#include "CompressionScreen.h"
#include "DelayScreen.h"
#include "ModulationScreen.h"
#include "ReverbScreen.h"
#include "MixerScreen.h"
#include "EqScreen.h"
#include "ScenesScreen.h"

#include "ucGuiMain.h"
#include "MainMenu.h"

#include "MisamenuConfig.h"
#include "MisaQuickSetScreen.h"

#include "Resource.h"

///////////////////////////////////////////////////////////////////////////////
//

#define MAINMENU_BUTTONS_XOFFSET 0
#define MAINMENU_BUTTONS_YOFFSET 6

// OR 23-11-11 : Several pages
static void SlideWindowProc(WM_MESSAGE* pMsg);
static void IndicatorProc(WM_MESSAGE* pMsg);
static const GUI_RECT leftsidebutton=
{
	0,2,39,498
};
static const GUI_RECT rightsidebutton=
{
	760,2,799,498
};
static U8 SlidingBorder;


///////////////////////////////////////////////////////////////////////////////
//

typedef enum __MAINMENUBUTTONS
{
	MAINMENU_PRESETS,
	MAINMENU_SCENES,
	MAINMENU_SYNTH_MIDI,
	MAINMENU_COMPRESSION,
	MAINMENU_DISTORTION,
	MAINMENU_BITCRUSHER,
	MAINMENU_MODULATION,
	MAINMENU_DELAY,
	MAINMENU_REVERB,
	MAINMENU_EQ,
	
	MAINMENUBUTTON_MAX
} MAINMENUBUTTONS;

typedef enum __MAINMENUITEMS
{
	MAINMENU_INDICATOR,		// OR  : Scene memories
	MAINMENU_SLIDEWINDOW,		// OR  : Scene memories
	MAINMENU_SYSTEM,
	MAINMENU_CLOSE,
	
	MAINMENUITEM_MAX
} MAINMENUITEMS;

typedef enum __MAINMENU_ID
{
	MAINMENU_ID_PRESETS = GUI_ID_MAINMENU_BASE,
	MAINMENU_ID_SCENES,
	MAINMENU_ID_SYNTH_MIDI,
	MAINMENU_ID_COMPRESSION,
	MAINMENU_ID_DISTORTION,
	MAINMENU_ID_BITCRUSHER,
	MAINMENU_ID_MODULATION,
	MAINMENU_ID_DELAY,
	MAINMENU_ID_REVERB,
	MAINMENU_ID_MIXER,
	MAINMENU_ID_EQ,
	MAINMENU_ID_SYSTEM,
	MAINMENU_ID_CLOSE,
	MAINMENU_ID_MAX
} MAINMENU_ID;

///////////////////////////////////////////////////////////////////////////////
//

static WM_HWIN hMainmenu;
static MAINMENU_PAINT_STATUS MainmenuPaintStatus;
static void MainMenuPaint(WM_MESSAGE* pMsg);
static void MainmenuProc(WM_MESSAGE* pMsg);
static BUTTON_Handle hMainmenuButtons[MAINMENUBUTTON_MAX];
static WM_HWIN hMainmenuItems[MAINMENUITEM_MAX];

///////////////////////////////////////////////////////////////////////////////
//

static GUI_COLOR MainMenuBKColor = GUI_GREEN;

static int lx,ly,cx,cy,dx,dy;

///////////////////////////////////////////////////////////////////////////////
//

U8 CreateMainMenu(void* pPara)
{
	int x,y;
	x=GUI_GetScreenSizeX();
	y=GUI_GetScreenSizeY();
	// OR 23-11-11 : Several pages
	SlidingBorder = 1;

	hMainmenu = WM_CreateWindow(0, 0, x, y, /*WM_CF_SHOW |*/ WM_CF_MEMDEV, MainmenuProc, 0);
	return 0;
}

U8 DeleteMainMenu()
{
	WM_DeleteWindow(hMainmenu);
	return 0;
}

U8 TopMainMenu(WM_HWIN hPreWin)
{
	if(hPreWin)
	{
		WM_HideWindow(hPreWin);
	}
	WM_ShowWindow(hMainmenu);
	//WM_BringToTop(hMainmenu);
	WM_ShowWindow(hMainmenuItems[MAINMENU_SLIDEWINDOW]);
	return 0;
}

static U8 MainmenuCreateItems(WM_HWIN hParent)
{
	int x,y;
	x = INDICATORFRAME;
	y = 0;

	// OR 23-11-11 : Several pages
	hMainmenuItems[MAINMENU_INDICATOR] = WM_CreateWindowAsChild(x, y, WM_GetWindowSizeX(hParent) - 2 * INDICATORFRAME, bmPRESETS_NORMAL.YSize, hParent, WM_CF_SHOW|WM_CF_MEMDEV, IndicatorProc,0);

	x = 1;
	y+=bmSYNTHMIDI_NORMAL.YSize;
	y+=MAINMENU_BUTTONS_YOFFSET;
	y+=2;
	hMainmenuItems[MAINMENU_SYSTEM] = BUTTON_CreateAsChild(x,y,bmMMSYSTEM_NORMAL.XSize,bmMMSYSTEM_NORMAL.YSize,
		hParent,MAINMENU_ID_SYSTEM,WM_CF_SHOW | WM_CF_MEMDEV);
	BUTTON_SetFocussable(hMainmenuItems[MAINMENU_SYSTEM],0);
	BUTTON_SetBitmap(hMainmenuItems[MAINMENU_SYSTEM],BUTTON_CI_UNPRESSED,&bmMMSYSTEM_NORMAL);
	BUTTON_SetBitmap(hMainmenuItems[MAINMENU_SYSTEM],BUTTON_CI_PRESSED,&bmMMSYSTEM_PRESSED);
	x+=bmMMSYSTEM_NORMAL.XSize;
	x += 4;

	hMainmenuItems[MAINMENU_CLOSE] = BUTTON_CreateAsChild(x,y,bmCLOSE_NORMAL.XSize,bmCLOSE_NORMAL.YSize,
		hParent,MAINMENU_ID_CLOSE,WM_CF_SHOW | WM_CF_MEMDEV);
	BUTTON_SetFocussable(hMainmenuItems[MAINMENU_CLOSE],0);
	BUTTON_SetBitmap(hMainmenuItems[MAINMENU_CLOSE],BUTTON_CI_UNPRESSED,&bmCLOSE_NORMAL);
	BUTTON_SetBitmap(hMainmenuItems[MAINMENU_CLOSE],BUTTON_CI_PRESSED,&bmCLOSE_PRESSED);
	
	return 0;
}

static U8 MainmenuDeleteItems()
{
	int i;

	for (i = 0; i < MAINMENUITEM_MAX; i++)
	{
		if(hMainmenuItems[i])
		{
			WM_DeleteWindow(hMainmenuItems[i]);
		}
	}
	return 0;
}

static void MainMenuPaint(WM_MESSAGE* pMsg)
{
	int x, y;
	char buf[64];
	switch(MainmenuPaintStatus)
	{
	case MAINMENU_PAINT_NORMAL:
		GUI_SetBkColor(MainMenuBKColor);
		GUI_Clear();
		GUI_SetColor(GUI_WHITE);
		GUI_SetFont(&GUI_Font24_ASCII);
		x = WM_GetWindowSizeX(pMsg->hWin);
		y = WM_GetWindowSizeY(pMsg->hWin);
		GUI_DispStringHCenterAt("Mainmenu", x / 2, (y / 2) - 12);
		break;
	case MAINMENU_PAINT_PENDOWN:
		GUI_SetBkColor(GUI_WHITE);
		GUI_Clear();
		GUI_SetColor(GUI_RED);
		GUI_SetFont(&GUI_Font24_ASCII);
		x = WM_GetWindowSizeX(pMsg->hWin);
		y = WM_GetWindowSizeY(pMsg->hWin);
		GUI_DispStringHCenterAt("Touch down!", x / 2, (y / 2) - 12);
		break;
	case MAINMENU_PAINT_PENMOVE:
		GUI_SetBkColor(GUI_BLUE);
		GUI_Clear();
		GUI_SetColor(GUI_RED);
		GUI_SetFont(&GUI_Font24_ASCII);
		x = WM_GetWindowSizeX(pMsg->hWin);
		y = WM_GetWindowSizeY(pMsg->hWin);
		::sprintf(buf,"Touch move! DX=%d,DY=%d",dx,dy);
		GUI_DispStringHCenterAt(buf, x / 2, (y / 2) - 80);
		::sprintf(buf,"Touch move! LX=%d,LY=%d",lx,ly);
		GUI_DispStringHCenterAt(buf, x / 2, (y / 2));
		::sprintf(buf,"Touch move! CX=%d,CY=%d",cx,cy);
		GUI_DispStringHCenterAt(buf, x / 2, (y / 2) + 80);
		break;
	case MAINMENU_PAINT_PENUP:
		GUI_SetBkColor(GUI_YELLOW);
		GUI_Clear();
		GUI_SetColor(GUI_RED);
		GUI_SetFont(&GUI_Font24_ASCII);
		x = WM_GetWindowSizeX(pMsg->hWin);
		y = WM_GetWindowSizeY(pMsg->hWin);
		GUI_DispStringHCenterAt("Touch up!", x / 2, (y / 2) - 12);
		break;
	}
}

static void MainmenuProc(WM_MESSAGE* pMsg)
{
	int x, y;
	int NCode,Id;
	static GUI_PID_STATE PID_LastState;
	//WM_HWIN hNext;
	//WM_Obj * pWin;
	
	// OR 23-11-11 : Several pages
	GUI_PID_STATE* pPID_State;
	static U8 sidebutton = 0;

	switch (pMsg->MsgId)
	{
	case WM_CREATE:
		PID_LastState.Pressed = 0;
		PID_LastState.x = 0;
		PID_LastState.y = 0;
		MainmenuCreateItems(pMsg->hWin);
		break;

	case WM_DELETE:
		MainmenuDeleteItems();
		DEBUG_STDOUT("Mainmenu window deleted!");
		break;

	case WM_PAINT:
	//	MainMenuPaint(pMsg);
		GUI_SetBkColor(GUI_BLACK);
		GUI_Clear();
				
		
		// OR 23-11-11 : Several pages
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

		y = WM_GetWindowOrgY(hMainmenuItems[MAINMENU_SLIDEWINDOW]);
		y += 230;
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
	case WM_NOTIFY_VIS_CHANGED:
		//pWin = WM_H2P(pMsg->hWin);
		//hNext = pWin->hNext;
		//SetLcdFlip(!hNext);
		break;

	case WM_TOUCH:
		// OR 23-11-11 : Several pages
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
						SlidingBorder = SlideGoPreviousPage(hMainmenuItems[MAINMENU_SLIDEWINDOW]);
						WM_InvalidateRect(pMsg->hWin, &leftsidebutton);
					}
					else if(GUI_RectsIntersect(&rightsidebutton,&ptRect))
					{
						sidebutton = 2;
						SlidingBorder = SlideGoNextPage(hMainmenuItems[MAINMENU_SLIDEWINDOW]);
						WM_InvalidateRect(pMsg->hWin, &rightsidebutton);
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

		break;

	case WM_NOTIFY_PARENT:
		Id = WM_GetId(pMsg->hWinSrc);
		NCode = pMsg->Data.v;
		switch(NCode)
		{
		case WM_NOTIFICATION_CLICKED:
			DEBUG_STDOUT("Mainmenu button clicked!");
			break;
		case WM_NOTIFICATION_RELEASED:
			switch(Id)
			{
			case MAINMENU_ID_SYSTEM:
				TopSystemScreen(pMsg->hWin);
				break;
			case MAINMENU_ID_CLOSE:
				// OR return to the Quick menu.  // TerminateApplication();
				WM_HideWindow(hMainmenu);
				BeginMisaquicksetScreen();
				break;
			default:
				;
			}
			DEBUG_STDOUT("Mainmenu button released!");
			break;
		}
		break;
	case WM_TIMER:
		DEBUG_STDOUT("WM_TIMER!");
		break;
	default:
		WM_DefaultProc(pMsg);
	}
}

// OR 23-11-11 : Several pages

static U8 IndicatorCreateItems(WM_HWIN hParent)
{
	hMainmenuItems[MAINMENU_SLIDEWINDOW] = WM_CreateWindowAsChild(0, 0, bmPRESETS_NORMAL.XSize * MAINMENUBUTTON_MAX, WM_GetWindowSizeY(hParent),hParent,WM_CF_SHOW|WM_CF_MEMDEV,SlideWindowProc,0);
	return 0;
}

static U8 IndicatorDeleteItems()
{
	if(hMainmenuItems[MAINMENU_SLIDEWINDOW])
	{
		WM_DeleteWindow(hMainmenuItems[MAINMENU_SLIDEWINDOW]);
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

// OR 23-11-11 : Several pages

static U8 SlideCreateItems(WM_HWIN hParent)
{
	int x,y;
	x = 0;
	y = 0;
	memset(hMainmenuButtons,0,sizeof(hMainmenuButtons));
	
	hMainmenuButtons[MAINMENU_PRESETS] = BUTTON_CreateAsChild(x,y,bmPRESETS_NORMAL.XSize, bmPRESETS_NORMAL.YSize,
		hParent,MAINMENU_ID_PRESETS,WM_CF_SHOW | WM_CF_MEMDEV);
	BUTTON_SetFocussable(hMainmenuButtons[MAINMENU_PRESETS],0);
	BUTTON_SetBitmap(hMainmenuButtons[MAINMENU_PRESETS],BUTTON_CI_UNPRESSED,&bmPRESETS_NORMAL);
	BUTTON_SetBitmap(hMainmenuButtons[MAINMENU_PRESETS],BUTTON_CI_PRESSED,&bmPRESETS_PRESSED);
	x+=bmPRESETS_NORMAL.XSize;
	x+=MAINMENU_BUTTONS_XOFFSET;

	hMainmenuButtons[MAINMENU_SCENES] = BUTTON_CreateAsChild(x,y, bmSCENES_NORMAL.XSize, bmSCENES_NORMAL.YSize,
		hParent, MAINMENU_ID_SCENES,WM_CF_SHOW | WM_CF_MEMDEV);
	BUTTON_SetFocussable(hMainmenuButtons[MAINMENU_SCENES],0);
	BUTTON_SetBitmap(hMainmenuButtons[MAINMENU_SCENES],BUTTON_CI_UNPRESSED,&bmSCENES_NORMAL);
	BUTTON_SetBitmap(hMainmenuButtons[MAINMENU_SCENES],BUTTON_CI_PRESSED,&bmSCENES_PRESSED);
	x+=bmEQ_NORMAL.XSize;
	x+=MAINMENU_BUTTONS_XOFFSET;

	hMainmenuButtons[MAINMENU_SYNTH_MIDI] = BUTTON_CreateAsChild(x,y,bmSYNTHMIDI_NORMAL.XSize,bmSYNTHMIDI_NORMAL.YSize,
			hParent,MAINMENU_ID_SYNTH_MIDI,WM_CF_SHOW | WM_CF_MEMDEV);
	BUTTON_SetFocussable(hMainmenuButtons[MAINMENU_SYNTH_MIDI],0);
	BUTTON_SetBitmap(hMainmenuButtons[MAINMENU_SYNTH_MIDI],BUTTON_CI_UNPRESSED,&bmSYNTHMIDI_NORMAL);
	BUTTON_SetBitmap(hMainmenuButtons[MAINMENU_SYNTH_MIDI],BUTTON_CI_PRESSED,&bmSYNTHMIDI_PRESSED);
	x+=bmSYNTHMIDI_NORMAL.XSize;
	x+=MAINMENU_BUTTONS_XOFFSET;

	hMainmenuButtons[MAINMENU_BITCRUSHER] = BUTTON_CreateAsChild(x,y,bmBITCRUSHER_NORMAL.XSize,bmBITCRUSHER_NORMAL.YSize,
		hParent,MAINMENU_ID_BITCRUSHER,WM_CF_SHOW | WM_CF_MEMDEV);
	BUTTON_SetFocussable(hMainmenuButtons[MAINMENU_BITCRUSHER],0);
	BUTTON_SetBitmap(hMainmenuButtons[MAINMENU_BITCRUSHER],BUTTON_CI_UNPRESSED,&bmBITCRUSHER_NORMAL);
	BUTTON_SetBitmap(hMainmenuButtons[MAINMENU_BITCRUSHER],BUTTON_CI_PRESSED,&bmBITCRUSHER_PRESSED);
	x+=bmBITCRUSHER_NORMAL.XSize;
	x+=MAINMENU_BUTTONS_XOFFSET;

	hMainmenuButtons[MAINMENU_COMPRESSION] = BUTTON_CreateAsChild(x,y,bmCOMPRESSION_NORMAL.XSize,bmCOMPRESSION_NORMAL.YSize,
		hParent,MAINMENU_ID_COMPRESSION,WM_CF_SHOW | WM_CF_MEMDEV);
	BUTTON_SetFocussable(hMainmenuButtons[MAINMENU_COMPRESSION],0);
	BUTTON_SetBitmap(hMainmenuButtons[MAINMENU_COMPRESSION],BUTTON_CI_UNPRESSED,&bmCOMPRESSION_NORMAL);
	BUTTON_SetBitmap(hMainmenuButtons[MAINMENU_COMPRESSION],BUTTON_CI_PRESSED,&bmCOMPRESSION_PRESSED);
	x+=bmCOMPRESSION_NORMAL.XSize;
	x+=MAINMENU_BUTTONS_XOFFSET;

	hMainmenuButtons[MAINMENU_DISTORTION] = BUTTON_CreateAsChild(x,y,bmDISTORTION_NORMAL.XSize,bmDISTORTION_NORMAL.YSize,
		hParent,MAINMENU_ID_DISTORTION,WM_CF_SHOW | WM_CF_MEMDEV);
	BUTTON_SetFocussable(hMainmenuButtons[MAINMENU_DISTORTION],0);
	BUTTON_SetBitmap(hMainmenuButtons[MAINMENU_DISTORTION],BUTTON_CI_UNPRESSED,&bmDISTORTION_NORMAL);
	BUTTON_SetBitmap(hMainmenuButtons[MAINMENU_DISTORTION],BUTTON_CI_PRESSED,&bmDISTORTION_PRESSED);
	x+=bmDISTORTION_NORMAL.XSize;
	x+=MAINMENU_BUTTONS_XOFFSET;

	hMainmenuButtons[MAINMENU_MODULATION] = BUTTON_CreateAsChild(x,y,bmMODULATION_NORMAL.XSize,bmMODULATION_NORMAL.YSize,
		hParent,MAINMENU_ID_MODULATION,WM_CF_SHOW | WM_CF_MEMDEV);
	BUTTON_SetFocussable(hMainmenuButtons[MAINMENU_MODULATION],0);
	BUTTON_SetBitmap(hMainmenuButtons[MAINMENU_MODULATION],BUTTON_CI_UNPRESSED,&bmMODULATION_NORMAL);
	BUTTON_SetBitmap(hMainmenuButtons[MAINMENU_MODULATION],BUTTON_CI_PRESSED,&bmMODULATION_PRESSED);
	x+=bmMODULATION_NORMAL.XSize;
	x+=MAINMENU_BUTTONS_XOFFSET;
	
	hMainmenuButtons[MAINMENU_DELAY] = BUTTON_CreateAsChild(x,y,bmDELAY_NORMAL.XSize,bmDELAY_NORMAL.YSize,
		hParent,MAINMENU_ID_DELAY,WM_CF_SHOW | WM_CF_MEMDEV);
	BUTTON_SetFocussable(hMainmenuButtons[MAINMENU_DELAY],0);
	BUTTON_SetBitmap(hMainmenuButtons[MAINMENU_DELAY],BUTTON_CI_UNPRESSED,&bmDELAY_NORMAL);
	BUTTON_SetBitmap(hMainmenuButtons[MAINMENU_DELAY],BUTTON_CI_PRESSED,&bmDELAY_PRESSED);
	x+=bmDELAY_NORMAL.XSize;
	x+=MAINMENU_BUTTONS_XOFFSET;
	
	hMainmenuButtons[MAINMENU_REVERB] = BUTTON_CreateAsChild(x,y,bmREVERB_NORMAL.XSize,bmREVERB_NORMAL.YSize,
		hParent,MAINMENU_ID_REVERB,WM_CF_SHOW | WM_CF_MEMDEV);
	BUTTON_SetFocussable(hMainmenuButtons[MAINMENU_REVERB],0);
	BUTTON_SetBitmap(hMainmenuButtons[MAINMENU_REVERB],BUTTON_CI_UNPRESSED,&bmREVERB_NORMAL);
	BUTTON_SetBitmap(hMainmenuButtons[MAINMENU_REVERB],BUTTON_CI_PRESSED,&bmREVERB_PRESSED);
	x+=bmREVERB_NORMAL.XSize;
	x+=MAINMENU_BUTTONS_XOFFSET;

	hMainmenuButtons[MAINMENU_EQ] = BUTTON_CreateAsChild(x,y,bmEQ_NORMAL.XSize,bmEQ_NORMAL.YSize,
		hParent,MAINMENU_ID_EQ,WM_CF_SHOW | WM_CF_MEMDEV);
	BUTTON_SetFocussable(hMainmenuButtons[MAINMENU_EQ],0);
	BUTTON_SetBitmap(hMainmenuButtons[MAINMENU_EQ],BUTTON_CI_UNPRESSED,&bmEQ_NORMAL);
	BUTTON_SetBitmap(hMainmenuButtons[MAINMENU_EQ],BUTTON_CI_PRESSED,&bmEQ_PRESSED);
	x+=bmEQ_NORMAL.XSize;
	x+=MAINMENU_BUTTONS_XOFFSET;

	return 0;
}

static U8 SlideDeleteItems()
{
	for(int i = 0; i < MAINMENU_INDICATOR; i++)
	{
		if(hMainmenuButtons[i])
			BUTTON_Delete(hMainmenuButtons[i]);
	}

	return 0;
}


static void SlideWindowProc(WM_MESSAGE* pMsg)
{
	int NCode,Id;
	std::vector<int> effect;
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
			case MAINMENU_ID_PRESETS:
				TopPresetsScreen(pMsg->hWin);
				break;
			case MAINMENU_ID_SYNTH_MIDI:
				TopSynthScreen(pMsg->hWin);
				break;
			case MAINMENU_ID_COMPRESSION:
				TopCompressionScreen(pMsg->hWin);
				break;
			case MAINMENU_ID_DISTORTION:
				TopDistortionScreen(pMsg->hWin);
				break;
			case MAINMENU_ID_BITCRUSHER:
				TopBitCrusherScreen(pMsg->hWin);
				break;
			case MAINMENU_ID_MODULATION:
				TopModulationScreen(pMsg->hWin);
				break;
			case MAINMENU_ID_DELAY:
				TopDelayScreen(pMsg->hWin);
				break;
			case MAINMENU_ID_REVERB:
				TopReverbScreen(pMsg->hWin);
				break;
			case MAINMENU_ID_EQ:
				TopMixerScreen(pMsg->hWin);
				break;

			case MAINMENU_ID_SCENES:
				TopScenesScreen(pMsg->hWin);
				break;

			default:
				;
			}
			break;
		}
		break;

	default:
		WM_DefaultProc(pMsg);
	}
}



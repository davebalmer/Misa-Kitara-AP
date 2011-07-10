
#include <cstdlib>
#include <iostream>

#if defined(Linux) && !defined(MISA_APPLICATION)
#include <Touchpanel.h>
#include <Neck.h>
#include <ControlScreen.h>
#include <Graphics.h>
#else
#include "Touchpanel.h"
#endif // Linux

#include "LCDFrameBuffer.h"

#include "GUI.h"
#include "WM.h"
#include "BUTTON.h"
#include "CHECKBOX.h"
#include "RADIO.h"

#include "MisaWidget.h"

#include "MisaAppInc.h"

#include "SystemScreen.h"
#include "MainMenu.h"
#include "PresetsScreen.h"
#include "SynthScreen.h"
#include "DistortionScreen.h"
#include "CompressionScreen.h"
#include "DelayScreen.h"
#include "ModulationScreen.h"
#include "ReverbScreen.h"
#include "MixerScreen.h"
#include "EqScreen.h"
#include "MisaQuickSetScreen.h"

#include "ucGuiMain.h"

U8 unThreadRun;

//#ifdef __cplusplus
//extern "C"
//{
//#endif
//void DisplayUpdate(void);
//#ifdef __cplusplus
//}
//#endif

///////////////////////////////////////////////////////////////////////////////
//

void GUI_X_Exit(void);
int LCD_L0_Exit(void);

///////////////////////////////////////////////////////////////////////////////
// Temporary defined here
#define MISA_LEFTHAND_MODE 0
#define MISA_RIGHTHAND_MODE 1

int MisaGetHandMode();
int MisaSetHandMode(int mode);

#ifdef Linux
U16 LCD_GetPix(int x, int y);
#else
unsigned int LCD_L0_GetPixelIndex(int x, int y);
#endif

#if 0
GUI_BITMAP Backgroud;

U8 KitaraInitBackPic()
{
	Backgroud.XSize = 800;
	Backgroud.YSize = 600;
	Backgroud.BytesPerLine = 1600;
	Backgroud.BitsPerPixel = 16;
	Backgroud.pData = 0;
	Backgroud.pPal = 0;
	Backgroud.pMethods = 0;
	return 0;
}

U8 KitrarCreateBackPic(int flip)
{
	int x,y;
	U16* pData;
	Backgroud.pData = (U8*)malloc(800*600*2);
	pData = (U16*)Backgroud.pData;
	if(0==Backgroud.pData)
	{
		exit(1);
	}
	for(y=0;y<600;y++)
	{
		for(x=0;x<800;x++)
		{
			#ifdef Linux
			pData[y*800+x] = LCD_GetPix(flip?799-x:x,flip?599-y:y);
			#else
			pData[y*800+x] = LCD_L0_GetPixelIndex(x,y);
			#endif
		}
	}
	return 0;
}

U8 KitrarDeleteBackPic()
{
	if(0==Backgroud.pData)
	{
		free((void*)Backgroud.pData);
		Backgroud.pData = 0;
	}
	return 0;
}

#endif

///////////////////////////////////////////////////////////////////////////////
//

static WM_CALLBACK* OldDesktopWindowProc;
static void DesktopWindowProc(WM_MESSAGE* pMsg);

///////////////////////////////////////////////////////////////////////////////
//

#ifndef MISA_APPLICATION
extern Touchpanel* pts;
#ifdef Linux
extern Neck* pnk;
extern ControlScreen* pcs;
extern Graphics *pGraph;
#endif // Linux
#endif // MISA_APPLICATION

//#define MISA_MESSAGETHREAD

#if defined(Linux) && defined(MISA_MESSAGETHREAD)

pthread_t MessageTID;

void* KitaraMessage(void* para)
{
	static int lastX,lastY;
	lastX = lastY = 0;
	while(unThreadRun)
	{
		struct touchstate tsState;
		GUI_PID_STATE pid_state;
		pts->getConfigMenuTouchState(&tsState);
		#if 0
		if(tsState.num_touches > tsState.old_num_touches)
		{
			pid_state.Pressed = 1;
			if(GetLcdFlip())
			{
				
				lastX = pid_state.x = 800-tsState.x[0];
				lastY = pid_state.y = 600-tsState.y[0];
			}
			else
			{
				lastX = pid_state.x = tsState.x[0];
				lastY = pid_state.y = tsState.y[0];
			}
			GUI_TOUCH_StoreStateEx(&pid_state);
		}
		else
		#endif
		if(0 == tsState.num_touches)
		{
			pid_state.Pressed = 0;
			pid_state.x = lastX;
			pid_state.y = lastY;
			GUI_TOUCH_StoreStateEx(&pid_state);
		}
		else
		{
			pid_state.Pressed = 1;
			if(GetLcdFlip())
			{
				
				lastX = pid_state.x = 800-tsState.x[0];
				lastY = pid_state.y = 600-tsState.y[0];
			}
			else
			{
				lastX = pid_state.x = tsState.x[0];
				lastY = pid_state.y = tsState.y[0];
			}
			GUI_TOUCH_StoreStateEx(&pid_state);
		}
#ifndef MISA_APPLICATION
		struct neckstate neck_state;
		pnk->getNeckState(&neck_state, MisaGetHandMode()?MISA_LEFTHAND_MODE:MISA_RIGHTHAND_MODE);
		pcs->playNotes(&neck_state);
#endif // MISA_APPLICATION
	}
	return 0;
}

#endif // defined(Linux) && defined(MISA_MESSAGETHREAD)

U32 KitaraInit()
{
	DEBUG_STDOUT("Start Main menu!");
	unThreadRun = 0;
#if defined(Linux) && !defined(USE_FRAMEBUFFER0)
	SetLcdFrameBuffer(pGraph->GetOffScreenBuffer());
#endif
	//KitaraInitBackPic();
	GUI_Init();
	MisaInitWidget();
///////////////////////////////////////////////////////////////////////////////
// cursor test code
	//GUI_CURSOR_Select(&GUI_CursorCrossL);
	//GUI_CURSOR_Show();
#ifdef MISA_APPLICATION
	Touchpanel tp;
	Touchpanel* pts=&tp;
#endif // MISA_APPLICATION
///////////////////////////////////
	GUI_SetBkColor(GUI_BLACK);
	GUI_Clear();
	WM_SetDesktopColor(GUI_DARKGRAY);
///////////////////////////////////////////////////////////////////////////////
//
	OldDesktopWindowProc = WM_SetCallback(WM_HBKWIN, DesktopWindowProc);
///////////////////////////////////////////////////////////////////////////////
// Create menu windows
	CreateMisaquicksetScreen(0);
	CreateMainMenu(0);
	CreateSystemScreen(0);
	CreatePresetsScreen(0);
	CreateSynthScreen(0);
	CreateDistortionScreen(0);
	CreateCompressionScreen(0);
	CreateDelayScreen(0);
	CreateModulationScreen(0);
	CreateReverbScreen(0);
	CreateMixerScreen(0);
	CreateEqScreen(0);
	MisaSetcontrolmenu_Create();
	
///////////////////////////////////////////////////////////////////////////////
// Update inforrmations to screens
	UpdateSynthSetting();
	UpdateSystemInfo();
	RunFramework();
	WM_Deactivate();

	SetLcdFlip(0); //always initialise in this position (required if USB plug pulled during menu display)
	return 0;
}

U32 KitaraExit()
{
	#ifdef MISA_APPLICATION
	tp.WaitForExit();
#endif // MISA_APPLICATION
///////////////////////////////////////////////////////////////////////////////
// Delete menu windows
	MisaSetcontrolmenu_Delete();
	RunFramework();
	DeleteMixerScreen();
	RunFramework();
	DeleteEqScreen();
	RunFramework();
	DeleteReverbScreen();
	RunFramework();
	DeleteModulationScreen();
	RunFramework();
	DeleteDelayScreen();
	RunFramework();
	DeleteCompressionScreen();
	RunFramework();
	DeleteDistortionScreen();
	RunFramework();
	DeleteSynthScreen();
	RunFramework();
	DeletePresetsScreen();
	RunFramework();
	DeleteSystemScreen();
	RunFramework();
	DeleteMainMenu();
	RunFramework();
	DeleteMisaquicksetScreen();
	RunFramework();
///////////////////////////////////////////////////////////////////////////////
//
	WM_SetCallback(WM_HBKWIN, OldDesktopWindowProc);
	RunFramework();
	DEBUG_STDOUT("Exit Main menu!");
	LCD_L0_Exit();
	return 0;
}

U32 KitaraMenu()
{
	static int lastX,lastY;
	lastX = lastY = 0;
	unThreadRun = 1;
///////////////////////////////////////////////////////////////////////////////
//
	//KitrarCreateBackPic();
///////////////////////////////////////////////////////////////////////////////
	MisaSetHandMode(MisaGetHandMode());
	WM_Activate();
///////////////////////////////////////////////////////////////////////////////
// Brgin mainmenu to top
	//EnterMainMenu();
	BeginMisaquicksetScreen();
#if defined(Linux) && defined(MISA_MESSAGETHREAD)
	//pthread_create(&MessageTID,0,KitaraMessage,0);
#endif // defined(Linux) && defined(MISA_MESSAGETHREAD)
	while(unThreadRun)
	{
		//GUI_Exec();
		//GUI_X_ExecIdle();
#if defined(Linux) && !defined(MISA_MESSAGETHREAD)
		struct touchstate tsState;
		GUI_PID_STATE pid_state;
		pts->getConfigMenuTouchState(&tsState);
		#if 0
		if(tsState.num_touches > tsState.old_num_touches)
		{
			pid_state.Pressed = 1;
			if(GetLcdFlip())
			{
				
				lastX = pid_state.x = 800-tsState.x[0];
				lastY = pid_state.y = 600-tsState.y[0];
			}
			else
			{
				lastX = pid_state.x = tsState.x[0];
				lastY = pid_state.y = tsState.y[0];
			}
			GUI_TOUCH_StoreStateEx(&pid_state);
		}
		else
		#endif
		if(0 == tsState.num_touches)
		{
			pid_state.Pressed = 0;
			pid_state.x = lastX;
			pid_state.y = lastY;
			GUI_TOUCH_StoreStateEx(&pid_state);
		}
		else
		{
			pid_state.Pressed = 1;
			if(GetLcdFlip())
			{
				
				lastX = pid_state.x = 800-tsState.x[0];
				lastY = pid_state.y = 600-tsState.y[0];
			}
			else
			{
				lastX = pid_state.x = tsState.x[0];
				lastY = pid_state.y = tsState.y[0];
			}
			GUI_TOUCH_StoreStateEx(&pid_state);
		}
	#ifndef MISA_APPLICATION
		struct neckstate neck_state;
		pnk->getNeckState(&neck_state, MisaGetHandMode()?MISA_LEFTHAND_MODE:MISA_RIGHTHAND_MODE);
		pcs->playNotes(&neck_state);
	#endif // MISA_APPLICATION
#endif // defined(Linux) && !defined(MISA_MESSAGETHREAD)
#ifdef Linux
		RunFramework();
#else
		GUI_Delay(15);
#endif
	}
#if defined(Linux) && defined(MISA_MESSAGETHREAD)
	//pthread_join(MessageTID,NULL);
#endif // defined(Linux) && defined(MISA_MESSAGETHREAD)
	ExitMainMenu();
	EndMisaquicksetScreen();
	GUI_SetColor(GUI_BLACK);
	GUI_Clear();
	RunFramework();
	WM_Deactivate();
	if(MISA_RIGHTHAND_MODE==MisaGetHandMode())
	{
		SetLcdFlip(!MisaGetHandMode());
	}
///////////////////////////////////////////////////////////////////////////////
//
	//KitrarDeleteBackPic();
///////////////////////////////////////////////////////////////////////////////
	return 0;
}

void KitaraStep()
{
	static int lastX,lastY;
#if defined(Linux) && !defined(MISA_MESSAGETHREAD)
	struct touchstate tsState;
	GUI_PID_STATE pid_state;
	pts->getConfigMenuTouchState(&tsState);
	#if 0
	if(tsState.num_touches > tsState.old_num_touches)
	{
		pid_state.Pressed = 1;
		if(GetLcdFlip())
		{
			
			lastX = pid_state.x = 800-tsState.x[0];
			lastY = pid_state.y = 600-tsState.y[0];
		}
		else
		{
			lastX = pid_state.x = tsState.x[0];
			lastY = pid_state.y = tsState.y[0];
		}
		GUI_TOUCH_StoreStateEx(&pid_state);
	}
	else
	#endif
	if(0 == tsState.num_touches)
	{
		pid_state.Pressed = 0;
		pid_state.x = lastX;
		pid_state.y = lastY;
		GUI_TOUCH_StoreStateEx(&pid_state);
	}
	else
	{
		pid_state.Pressed = 1;
		if(GetLcdFlip())
		{
			
			lastX = pid_state.x = 800-tsState.x[0];
			lastY = pid_state.y = 600-tsState.y[0];
		}
		else
		{
			lastX = pid_state.x = tsState.x[0];
			lastY = pid_state.y = tsState.y[0];
		}
		GUI_TOUCH_StoreStateEx(&pid_state);
	}
#ifndef MISA_APPLICATION
	struct neckstate neck_state;
	pnk->getNeckState(&neck_state, MisaGetHandMode()?MISA_LEFTHAND_MODE:MISA_RIGHTHAND_MODE);
	pcs->playNotes(&neck_state);
#endif // MISA_APPLICATION
#endif // defined(Linux) && !defined(MISA_MESSAGETHREAD)
	RunFramework();
}

U32 ucGuiMain(void* pPara)
{
	unThreadRun = 1;
	DEBUG_STDOUT("Start Main menu!");
#if defined(Linux) && !defined(USE_FRAMEBUFFER0)
	SetLcdFrameBuffer(pGraph->GetOffScreenBuffer());
#endif
	//KitaraInitBackPic();
	GUI_Init();
	MisaInitWidget();
///////////////////////////////////////////////////////////////////////////////
// cursor test code
	//GUI_CURSOR_Select(&GUI_CursorCrossL);
	//GUI_CURSOR_Show();
#ifdef MISA_APPLICATION
	Touchpanel tp;
	Touchpanel* pts=&tp;
#endif // MISA_APPLICATION
///////////////////////////////////
	GUI_SetBkColor(GUI_BLACK);
	GUI_Clear();
	SetLcdFlip(1);
	WM_SetDesktopColor(GUI_DARKGRAY);
///////////////////////////////////////////////////////////////////////////////
//
	OldDesktopWindowProc = WM_SetCallback(WM_HBKWIN, DesktopWindowProc);
///////////////////////////////////////////////////////////////////////////////
// Create menu windows
	CreateMisaquicksetScreen(0);
	CreateMainMenu(0);
	CreateSystemScreen(0);
	CreatePresetsScreen(0);
	CreateSynthScreen(0);
	CreateDistortionScreen(0);
	CreateCompressionScreen(0);
	CreateDelayScreen(0);
	CreateModulationScreen(0);
	CreateReverbScreen(0);
	CreateMixerScreen(0);
	CreateEqScreen(0);
	MisaSetcontrolmenu_Create();
///////////////////////////////////////////////////////////////////////////////
// Brgin mainmenu to top
	//TopMainMenu(0);
	BeginMisaquicksetScreen();
///////////////////////////////////////////////////////////////////////////////
// Update inforrmations to screens
	UpdateSynthSetting();
	UpdateSystemInfo();
///////////////////////////////////////////////////////////////////////////////
// Message loop
	static int lastX,lastY;
	lastX = lastY = 0;
	while(unThreadRun)
	{
		//GUI_Exec();
		//GUI_X_ExecIdle();
#ifndef WIN32
		struct touchstate tsState;
		GUI_PID_STATE pid_state;
		pts->getConfigMenuTouchState(&tsState);
		if(tsState.num_touches > tsState.old_num_touches)
		{
			pid_state.Pressed = 1;
			if(GetLcdFlip())
			{
				
				lastX = pid_state.x = 800-tsState.x[0];
				lastY = pid_state.y = 600-tsState.y[0];
			}
			else
			{
				lastX = pid_state.x = tsState.x[0];
				lastY = pid_state.y = tsState.y[0];
			}
			GUI_TOUCH_StoreStateEx(&pid_state);
		}
		else if(0 == tsState.num_touches)
		{
			pid_state.Pressed = 0;
			pid_state.x = lastX;
			pid_state.y = lastY;
			GUI_TOUCH_StoreStateEx(&pid_state);
		}
		else
		{
			pid_state.Pressed = 1;
			if(GetLcdFlip())
			{
				
				lastX = pid_state.x = 800-tsState.x[0];
				lastY = pid_state.y = 600-tsState.y[0];
			}
			else
			{
				lastX = pid_state.x = tsState.x[0];
				lastY = pid_state.y = tsState.y[0];
			}
			GUI_TOUCH_StoreStateEx(&pid_state);
		}
	#ifndef MISA_APPLICATION
		struct neckstate neck_state;
		pnk->getNeckState(&neck_state, MisaGetHandMode()?MISA_LEFTHAND_MODE:MISA_RIGHTHAND_MODE);
		pcs->playNotes(&neck_state);
	#endif // MISA_APPLICATION
#endif
#ifdef Linux
		RunFramework();
#else
		GUI_Delay(15);
#endif
	}
///////////////////////////////////////////////////////////////////////////////
// cursor test code
	//GUI_CURSOR_Hide();
	// Wait tp quit.
#ifdef MISA_APPLICATION
	tp.WaitForExit();
#endif // MISA_APPLICATION
///////////////////////////////////////////////////////////////////////////////
// Delete menu windows
	MisaSetcontrolmenu_Delete();
	RunFramework();
	DeleteMixerScreen();
	RunFramework();
	DeleteEqScreen();
	RunFramework();
	DeleteReverbScreen();
	RunFramework();
	DeleteModulationScreen();
	RunFramework();
	DeleteDelayScreen();
	RunFramework();
	DeleteCompressionScreen();
	RunFramework();
	DeleteDistortionScreen();
	RunFramework();
	DeleteSynthScreen();
	RunFramework();
	DeletePresetsScreen();
	RunFramework();
	DeleteSystemScreen();
	RunFramework();
	DeleteMainMenu();
	RunFramework();
	DeleteMisaquicksetScreen();
	RunFramework();
///////////////////////////////////////////////////////////////////////////////
//
	WM_SetCallback(WM_HBKWIN, OldDesktopWindowProc);
	RunFramework();
	DEBUG_STDOUT("Exit Main menu!");
	SetLcdFlip(0);
	LCD_L0_Exit();
#if 0//ndef WIN32
	GUI_X_Exit();
#endif
	return 0;
}

static void DesktopWindowProc(WM_MESSAGE* pMsg)
{
	int NCode,Id;
	static GUI_PID_STATE TouchState;
	static GUI_RECT rect;
	switch (pMsg->MsgId)
	{
	case WM_PAINT:
#ifdef WIN32
		GUI_SetColor(GUI_BLACK);
		GUI_Clear();
#endif
		break;
	case WM_TOUCH:
		if(pMsg->Data.p)
		{
			TouchState = *(GUI_PID_STATE*)pMsg->Data.p;
		}
		break;
	case WM_CUST_CHANGE_TEXT:
		break;
	case WM_NOTIFY_PARENT:
		Id = WM_GetId(pMsg->hWinSrc);
		NCode = pMsg->Data.v;
		switch(NCode)
		{
		case WM_NOTIFICATION_CLICKED:
			DEBUG_STDOUT("button clicked!");
			break;
		case WM_NOTIFICATION_RELEASED:
			if(Id == GUI_ID_CANCEL)
			{
				TerminateApplication();
				DEBUG_STDOUT("button released!");
			}
			break;
		}
		break;
	default:
		WM_DefaultProc(pMsg);
	}
}

void TerminateApplication()
{
	unThreadRun = 0;
}

#ifdef D2DUPDATE

void D2D_Bitblt(int x0, int y0, int width, int heigh);

void RunFramework(void)
{
	if(GUI_Exec())
	{
		D2D_Bitblt(0,0,800,600);
	}
	GUI_X_ExecIdle();
}

void Misa_Delay(int ms)
{
	ms /= 5;
	while(ms--)
	{
		if(GUI_Exec())
		{
			D2D_Bitblt(0,0,800,600);
		}
		GUI_X_Delay(5);
	}
}

#else

void RunFramework(void)
{
	GUI_Exec();
	GUI_X_ExecIdle();
}

void Misa_Delay(int ms)
{
	GUI_Delay(ms);
}

#endif

#ifndef USE_FRAMEBUFFER0

void DisplayUpdate(void)
{
#ifdef Linux
	pGraph->flip();
#endif
}

#endif


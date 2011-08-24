
#include <cstdlib>
#include <iostream>
using namespace std;
#include <string>

#include "GUI.h"
#include "WM.h"
#include "BUTTON.h"
#include "TEXT.h"

#include "MisaWidget.h"
#include "MisaDialogueBox.h"

#include "MisaAppInc.h"

#include "MainMenu.h"
#include "SystemScreen.h"

#include "MisamenuConfig.h"

#include "Resource.h"

///////////////////////////////////////////////////////////////////////////////
//

#define SYSTEM_TYPE_XPOS 80
#define SYSTEM_TYPE_YPOS 120

#define SYSTEM_XOFFSET 2
#define SYSTEM_YOFFSET 2

///////////////////////////////////////////////////////////////////////////////
//

typedef enum __SYSTEMITEMS
{
	SYSTEM_INFOLABEL,
	SYSTEM_UPDATESOUNDBANK,
	SYSTEM_LEFTHANDSWITCH,
	SYSTEM_RIGHTHANDSWITCH,
	SYSTEM_BALLTRAVEL,
	SYSTEM_LEFTHANDLABEL,
	SYSTEM_RIGHTHANDLABEL,
	SYSTEM_BALLTRAVELLABEL,
	SYSTEM_ENABLESUSTAIN,
//	SYSTEM_SHOWSTRINGS,
	SYSTEM_ENABLESUSTAINLABEL,
//	SYSTEM_SHOWSTRINGSLABEL,
	SYSTEM_MAX
} SYSTEMITEMS;

typedef enum __SYSTEM_ID
{
	SYSTEM_ID_UPDATESOUNDBANK=GUI_ID_SYSTEM_BASE,
	SYSTEM_ID_LEFTHANDSWITCH,
	SYSTEM_ID_RIGHTHANDSWITCH,
	SYSTEM_ID_BALLTRAVEL,
	SYSTEM_ID_TEXT,
	SYSTEM_ID_ENABLESUSTAIN,
//	SYSTEM_ID_SHOWSTRINGS,
	SYSTEM_ID_MAX
} SYSTEM_ID;

///////////////////////////////////////////////////////////////////////////////
//

static WM_HWIN hSystem;
static void SystemProc(WM_MESSAGE* pMsg);
static WM_HWIN hSystemItems[SYSTEM_MAX];
static COMMCOMPONENT hCOMPcomm;

///////////////////////////////////////////////////////////////////////////////
//

static U8 SystemCreateItems(WM_HWIN hParent)
{
	int x,y;
	hCOMPcomm = CreateCommComponentEx(hParent,COMMON_ITEM_CLOSE);//|COMMON_ITEM_FX1|COMMON_ITEM_FX2
	memset(hSystemItems,0,sizeof(hSystemItems));
	x = 0;
	y = bmEMPTYTITLEBAR.YSize;
	hSystemItems[SYSTEM_INFOLABEL] = TEXT_CreateAsChild(x,y,800,250,hParent,SYSTEM_ID_TEXT,WM_CF_SHOW|WM_CF_MEMDEV,"System Information",GUI_TA_HCENTER|GUI_TA_VCENTER);
	TEXT_SetFont(hSystemItems[SYSTEM_INFOLABEL],&GUI_Font24B_ASCII);
	TEXT_SetTextColor(hSystemItems[SYSTEM_INFOLABEL],GUI_WHITE);
	x = SYSTEM_TYPE_XPOS;
	y += 250;
	hSystemItems[SYSTEM_LEFTHANDSWITCH] = MisaRadio_Create(x,y,bmSM_WF_UN.XSize,bmSM_WF_UN.YSize,hParent,SYSTEM_ID_LEFTHANDSWITCH,WM_CF_SHOW | WM_CF_MEMDEV,&bmSM_WF_UN,&bmSM_WF_SE);
	//hSystemItems[SYSTEM_LEFTHANDSWITCH] = BUTTON_CreateAsChild(x,y,bmFX1_NORMAL.XSize,bmFX1_NORMAL.YSize,hParent,SYSTEM_ID_LEFTHANDSWITCH,WM_CF_SHOW|WM_CF_MEMDEV);
	//BUTTON_SetFocussable(hSystemItems[SYSTEM_LEFTHANDSWITCH],0);
	//BUTTON_SetBitmap(hSystemItems[SYSTEM_LEFTHANDSWITCH],BUTTON_CI_UNPRESSED,&bmFX1_NORMAL);
	//BUTTON_SetBitmap(hSystemItems[SYSTEM_LEFTHANDSWITCH],BUTTON_CI_PRESSED,&bmFX1_PRESSED);
	x += bmSM_WF_UN.XSize;
	hSystemItems[SYSTEM_LEFTHANDLABEL] = TEXT_CreateAsChild(x,y,150,bmSM_WF_UN.YSize,hParent,SYSTEM_ID_TEXT,WM_CF_SHOW|WM_CF_MEMDEV,"Left handed",GUI_TA_HCENTER|GUI_TA_VCENTER);
	TEXT_SetFont(hSystemItems[SYSTEM_LEFTHANDLABEL],&GUI_Font24B_ASCII);
	TEXT_SetTextColor(hSystemItems[SYSTEM_LEFTHANDLABEL],GUI_WHITE);
	x += 150;
	hSystemItems[SYSTEM_RIGHTHANDSWITCH] = MisaRadio_Create(x,y,bmSM_WF_UN.XSize,bmSM_WF_UN.YSize,hParent,SYSTEM_ID_RIGHTHANDSWITCH,WM_CF_SHOW | WM_CF_MEMDEV,&bmSM_WF_UN,&bmSM_WF_SE);
	//hSystemItems[SYSTEM_RIGHTHANDSWITCH] = BUTTON_CreateAsChild(x,y,bmFX2_NORMAL.XSize,bmFX2_NORMAL.YSize,hParent,SYSTEM_ID_RIGHTHANDSWITCH,WM_CF_SHOW|WM_CF_MEMDEV);
	//BUTTON_SetFocussable(hSystemItems[SYSTEM_RIGHTHANDSWITCH],0);
	//BUTTON_SetBitmap(hSystemItems[SYSTEM_RIGHTHANDSWITCH],BUTTON_CI_UNPRESSED,&bmFX2_NORMAL);
	//BUTTON_SetBitmap(hSystemItems[SYSTEM_RIGHTHANDSWITCH],BUTTON_CI_PRESSED,&bmFX2_PRESSED);
	x += bmSM_WF_UN.XSize;
	hSystemItems[SYSTEM_RIGHTHANDLABEL] = TEXT_CreateAsChild(x,y,150,bmSM_WF_UN.YSize,hParent,SYSTEM_ID_TEXT,WM_CF_SHOW|WM_CF_MEMDEV,"Right handed",GUI_TA_HCENTER|GUI_TA_VCENTER);
	TEXT_SetFont(hSystemItems[SYSTEM_RIGHTHANDLABEL],&GUI_Font24B_ASCII);
	TEXT_SetTextColor(hSystemItems[SYSTEM_RIGHTHANDLABEL],GUI_WHITE);

	x+= 160;
	hSystemItems[SYSTEM_ENABLESUSTAINLABEL] = TEXT_CreateAsChild(x+50,y,200,bmSM_WF_UN.YSize,hParent,SYSTEM_ID_TEXT,WM_CF_SHOW|WM_CF_MEMDEV,"Sustain Mode",GUI_TA_HCENTER|GUI_TA_VCENTER);
	TEXT_SetFont(hSystemItems[SYSTEM_ENABLESUSTAINLABEL],&GUI_Font24B_ASCII);
	TEXT_SetTextColor(hSystemItems[SYSTEM_ENABLESUSTAINLABEL],GUI_WHITE);
	hSystemItems[SYSTEM_ENABLESUSTAIN] = MisaCheckbox_Create(x,y,bmSM_WF_UN.XSize,bmSM_WF_UN.YSize,hParent,SYSTEM_ID_ENABLESUSTAIN,WM_CF_SHOW | WM_CF_MEMDEV,&bmSM_WF_UN,&bmSM_WF_SE);

	y += bmSM_WF_UN.YSize;
	y += 10;
/*	hSystemItems[SYSTEM_SHOWSTRINGSLABEL] = TEXT_CreateAsChild(x+20,y,200,bmSM_WF_UN.YSize,hParent,SYSTEM_ID_TEXT,WM_CF_SHOW|WM_CF_MEMDEV,"Strings",GUI_TA_HCENTER|GUI_TA_VCENTER);
	TEXT_SetFont(hSystemItems[SYSTEM_SHOWSTRINGSLABEL],&GUI_Font24B_ASCII);
	TEXT_SetTextColor(hSystemItems[SYSTEM_SHOWSTRINGSLABEL],GUI_WHITE);
	hSystemItems[SYSTEM_SHOWSTRINGS] = MisaCheckbox_Create(x,y,bmSM_WF_UN.XSize,bmSM_WF_UN.YSize,hParent,SYSTEM_ID_SHOWSTRINGS,WM_CF_SHOW | WM_CF_MEMDEV,&bmSM_WF_UN,&bmSM_WF_SE);
*/

	x = SYSTEM_TYPE_XPOS;
	hSystemItems[SYSTEM_BALLTRAVELLABEL] = TEXT_CreateAsChild(x,y,200,bmSM_WF_UN.YSize,hParent,SYSTEM_ID_TEXT,WM_CF_SHOW|WM_CF_MEMDEV,"Ball control travel:",GUI_TA_HCENTER|GUI_TA_VCENTER);
	TEXT_SetFont(hSystemItems[SYSTEM_BALLTRAVELLABEL],&GUI_Font24B_ASCII);
	TEXT_SetTextColor(hSystemItems[SYSTEM_BALLTRAVELLABEL],GUI_WHITE);
	x += 222;
	hSystemItems[SYSTEM_BALLTRAVEL] = MisaCheckbox_Create(x,y,bmSM_WF_UN.XSize,bmSM_WF_UN.YSize,hParent,SYSTEM_ID_BALLTRAVEL,WM_CF_SHOW | WM_CF_MEMDEV,&bmSM_WF_UN,&bmSM_WF_SE);
	x = 2;
	y = 502;
	hSystemItems[SYSTEM_UPDATESOUNDBANK] = BUTTON_CreateAsChild(x,y,bmCM_UPWA_UN.XSize,bmCM_UPWA_UN.YSize,hParent,SYSTEM_ID_UPDATESOUNDBANK,WM_CF_SHOW|WM_CF_MEMDEV);
	BUTTON_SetFocussable(hSystemItems[SYSTEM_UPDATESOUNDBANK],0);
	BUTTON_SetBitmap(hSystemItems[SYSTEM_UPDATESOUNDBANK],BUTTON_CI_UNPRESSED,&bmCM_UPWA_UN);
	BUTTON_SetBitmap(hSystemItems[SYSTEM_UPDATESOUNDBANK],BUTTON_CI_PRESSED,&bmCM_UPWA_SE);
	if(MISA_LEFTHAND_MODE==MisaGetHandMode())
	{
		MisaRadio_SetStatus(hSystemItems[SYSTEM_LEFTHANDSWITCH],1);
		MisaRadio_SetStatus(hSystemItems[SYSTEM_RIGHTHANDSWITCH],0);
	}
	else
	{
		MisaRadio_SetStatus(hSystemItems[SYSTEM_LEFTHANDSWITCH],0);
		MisaRadio_SetStatus(hSystemItems[SYSTEM_RIGHTHANDSWITCH],1);
	}

	MisaRadio_SetStatus(hSystemItems[SYSTEM_BALLTRAVEL], MisaGetBallTravel());
	MisaRadio_SetStatus(hSystemItems[SYSTEM_ENABLESUSTAIN], MisaGetEnableSustain());

	return 0;
}

static U8 SystemDeleteItems()
{
	DeleteCommComponent(hCOMPcomm);
	if(hSystemItems[SYSTEM_INFOLABEL])
	{
		TEXT_Delete(hSystemItems[SYSTEM_INFOLABEL]);
	}
	if(hSystemItems[SYSTEM_UPDATESOUNDBANK])
	{
		BUTTON_Delete(hSystemItems[SYSTEM_UPDATESOUNDBANK]);
	}
	if(hSystemItems[SYSTEM_LEFTHANDSWITCH])
	{
		MisaRadio_Delete(hSystemItems[SYSTEM_LEFTHANDSWITCH]);
	}
	if(hSystemItems[SYSTEM_RIGHTHANDSWITCH])
	{
		MisaRadio_Delete(hSystemItems[SYSTEM_RIGHTHANDSWITCH]);
	}
	if(hSystemItems[SYSTEM_BALLTRAVEL])
	{
		MisaCheckbox_Delete(hSystemItems[SYSTEM_BALLTRAVEL]);
	}
	if(hSystemItems[SYSTEM_ENABLESUSTAIN])
	{
		MisaCheckbox_Delete(hSystemItems[SYSTEM_ENABLESUSTAIN]);
	}
/*	if(hSystemItems[SYSTEM_SHOWSTRINGS])
	{
		MisaCheckbox_Delete(hSystemItems[SYSTEM_SHOWSTRINGS]);
	}
*/	if(hSystemItems[SYSTEM_LEFTHANDLABEL])
	{
		TEXT_Delete(hSystemItems[SYSTEM_LEFTHANDLABEL]);
	}
	if(hSystemItems[SYSTEM_RIGHTHANDLABEL])
	{
		TEXT_Delete(hSystemItems[SYSTEM_RIGHTHANDLABEL]);
	}
	if(hSystemItems[SYSTEM_BALLTRAVELLABEL])
	{
		TEXT_Delete(hSystemItems[SYSTEM_BALLTRAVELLABEL]);
	}
	if(hSystemItems[SYSTEM_ENABLESUSTAINLABEL])
	{
		TEXT_Delete(hSystemItems[SYSTEM_ENABLESUSTAINLABEL]);
	}
/*	if(hSystemItems[SYSTEM_SHOWSTRINGSLABEL])
	{
		TEXT_Delete(hSystemItems[SYSTEM_SHOWSTRINGSLABEL]);
	}
*/	return 0;
}

U8 CreateSystemScreen(void* pPara)
{
	int x,y;
	x=GUI_GetScreenSizeX();
	y=GUI_GetScreenSizeY();
	hSystem = WM_CreateWindow(0, 0, x, y, /*WM_CF_SHOW | */WM_CF_MEMDEV, SystemProc, 0);
	return 0;
}

U8 DeleteSystemScreen()
{
	SystemDeleteItems();
	WM_DeleteWindow(hSystem);
	return 0;
}

U8 TopSystemScreen(WM_HWIN hPreWin)
{
	WM_HideWindow(hPreWin);
	WM_ShowWindow(hSystem);
	//WM_BringToTop(hSystem);
	return 0;
}

U8 UpdateSystemInfo()
{
#ifdef Linux
	string sysinfo;
	string getSystemInfo(void);
	sysinfo = getSystemInfo();
	TEXT_SetText(hSystemItems[SYSTEM_INFOLABEL], sysinfo.c_str());
#endif
	return 0;
}

static void SystemProc(WM_MESSAGE* pMsg)
{
	int x, y;
	static bool state_updated = false;
	int NCode,Id;
	ProcessCommComponent(pMsg, &hCOMPcomm);
	switch (pMsg->MsgId)
	{
	case WM_CREATE:
		SystemCreateItems(pMsg->hWin);
		break;
	case WM_DELETE:
		SystemDeleteItems();
		break;
	case WM_PAINT:
		GUI_SetBkColor(GUI_BLACK);
		GUI_Clear();
		GUI_SetTextMode(GUI_TEXTMODE_TRANS);
		x=1;
		y=0;
		GUI_DrawBitmap(&bmEMPTYTITLEBAR, 0, 0);
		x = 222;
		y += bmEMPTYTITLEBAR.YSize;
		y += SYSTEM_YOFFSET;
		GUI_SetFont(&GUI_Font32B_ASCII);
		x = WM_GetWindowSizeX(pMsg->hWin);
		GUI_DispStringHCenterAt("System", x / 2, 5);
		break;
	case WM_NOTIFY_PARENT:
		if(DispatchCommComponent(pMsg,&hCOMPcomm))
		{
			Id = WM_GetId(pMsg->hWinSrc);
			NCode = pMsg->Data.v;
			switch(NCode)
			{
			case WM_NOTIFICATION_CLICKED:
				DEBUG_STDOUT("SYSTEM button clicked!");
				break;
			case WM_NOTIFICATION_RELEASED:
				switch(Id)
				{
				case COMMON_ID_CLOSE:
					if(state_updated)
						SaveToConfigFile();
					state_updated = false;
					break;
				case SYSTEM_ID_UPDATESOUNDBANK:
					if(GUI_ID_OK == Misa_ConfirmBox("Are you sure?","Delete this voice!",GUI_MESSAGEBOX_CF_MODAL))
					{
						Misa_WarningMessageBegin();
						RunFramework();
						x = UpdateSoundbank();
						Misa_WarningMessageEnd();
						RunFramework();
						switch(x)
						{
						case 0:
							Misa_WarningBox("Done.", "Update successful.", GUI_MESSAGEBOX_CF_MODAL);
							break;
						case 0x100:
						case 0x200:
							Misa_WarningBox("Cannot open device!", "", GUI_MESSAGEBOX_CF_MODAL);
							break;
						case 0x300:
							Misa_WarningBox("Cannot open file!", "", GUI_MESSAGEBOX_CF_MODAL);
							break;
						case 0x400:
							Misa_WarningBox("Time out!", "", GUI_MESSAGEBOX_CF_MODAL);
							break;
						case 127:
							Misa_WarningBox("Shell Error!", "", GUI_MESSAGEBOX_CF_MODAL);
							break;
						default:
							Misa_WarningBox("Unknown Error!", "", GUI_MESSAGEBOX_CF_MODAL);
							break;
						}
					}
					break;
				case SYSTEM_ID_LEFTHANDSWITCH:
					state_updated = true;
					MisaRadio_SetStatus(hSystemItems[SYSTEM_RIGHTHANDSWITCH],0);
					MisaSetHandMode(MISA_LEFTHAND_MODE);
					break;
				case SYSTEM_ID_RIGHTHANDSWITCH:
					state_updated = true;
					MisaRadio_SetStatus(hSystemItems[SYSTEM_LEFTHANDSWITCH],0);
					MisaSetHandMode(MISA_RIGHTHAND_MODE);
					break;
				case SYSTEM_ID_BALLTRAVEL:
					state_updated = true;
					MisaSetBallTravel(MisaCheckbox_GetStatus(hSystemItems[SYSTEM_BALLTRAVEL]));
					break;
				case SYSTEM_ID_ENABLESUSTAIN:
					state_updated = true;
					MisaSetEnableSustain(MisaCheckbox_GetStatus(hSystemItems[SYSTEM_ENABLESUSTAIN]));
					break;
/*				case SYSTEM_ID_SHOWSTRINGS:
					MisaSetShowStrings(MisaCheckbox_GetStatus(hSystemItems[SYSTEM_SHOWSTRINGS]));
					break;
*/				default:
					;
				}
				DEBUG_STDOUT("SYSTEM button released!");
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


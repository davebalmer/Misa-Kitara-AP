

#include <cstdlib>
#include <iostream>

#ifdef Linux
#include <dirent.h>
#endif
using namespace std;
#include <string>
#include <vector>

#ifdef WIN32
#include <Windows.h>
#endif

#include "GUI.h"
#include "WM.h"
#include "BUTTON.h"
#include "EDIT.h"

#include "MisaWidget.h"
#include "MisaDialogueBox.h"

#include "MisaAppInc.h"

#include "MainMenu.h"
#include "MisaQuickSetScreen.h"

#include "MisamenuConfig.h"

#include "Resource.h"

///////////////////////////////////////////////////////////////////////////////
//

#define MISAQUICKSET_XOFFSET 2
#define MISAQUICKSET_YOFFSET 2

#define MISAQUICKSET_PRESETSNUM 6

///////////////////////////////////////////////////////////////////////////////
//

extern std::string working_directory;

typedef enum __MISAQUICKSET
{
	MISAQUICKSET_TAPMODE,
	MISAQUICKSET_PRESET1,
	MISAQUICKSET_PRESET2,
	MISAQUICKSET_PRESET3,
	MISAQUICKSET_PRESET4,
	MISAQUICKSET_PRESET5,
	MISAQUICKSET_PRESET6,
	//MISAQUICKSET_STRINGS,
	MISAQUICKSET_CONFIG,
	MISAQUICKSET_MASTERVOLUME,
	MISAQUICKSET_MAX
} MISAQUICKSET;

typedef enum __MISAQUICKSET_ID
{
	MISAQUICKSET_ID_TAPMODE=GUI_ID_QUICKSET_BASE,
	MISAQUICKSET_ID_PRESET1,
	MISAQUICKSET_ID_PRESET2,
	MISAQUICKSET_ID_PRESET3,
	MISAQUICKSET_ID_PRESET4,
	MISAQUICKSET_ID_PRESET5,
	MISAQUICKSET_ID_PRESET6,
	//MISAQUICKSET_ID_STRINGS,
	MISAQUICKSET_ID_CONFIG,
	MISAQUICKSET_ID_MASTERVOLUME,
	MISAQUICKSET_ID_MAX
} MISAQUICKSET_ID;

///////////////////////////////////////////////////////////////////////////////
//

static WM_HWIN hMisaquickset;
static void MisaquicksetProc(WM_MESSAGE* pMsg);
static WM_HWIN hMisaquicksetItems[MISAQUICKSET_MAX];
//static COMMCOMPONENT hCOMPcomm;
U8 UpdateQuicksetInfo();

void TerminateApplication();

///////////////////////////////////////////////////////////////////////////////
//

static U8 MisaquicksetCreatePresetButtons()
{
	int i,size,x,y;
#ifdef Linux
	DIR *dp;
	struct dirent *ep;
	vector<string> preset_filenames;

	preset_filenames.clear();

	dp = opendir((working_directory + "/presets/").c_str());
	if(dp != NULL)
	{
		while((ep = readdir(dp)) != NULL)
		{
			std::string s = ep->d_name;
			if(s.size() > 3)
				if(s.substr(s.size() - 3, 3) == ".mz")
				{
					s.erase(s.size()-3,s.size());
					preset_filenames.push_back(s);
				}
		}
	}

	sort(preset_filenames.begin(), preset_filenames.end());

	closedir(dp);
#else
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	DWORD error;
	vector<string> preset_filenames;

	hFind = FindFirstFile((working_directory + "/presets\\*.mz").c_str(), &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		return 0;
	}
	else
	{
		string s;
		s = FindFileData.cFileName;
		if(s.substr(s.size() - 3, 3) == ".mz")
		{
			s.erase(s.size() - 3,s.size());
			preset_filenames.push_back(s);
		}
		while(FindNextFile(hFind,&FindFileData))
		{
			s = FindFileData.cFileName;
			if(s.substr(s.size() - 3, 3) == ".mz")
			{
				s.erase(s.size() - 3,s.size());
				preset_filenames.push_back(s);
			}
		}
		error = GetLastError();
	}
	FindClose(hFind);
#endif
	size = preset_filenames.size();
	if(size>MISAQUICKSET_PRESETSNUM)
	{
		size = MISAQUICKSET_PRESETSNUM;
	}
	x = 200;
	y = 150;
	for(i=0;i<size;i++)
	{
		hMisaquicksetItems[MISAQUICKSET_PRESET1+i] = MisaRadio_CreateEx(x, y, bmSELECT.XSize, bmSELECT.YSize,
			hMisaquickset, MISAQUICKSET_ID_PRESET1+i, WM_CF_SHOW|WM_CF_MEMDEV, preset_filenames[i].c_str(),0, &bmSELECT);
		y += bmSELECT.YSize;
	}
	preset_filenames.clear();
	return 0;
}

static U8 MisaquicksetDeletePresetButtons()
{
	if(hMisaquicksetItems[MISAQUICKSET_PRESET1])
	{
		MisaRadio_Delete(hMisaquicksetItems[MISAQUICKSET_PRESET1]);
		hMisaquicksetItems[MISAQUICKSET_PRESET1] = 0;
	}
	if(hMisaquicksetItems[MISAQUICKSET_PRESET2])
	{
		MisaRadio_Delete(hMisaquicksetItems[MISAQUICKSET_PRESET2]);
		hMisaquicksetItems[MISAQUICKSET_PRESET2] = 0;
	}
	if(hMisaquicksetItems[MISAQUICKSET_PRESET3])
	{
		MisaRadio_Delete(hMisaquicksetItems[MISAQUICKSET_PRESET3]);
		hMisaquicksetItems[MISAQUICKSET_PRESET3] = 0;
	}
	if(hMisaquicksetItems[MISAQUICKSET_PRESET4])
	{
		MisaRadio_Delete(hMisaquicksetItems[MISAQUICKSET_PRESET4]);
		hMisaquicksetItems[MISAQUICKSET_PRESET4] = 0;
	}
	return 0;
}

static U8 MisaquicksetLoadPreset(int pos)
{
	char name[32];
	string s;
	if(hMisaquicksetItems[MISAQUICKSET_PRESET1+pos])
	{
		SynthTurnNotesOff();
		BUTTON_GetText(hMisaquicksetItems[MISAQUICKSET_PRESET1+pos], name, 32);
		s = name;
		SynthLoadPreset(s+".mz");
		UpdateSynthSettingEx(MISAMODULE_ALL);
		BUTTON_SetBitmap(hMisaquicksetItems[MISAQUICKSET_PRESET1+pos], BUTTON_CI_PRESSED, &bmLOAD);
		Misa_Delay(200);
	}
	TerminateApplication();
	MisaquicksetDeletePresetButtons();
	return 0;
}

static U8 MisaquicksetCreateItems(WM_HWIN hParent)
{
	int x,y;
	//hCOMPcomm = CreateCommComponentEx(hParent,COMMON_ITEM_CLOSE);//|COMMON_ITEM_FX1|COMMON_ITEM_FX2
	memset(hMisaquicksetItems,0,sizeof(hMisaquicksetItems));
	x = 0;
	y = 0;
	hMisaquicksetItems[MISAQUICKSET_TAPMODE] = MisaCheckbox_Create(x,y,bmQS_TAPMODE_UN.XSize,bmQS_TAPMODE_UN.YSize,hParent,MISAQUICKSET_ID_TAPMODE,WM_CF_SHOW|WM_CF_MEMDEV, &bmQS_TAPMODE_UN, &bmQS_TAPMODE_SE);
	y =500;
	hMisaquicksetItems[MISAQUICKSET_CONFIG] = BUTTON_CreateAsChild(x,y,bmQS_CONF_UN.XSize,bmQS_CONF_UN.YSize,hParent,MISAQUICKSET_ID_CONFIG,WM_CF_SHOW|WM_CF_MEMDEV);
	BUTTON_SetFocussable(hMisaquicksetItems[MISAQUICKSET_CONFIG],0);
	BUTTON_SetBitmap(hMisaquicksetItems[MISAQUICKSET_CONFIG],BUTTON_CI_UNPRESSED,&bmQS_CONF_UN);
	BUTTON_SetBitmap(hMisaquicksetItems[MISAQUICKSET_CONFIG],BUTTON_CI_PRESSED,&bmQS_CONF_UN);
	x = 0;//bmQS_CONF_UN.XSize+100;
	y = 100+16;
	hMisaquicksetItems[MISAQUICKSET_MASTERVOLUME] = MisaProgressbar_CreateEx(x,y,0,0,hParent,MISAQUICKSET_ID_MASTERVOLUME,"Master","volume",MISAPROG_FONT24,0);
	return 0;
}

static U8 MisaquicksetDeleteItems()
{
	//DeleteCommComponent(hCOMPcomm);
	if(hMisaquicksetItems[MISAQUICKSET_TAPMODE])
	{
		MisaCheckbox_Delete(hMisaquicksetItems[MISAQUICKSET_TAPMODE]);
	}
	if(hMisaquicksetItems[MISAQUICKSET_PRESET1])
	{
		BUTTON_Delete(hMisaquicksetItems[MISAQUICKSET_PRESET1]);
	}
	if(hMisaquicksetItems[MISAQUICKSET_PRESET2])
	{
		BUTTON_Delete(hMisaquicksetItems[MISAQUICKSET_PRESET2]);
	}
	if(hMisaquicksetItems[MISAQUICKSET_PRESET3])
	{
		BUTTON_Delete(hMisaquicksetItems[MISAQUICKSET_PRESET3]);
	}
#if 0
	if(hMisaquicksetItems[MISAQUICKSET_STRINGS])
	{
		MisaCheckbox_Delete(hMisaquicksetItems[MISAQUICKSET_STRINGS]);
	}
#endif
	if(hMisaquicksetItems[MISAQUICKSET_CONFIG])
	{
		BUTTON_Delete(hMisaquicksetItems[MISAQUICKSET_CONFIG]);
	}
	if(hMisaquicksetItems[MISAQUICKSET_MASTERVOLUME])
	{
		MisaProgressbar_Delete(hMisaquicksetItems[MISAQUICKSET_MASTERVOLUME]);
	}
	return 0;
}

U8 CreateMisaquicksetScreen(void* pPara)
{
	int x,y;
	x=GUI_GetScreenSizeX();
	y=GUI_GetScreenSizeY();
	hMisaquickset = WM_CreateWindow(0, 0, x, y, WM_CF_MEMDEV, MisaquicksetProc, 0);
	return 0;
}

U8 DeleteMisaquicksetScreen()
{
	MisaquicksetDeleteItems();
	WM_DeleteWindow(hMisaquickset);
	return 0;
}

U8 TopMisaquicksetScreen(WM_HWIN hPreWin)
{
	if(hPreWin)
	{
		WM_HideWindow(hPreWin);
	}
	UpdateQuicksetInfo();
	WM_ShowWindow(hMisaquickset);
	//WM_BringToTop(hSystem);
	return 0;
}

U8 BeginMisaquicksetScreen()
{
	UpdateQuicksetInfo();
	WM_ShowWindow(hMisaquickset);
	return 0;
}

U8 EndMisaquicksetScreen()
{
	WM_HideWindow(hMisaquickset);
	return 0;
}

U8 UpdateQuicksetInfo()
{
	MisaCheckbox_SetStatus(hMisaquicksetItems[MISAQUICKSET_TAPMODE],SynthGetTapmode());
#if 0
	MisaCheckbox_SetStatus(hMisaquicksetItems[MISAQUICKSET_STRINGS],SynthGetStringmode());
#endif
	MisaProgressbar_SetPercentEx(hMisaquicksetItems[MISAQUICKSET_MASTERVOLUME],SynthGetMastervolume(),0);
	MisaquicksetCreatePresetButtons();
	return 0;
}

U8 GetQuicksetInfo()
{
	return 0;
}

static void MisaquicksetProc(WM_MESSAGE* pMsg)
{
	static int x;
	int NCode,Id;
	GUI_PID_STATE* pPID_State;
	switch (pMsg->MsgId)
	{
	case WM_CREATE:
		MisaquicksetCreateItems(pMsg->hWin);
		break;
	case WM_DELETE:
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
			case MISAQUICKSET_ID_TAPMODE:
				SynthSetTapmode(MisaCheckbox_GetStatus(pMsg->hWinSrc));
				RunFramework();
				TerminateApplication();
				MisaquicksetDeletePresetButtons();
				break;
			case MISAQUICKSET_ID_PRESET1:
				if(1 == x)
				{
					MisaquicksetLoadPreset(0);
				}
				else
				{
					BUTTON_SetBitmap(pMsg->hWinSrc, BUTTON_CI_PRESSED, &bmSELECT);
					if(hMisaquicksetItems[MISAQUICKSET_PRESET2])
					{
						MisaRadio_SetStatus(hMisaquicksetItems[MISAQUICKSET_PRESET2], 0);
					}
					if(hMisaquicksetItems[MISAQUICKSET_PRESET3])
					{
						MisaRadio_SetStatus(hMisaquicksetItems[MISAQUICKSET_PRESET3], 0);
					}
					if(hMisaquicksetItems[MISAQUICKSET_PRESET4])
					{
						MisaRadio_SetStatus(hMisaquicksetItems[MISAQUICKSET_PRESET4], 0);
					}
					if(hMisaquicksetItems[MISAQUICKSET_PRESET5])
					{
						MisaRadio_SetStatus(hMisaquicksetItems[MISAQUICKSET_PRESET5], 0);
					}
					if(hMisaquicksetItems[MISAQUICKSET_PRESET6])
					{
						MisaRadio_SetStatus(hMisaquicksetItems[MISAQUICKSET_PRESET6], 0);
					}
					x = 1;
				}
				break;
			case MISAQUICKSET_ID_PRESET2:
				if(2 == x)
				{
					MisaquicksetLoadPreset(1);
				}
				else
				{
					BUTTON_SetBitmap(pMsg->hWinSrc, BUTTON_CI_PRESSED, &bmSELECT);
					if(hMisaquicksetItems[MISAQUICKSET_PRESET1])
					{
						MisaRadio_SetStatus(hMisaquicksetItems[MISAQUICKSET_PRESET1], 0);
					}
					if(hMisaquicksetItems[MISAQUICKSET_PRESET3])
					{
						MisaRadio_SetStatus(hMisaquicksetItems[MISAQUICKSET_PRESET3], 0);
					}
					if(hMisaquicksetItems[MISAQUICKSET_PRESET4])
					{
						MisaRadio_SetStatus(hMisaquicksetItems[MISAQUICKSET_PRESET4], 0);
					}
					if(hMisaquicksetItems[MISAQUICKSET_PRESET5])
					{
						MisaRadio_SetStatus(hMisaquicksetItems[MISAQUICKSET_PRESET5], 0);
					}
					if(hMisaquicksetItems[MISAQUICKSET_PRESET6])
					{
						MisaRadio_SetStatus(hMisaquicksetItems[MISAQUICKSET_PRESET6], 0);
					}
					x = 2;
				}
				break;
			case MISAQUICKSET_ID_PRESET3:
				if(3 == x)
				{
					MisaquicksetLoadPreset(2);
				}
				else
				{
					BUTTON_SetBitmap(pMsg->hWinSrc, BUTTON_CI_PRESSED, &bmSELECT);
					if(hMisaquicksetItems[MISAQUICKSET_PRESET1])
					{
						MisaRadio_SetStatus(hMisaquicksetItems[MISAQUICKSET_PRESET1], 0);
					}
					if(hMisaquicksetItems[MISAQUICKSET_PRESET2])
					{
						MisaRadio_SetStatus(hMisaquicksetItems[MISAQUICKSET_PRESET2], 0);
					}
					if(hMisaquicksetItems[MISAQUICKSET_PRESET4])
					{
						MisaRadio_SetStatus(hMisaquicksetItems[MISAQUICKSET_PRESET4], 0);
					}
					if(hMisaquicksetItems[MISAQUICKSET_PRESET5])
					{
						MisaRadio_SetStatus(hMisaquicksetItems[MISAQUICKSET_PRESET5], 0);
					}
					if(hMisaquicksetItems[MISAQUICKSET_PRESET6])
					{
						MisaRadio_SetStatus(hMisaquicksetItems[MISAQUICKSET_PRESET6], 0);
					}
					x = 3;
				}
				break;
			case MISAQUICKSET_ID_PRESET4:
				if(4 == x)
				{
					MisaquicksetLoadPreset(3);
				}
				else
				{
					BUTTON_SetBitmap(pMsg->hWinSrc, BUTTON_CI_PRESSED, &bmSELECT);
					if(hMisaquicksetItems[MISAQUICKSET_PRESET1])
					{
						MisaRadio_SetStatus(hMisaquicksetItems[MISAQUICKSET_PRESET1], 0);
					}
					if(hMisaquicksetItems[MISAQUICKSET_PRESET2])
					{
						MisaRadio_SetStatus(hMisaquicksetItems[MISAQUICKSET_PRESET2], 0);
					}
					if(hMisaquicksetItems[MISAQUICKSET_PRESET3])
					{
						MisaRadio_SetStatus(hMisaquicksetItems[MISAQUICKSET_PRESET3], 0);
					}
					if(hMisaquicksetItems[MISAQUICKSET_PRESET5])
					{
						MisaRadio_SetStatus(hMisaquicksetItems[MISAQUICKSET_PRESET5], 0);
					}
					if(hMisaquicksetItems[MISAQUICKSET_PRESET6])
					{
						MisaRadio_SetStatus(hMisaquicksetItems[MISAQUICKSET_PRESET6], 0);
					}
					x = 4;
				}
				break;
			case MISAQUICKSET_ID_PRESET5:
				if(5 == x)
				{
					MisaquicksetLoadPreset(4);
				}
				else
				{
					BUTTON_SetBitmap(pMsg->hWinSrc, BUTTON_CI_PRESSED, &bmSELECT);
					if(hMisaquicksetItems[MISAQUICKSET_PRESET1])
					{
						MisaRadio_SetStatus(hMisaquicksetItems[MISAQUICKSET_PRESET1], 0);
					}
					if(hMisaquicksetItems[MISAQUICKSET_PRESET2])
					{
						MisaRadio_SetStatus(hMisaquicksetItems[MISAQUICKSET_PRESET2], 0);
					}
					if(hMisaquicksetItems[MISAQUICKSET_PRESET3])
					{
						MisaRadio_SetStatus(hMisaquicksetItems[MISAQUICKSET_PRESET3], 0);
					}
					if(hMisaquicksetItems[MISAQUICKSET_PRESET4])
					{
						MisaRadio_SetStatus(hMisaquicksetItems[MISAQUICKSET_PRESET4], 0);
					}
					if(hMisaquicksetItems[MISAQUICKSET_PRESET6])
					{
						MisaRadio_SetStatus(hMisaquicksetItems[MISAQUICKSET_PRESET6], 0);
					}
					x = 5;
				}
				break;
			case MISAQUICKSET_ID_PRESET6:
				if(6 == x)
				{
					MisaquicksetLoadPreset(5);
				}
				else
				{
					BUTTON_SetBitmap(pMsg->hWinSrc, BUTTON_CI_PRESSED, &bmSELECT);
					if(hMisaquicksetItems[MISAQUICKSET_PRESET1])
					{
						MisaRadio_SetStatus(hMisaquicksetItems[MISAQUICKSET_PRESET1], 0);
					}
					if(hMisaquicksetItems[MISAQUICKSET_PRESET2])
					{
						MisaRadio_SetStatus(hMisaquicksetItems[MISAQUICKSET_PRESET2], 0);
					}
					if(hMisaquicksetItems[MISAQUICKSET_PRESET3])
					{
						MisaRadio_SetStatus(hMisaquicksetItems[MISAQUICKSET_PRESET3], 0);
					}
					if(hMisaquicksetItems[MISAQUICKSET_PRESET4])
					{
						MisaRadio_SetStatus(hMisaquicksetItems[MISAQUICKSET_PRESET4], 0);
					}
					if(hMisaquicksetItems[MISAQUICKSET_PRESET5])
					{
						MisaRadio_SetStatus(hMisaquicksetItems[MISAQUICKSET_PRESET5], 0);
					}
					x = 6;
				}
				break;
#if 0
			case MISAQUICKSET_ID_STRINGS:
				SynthSetStringmode(MisaCheckbox_GetStatus(pMsg->hWinSrc));
				RunFramework();
				TerminateApplication();
				break;
#endif
			case COMMON_ID_CLOSE:
				RunFramework();
				TerminateApplication();
				MisaquicksetDeletePresetButtons();
				break;
			case MISAQUICKSET_ID_CONFIG:
				TopMainMenu(pMsg->hWin);
				MisaquicksetDeletePresetButtons();
				break;
			default:
				x = 0;
			}
			break;
		case WM_NOTIFICATION_VALUE_CHANGED:
			x = MisaProgressbar_GetPercent(pMsg->hWinSrc);
			SynthSetMastervolume(x<127?x:127);
			break;
		}
		break;
	case WM_TOUCH:
		if(pMsg->Data.p)
		{
			pPID_State = (GUI_PID_STATE*)pMsg->Data.p;
			if(pPID_State->Pressed)
			{
			}
			else
			{
				TerminateApplication();
			}
		}
		break;
	default:
		WM_DefaultProc(pMsg);
	}
}


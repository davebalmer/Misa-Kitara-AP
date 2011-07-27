
#include <cstdlib>
#include <iostream>

#include "GUI.h"
#include "WM.h"
#include "BUTTON.h"

#include "MisaWidget.h"
#include "MisaDialogueBox.h"

#include "MisaAppInc.h"

#include "MainMenu.h"
#include "ControlAssignments.h"
#include "MIDIControlScreen.h"

#include "MisamenuConfig.h"

#include "Resource.h"

///////////////////////////////////////////////////////////////////////////////
//

#define MIDICONTROL_TYPE_XPOS 80
#define MIDICONTROL_TYPE_YPOS 120

#define MIDICONTROL_XOFFSET 2
#define MIDICONTROL_YOFFSET 2

///////////////////////////////////////////////////////////////////////////////
//

typedef enum __MIDICONTROLITEMS
{
	MIDICONTROL_CONFIRM,
	MIDICONTROL_RETURN,
	MIDICONTROL_LEFTARROW,
	MIDICONTROL_RIGHTARROW,
	MIDICONTROL_PITCH,
	MIDICONTROL_VELOCITY,
	MIDICONTROL_CC,
	MIDICONTROL_PCCHANNEL,
	MIDICONTROL_PCSTRING,
	MIDICONTROL_PCNUMBER,
	MIDICONTROL_INVERT,
	MIDICONTROL_VARIMIN,
	MIDICONTROL_VARIMAX,
	MIDICONTROL_DRAGCENTRE,
	MIDICONTROL_MAX
} MIDICONTROLITEMS;

typedef enum __MIDICONTROL_ID
{
	MIDICONTROL_ID_CONFIRM=GUI_ID_MIDICONTROL_BASE,
	MIDICONTROL_ID_RETURN,
	MIDICONTROL_ID_LEFTARROW,
	MIDICONTROL_ID_RIGHTARROW,
	MIDICONTROL_ID_PITCH,
	MIDICONTROL_ID_VELOCITY,
	MIDICONTROL_ID_CC,
	MIDICONTROL_ID_PCCHANNEL,
	MIDICONTROL_ID_PCSTRING,
	MIDICONTROL_ID_PCNUMBER,
	MIDICONTROL_ID_INVERT,
	MIDICONTROL_ID_VARIMIN,
	MIDICONTROL_ID_VARIMAX,
	MIDICONTROL_ID_DRAGCENTRE,
	MIDICONTROL_ID_MAX
} MIDICONTROL_ID;

///////////////////////////////////////////////////////////////////////////////
// Control combo

typedef struct __MIDICTRL_CTCOMBO
{
	GUI_RECT left;
	GUI_RECT right;
	GUI_RECT centre;
} MIDICTRL_CTCOMBO,*LPMIDICTRL_CTCOMBO;

///////////////////////////////////////////////////////////////////////////////
//

static WM_HWIN hMIDIControl;
static void MIDIControlProc(WM_MESSAGE* pMsg);
static WM_HWIN hMIDIControlItems[MIDICONTROL_MAX];

static int MIDIControlGetFX();

static int control;
static int type;
static const char* strControltype[]=
{
	"touch_x",
	"touch_y",
	"drag_x",
	"drag_y",
	"ball_x",
	"ball_y",
	"variation",
	"stop_sound"
};

static MIDICTRL_CTCOMBO ctrlCombo;

///////////////////////////////////////////////////////////////////////////////
//

static void MIDIControlVaMin(int value,char* buf)
{
	sprintf(buf,"min:%d",value);
}

static void MIDIControlVaMax(int value,char* buf)
{
	sprintf(buf,"max:%d",value);
}

static void MIDIControlDragCentre(int value,char* buf)
{
	sprintf(buf,"Centre:%d",value);
}

static U8 MIDIControlCreateItems(WM_HWIN hParent)
{
	int x,y;
	memset(hMIDIControlItems,0,sizeof(hMIDIControlItems));
	x=5;
	y=96;
	ctrlCombo.left.x0 = x;
	ctrlCombo.left.y0 = y;
	hMIDIControlItems[MIDICONTROL_LEFTARROW] = BUTTON_CreateAsChild(x,y,bmMC_CTLA_UN.XSize,bmMC_CTLA_UN.YSize,hParent,MIDICONTROL_ID_LEFTARROW,WM_CF_SHOW|WM_CF_MEMDEV);
	BUTTON_SetFocussable(hMIDIControlItems[MIDICONTROL_LEFTARROW],0);
	BUTTON_SetBitmap(hMIDIControlItems[MIDICONTROL_LEFTARROW],BUTTON_CI_UNPRESSED,&bmMC_CTLA_UN);
	BUTTON_SetBitmap(hMIDIControlItems[MIDICONTROL_LEFTARROW],BUTTON_CI_PRESSED,&bmMC_CTLA_SE);
	x += bmMC_CTLA_UN.XSize;
	ctrlCombo.left.x1 = x;
	ctrlCombo.left.y1 = y+bmMC_CTLA_UN.YSize;
	ctrlCombo.centre.x0 = x;
	ctrlCombo.centre.y0 = y;
	x += bmMC_CTRL_CENTRE.XSize;
	ctrlCombo.centre.x1 = x;
	ctrlCombo.centre.y1 = y+bmMC_CTLA_UN.YSize;
	ctrlCombo.right.x0 = x;
	ctrlCombo.right.y0 = y;
	hMIDIControlItems[MIDICONTROL_RIGHTARROW] = BUTTON_CreateAsChild(x,y,bmMC_CTRA_UN.XSize,bmMC_CTRA_UN.YSize,hParent,MIDICONTROL_ID_RIGHTARROW,WM_CF_SHOW|WM_CF_MEMDEV);
	BUTTON_SetFocussable(hMIDIControlItems[MIDICONTROL_RIGHTARROW],0);
	BUTTON_SetBitmap(hMIDIControlItems[MIDICONTROL_RIGHTARROW],BUTTON_CI_UNPRESSED,&bmMC_CTRA_UN);
	BUTTON_SetBitmap(hMIDIControlItems[MIDICONTROL_RIGHTARROW],BUTTON_CI_PRESSED,&bmMC_CTRA_SE);
	x += bmMC_CTRA_UN.XSize;
	y += bmMC_CTRA_UN.YSize;
	ctrlCombo.right.x1 = x;
	ctrlCombo.right.y1 = y;

	x=5;
	y=179;
	hMIDIControlItems[MIDICONTROL_PITCH] = MisaRadio_Create(x,y,bmMC_PITCH_UN.XSize,bmMC_PITCH_UN.YSize,hParent,MIDICONTROL_ID_PITCH,WM_CF_SHOW|WM_CF_MEMDEV,&bmMC_PITCH_UN,&bmMC_PITCH_SE);
	x += bmMC_PITCH_UN.XSize;
	hMIDIControlItems[MIDICONTROL_VELOCITY] = MisaRadio_Create(x,y,bmMC_VELOCITY_UN.XSize,bmMC_VELOCITY_UN.YSize,hParent,MIDICONTROL_ID_VELOCITY,WM_CF_SHOW|WM_CF_MEMDEV,&bmMC_VELOCITY_UN,&bmMC_VELOCITY_SE);
	x += bmMC_VELOCITY_UN.XSize;
	hMIDIControlItems[MIDICONTROL_CC] = MisaRadio_Create(x,y,bmMC_CC_UN.XSize,bmMC_CC_UN.YSize,hParent,MIDICONTROL_ID_CC,WM_CF_SHOW|WM_CF_MEMDEV,&bmMC_CC_UN,&bmMC_CC_SE);
	MisaRadio_SetStatus(hMIDIControlItems[MIDICONTROL_PITCH], 1);

	x = MIDICONTROL_XOFFSET;
	y = 502;
	hMIDIControlItems[MIDICONTROL_CONFIRM] = BUTTON_CreateAsChild(x,y,bmMC_OK_UN.XSize,bmMC_OK_UN.YSize,hParent,MIDICONTROL_ID_CONFIRM,WM_CF_SHOW|WM_CF_MEMDEV);
	BUTTON_SetFocussable(hMIDIControlItems[MIDICONTROL_CONFIRM],0);
	BUTTON_SetBitmap(hMIDIControlItems[MIDICONTROL_CONFIRM],BUTTON_CI_UNPRESSED,&bmMC_OK_UN);
	BUTTON_SetBitmap(hMIDIControlItems[MIDICONTROL_CONFIRM],BUTTON_CI_PRESSED,&bmMC_OK_SE);
	x+=bmMC_OK_UN.XSize;
	x+=MIDICONTROL_XOFFSET;
	x+=MIDICONTROL_XOFFSET;
	hMIDIControlItems[MIDICONTROL_RETURN] = BUTTON_CreateAsChild(x,y,bmCLOSE_NORMAL.XSize,bmCLOSE_NORMAL.YSize,hParent,MIDICONTROL_ID_RETURN,WM_CF_SHOW | WM_CF_MEMDEV);
	BUTTON_SetFocussable(hMIDIControlItems[MIDICONTROL_RETURN],0);
	BUTTON_SetBitmap(hMIDIControlItems[MIDICONTROL_RETURN],BUTTON_CI_UNPRESSED,&bmCLOSE_NORMAL);
	BUTTON_SetBitmap(hMIDIControlItems[MIDICONTROL_RETURN],BUTTON_CI_PRESSED,&bmCLOSE_PRESSED);

	x=5;
	y=266;
	hMIDIControlItems[MIDICONTROL_PCCHANNEL] = MisaProgressCombo_CreateEx(x,y,0,0,hParent,WM_CF_SHOW|WM_CF_MEMDEV,MIDICONTROL_ID_PCCHANNEL,"CHANNEL",0);
	MisaProgressCombo_SetRange(hMIDIControlItems[MIDICONTROL_PCCHANNEL],15,1);
	hMIDIControlItems[MIDICONTROL_PCSTRING] = MisaProgressCombo_CreateEx(x,y,0,0,hParent,WM_CF_MEMDEV,MIDICONTROL_ID_PCSTRING,"STRING",0);
	MisaProgressCombo_SetRange(hMIDIControlItems[MIDICONTROL_PCSTRING],5,1);
	y=373;
	hMIDIControlItems[MIDICONTROL_PCNUMBER] = MisaProgressCombo_CreateEx(x,y,0,0,hParent,WM_CF_MEMDEV,MIDICONTROL_ID_PCNUMBER,"CONTROL NUM",0);

	x = 524;
	y = 95;
	hMIDIControlItems[MIDICONTROL_INVERT] = MisaCheckbox_Create(x,y,bmMC_INVERT_UN.XSize,bmMC_INVERT_UN.YSize,hParent,MIDICONTROL_ID_INVERT,WM_CF_SHOW|WM_CF_MEMDEV,&bmMC_INVERT_UN,&bmMC_INVERT_SE);
	x = 625;
	y = 140;
	hMIDIControlItems[MIDICONTROL_VARIMIN] = MisaVerSlider_CreateEx(x,y,0,0,hParent,MIDICONTROL_ID_VARIMIN,MIDIControlVaMin);
	x += 80;
	hMIDIControlItems[MIDICONTROL_VARIMAX] = MisaVerSlider_CreateEx(x,y,0,0,hParent,MIDICONTROL_ID_VARIMAX,MIDIControlVaMax);
	x -= 40;
	hMIDIControlItems[MIDICONTROL_DRAGCENTRE] = MisaVerSlider_CreateEx(x,y,0,0,hParent,MIDICONTROL_ID_DRAGCENTRE,MIDIControlDragCentre);
	return 0;
}

static U8 MIDIControlDeleteItems()
{
	if(hMIDIControlItems[MIDICONTROL_CONFIRM])
	{
		BUTTON_Delete(hMIDIControlItems[MIDICONTROL_CONFIRM]);
		hMIDIControlItems[MIDICONTROL_CONFIRM] = 0;
	}
	if(hMIDIControlItems[MIDICONTROL_RETURN])
	{
		BUTTON_Delete(hMIDIControlItems[MIDICONTROL_RETURN]);
		hMIDIControlItems[MIDICONTROL_RETURN] = 0;
	}
	if(hMIDIControlItems[MIDICONTROL_LEFTARROW])
	{
		BUTTON_Delete(hMIDIControlItems[MIDICONTROL_LEFTARROW]);
		hMIDIControlItems[MIDICONTROL_LEFTARROW] = 0;
	}
	if(hMIDIControlItems[MIDICONTROL_RIGHTARROW])
	{
		BUTTON_Delete(hMIDIControlItems[MIDICONTROL_RIGHTARROW]);
		hMIDIControlItems[MIDICONTROL_RIGHTARROW] = 0;
	}
	if(hMIDIControlItems[MIDICONTROL_PITCH])
	{
		MisaRadio_Delete(hMIDIControlItems[MIDICONTROL_PITCH]);
		hMIDIControlItems[MIDICONTROL_PITCH] = 0;
	}
	if(hMIDIControlItems[MIDICONTROL_VELOCITY])
	{
		MisaRadio_Delete(hMIDIControlItems[MIDICONTROL_VELOCITY]);
		hMIDIControlItems[MIDICONTROL_VELOCITY] = 0;
	}
	if(hMIDIControlItems[MIDICONTROL_CC])
	{
		MisaRadio_Delete(hMIDIControlItems[MIDICONTROL_CC]);
		hMIDIControlItems[MIDICONTROL_CC] = 0;
	}
	if(hMIDIControlItems[hMIDIControlItems[MIDICONTROL_PCCHANNEL]])
	{
		MisaProgressCombo_Delete(hMIDIControlItems[hMIDIControlItems[MIDICONTROL_PCCHANNEL]]);
		hMIDIControlItems[hMIDIControlItems[MIDICONTROL_PCCHANNEL]] = 0;
	}
	if(hMIDIControlItems[hMIDIControlItems[MIDICONTROL_PCSTRING]])
	{
		MisaProgressCombo_Delete(hMIDIControlItems[hMIDIControlItems[MIDICONTROL_PCSTRING]]);
		hMIDIControlItems[hMIDIControlItems[MIDICONTROL_PCSTRING]] = 0;
	}
	if(hMIDIControlItems[hMIDIControlItems[MIDICONTROL_PCNUMBER]])
	{
		MisaProgressCombo_Delete(hMIDIControlItems[hMIDIControlItems[MIDICONTROL_PCNUMBER]]);
		hMIDIControlItems[hMIDIControlItems[MIDICONTROL_PCNUMBER]] = 0;
	}
	if(hMIDIControlItems[MIDICONTROL_INVERT])
	{
		MisaCheckbox_Delete(hMIDIControlItems[MIDICONTROL_INVERT]);
		hMIDIControlItems[MIDICONTROL_INVERT] = 0;
	}
	if(hMIDIControlItems[MIDICONTROL_VARIMIN])
	{
		MisaVerSlider_Delete(hMIDIControlItems[MIDICONTROL_VARIMIN]);
		hMIDIControlItems[MIDICONTROL_VARIMIN] = 0;
	}
	if(hMIDIControlItems[MIDICONTROL_VARIMAX])
	{
		MisaVerSlider_Delete(hMIDIControlItems[MIDICONTROL_VARIMAX]);
		hMIDIControlItems[MIDICONTROL_VARIMAX] = 0;
	}
	if(hMIDIControlItems[MIDICONTROL_DRAGCENTRE])
	{
		MisaVerSlider_Delete(hMIDIControlItems[MIDICONTROL_DRAGCENTRE]);
		hMIDIControlItems[MIDICONTROL_DRAGCENTRE] = 0;
	}
	return 0;
}

U8 CreateMIDIControlScreen(void* pPara)
{
	int x,y;
	x=GUI_GetScreenSizeX();
	y=GUI_GetScreenSizeY();
	control = 0;
	type = 0;
	hMIDIControl = WM_CreateWindow(0, 0, x, y, /*WM_CF_SHOW | */WM_CF_MEMDEV, MIDIControlProc, 0);
	return 0;
}

U8 DeleteMIDIControlScreen()
{
	WM_DeleteWindow(hMIDIControl);
	return 0;
}

U8 TopMIDIControlScreen(WM_HWIN hPreWin)
{
	WM_HideWindow(hPreWin);
	WM_ShowWindow(hMIDIControl);
	//WM_BringToTop(hMIDIControl);
	SetWindowToUpdateIfPresetModified(hMIDIControl);
	return 0;
}

U8 UpdateMIDIControlInfo(PMIDICTRL_INFO pInfo)
{
	control = pInfo->control;
	type = pInfo->type;
	if(control > sizeof(strControltype)/4-1)
	{
		control = sizeof(strControltype)/4-1;
	}
	WM_HideWindow(hMIDIControlItems[MIDICONTROL_VARIMIN]);
	WM_HideWindow(hMIDIControlItems[MIDICONTROL_VARIMAX]);
	WM_HideWindow(hMIDIControlItems[MIDICONTROL_DRAGCENTRE]);
	if(7 == control)
	{
		WM_HideWindow(hMIDIControlItems[MIDICONTROL_PITCH]);
		WM_HideWindow(hMIDIControlItems[MIDICONTROL_VELOCITY]);
		WM_HideWindow(hMIDIControlItems[MIDICONTROL_CC]);
		WM_HideWindow(hMIDIControlItems[MIDICONTROL_INVERT]);
		
		WM_ShowWindow(hMIDIControlItems[MIDICONTROL_PCCHANNEL]);
		WM_ShowWindow(hMIDIControlItems[MIDICONTROL_PCSTRING]);
		WM_ShowWindow(hMIDIControlItems[MIDICONTROL_PCNUMBER]);
		WM_MoveTo(hMIDIControlItems[MIDICONTROL_PCCHANNEL],5,373);
		MisaProgressCombo_SetPercent(hMIDIControlItems[MIDICONTROL_PCCHANNEL],0);
		MisaProgressCombo_SetRange(hMIDIControlItems[MIDICONTROL_PCCHANNEL], 127, 0);
		MisaProgressCombo_SetText(hMIDIControlItems[MIDICONTROL_PCCHANNEL], "VALUE");
		WM_MoveTo(hMIDIControlItems[MIDICONTROL_PCSTRING],5,160);
		WM_MoveTo(hMIDIControlItems[MIDICONTROL_PCNUMBER],5,266);
		type = 3;
		MisaRadio_SetStatus(hMIDIControlItems[MIDICONTROL_PITCH], 0);
		MisaRadio_SetStatus(hMIDIControlItems[MIDICONTROL_VELOCITY], 0);
		MisaRadio_SetStatus(hMIDIControlItems[MIDICONTROL_CC], 1);
	}
	else
	{
		WM_ShowWindow(hMIDIControlItems[MIDICONTROL_PITCH]);
		WM_ShowWindow(hMIDIControlItems[MIDICONTROL_VELOCITY]);
		WM_ShowWindow(hMIDIControlItems[MIDICONTROL_CC]);
		WM_ShowWindow(hMIDIControlItems[MIDICONTROL_INVERT]);
		WM_MoveTo(hMIDIControlItems[MIDICONTROL_PCCHANNEL],5,266);
		MisaProgressCombo_SetPercent(hMIDIControlItems[MIDICONTROL_PCCHANNEL],0);
		MisaProgressCombo_SetRange(hMIDIControlItems[MIDICONTROL_PCCHANNEL],15,1);
		MisaProgressCombo_SetText(hMIDIControlItems[MIDICONTROL_PCCHANNEL], "CHANNEL");
		WM_ShowWindow(hMIDIControlItems[MIDICONTROL_PCCHANNEL]);
		WM_MoveTo(hMIDIControlItems[MIDICONTROL_PCSTRING],5,266);
		WM_HideWindow(hMIDIControlItems[MIDICONTROL_PCSTRING]);
		WM_MoveTo(hMIDIControlItems[MIDICONTROL_PCNUMBER],5,373);
		WM_ShowWindow(hMIDIControlItems[MIDICONTROL_PCNUMBER]);
		switch(control)
		{
		case 0:
		case 1:
		case 4:
		case 5:
			break;
		case 2:
		case 3:
			if(type)
			{
				WM_ShowWindow(hMIDIControlItems[MIDICONTROL_DRAGCENTRE]);
			}
			break;
		case 6:
			WM_ShowWindow(hMIDIControlItems[MIDICONTROL_VARIMIN]);
			WM_ShowWindow(hMIDIControlItems[MIDICONTROL_VARIMAX]);
			break;
		default:;
		}
	}
	#if 0
	if(sizeof(strControltype)/4-1 == control)
	{
		WM_ShowWindow(hMIDIControlItems[MIDICONTROL_VARIMIN]);
		WM_ShowWindow(hMIDIControlItems[MIDICONTROL_VARIMAX]);
		WM_HideWindow(hMIDIControlItems[MIDICONTROL_DRAGCENTRE]);
	}
	else if((2==control || 3==control) && 0!=type)
	{
		WM_HideWindow(hMIDIControlItems[MIDICONTROL_VARIMIN]);
		WM_HideWindow(hMIDIControlItems[MIDICONTROL_VARIMAX]);
		WM_ShowWindow(hMIDIControlItems[MIDICONTROL_DRAGCENTRE]);
	}
	else
	{
		WM_HideWindow(hMIDIControlItems[MIDICONTROL_VARIMIN]);
		WM_HideWindow(hMIDIControlItems[MIDICONTROL_VARIMAX]);
		WM_HideWindow(hMIDIControlItems[MIDICONTROL_DRAGCENTRE]);
	}
	#endif
	switch(type)
	{
	case 0:
		MisaProgressCombo_SetPercent(hMIDIControlItems[MIDICONTROL_PCCHANNEL],pInfo->channel);
		MisaProgressCombo_SetPercent(hMIDIControlItems[MIDICONTROL_PCSTRING],0);
		MisaProgressCombo_SetPercent(hMIDIControlItems[MIDICONTROL_PCNUMBER],0);
		WM_ShowWindow(hMIDIControlItems[MIDICONTROL_PCCHANNEL]);
		WM_HideWindow(hMIDIControlItems[MIDICONTROL_PCSTRING]);
		WM_HideWindow(hMIDIControlItems[MIDICONTROL_PCNUMBER]);
		break;
	case 1:
		MisaProgressCombo_SetPercent(hMIDIControlItems[MIDICONTROL_PCCHANNEL],0);
		MisaProgressCombo_SetPercent(hMIDIControlItems[MIDICONTROL_PCSTRING],pInfo->string);
		MisaProgressCombo_SetPercent(hMIDIControlItems[MIDICONTROL_PCNUMBER],0);
		WM_ShowWindow(hMIDIControlItems[MIDICONTROL_PCSTRING]);
		WM_HideWindow(hMIDIControlItems[MIDICONTROL_PCCHANNEL]);
		WM_HideWindow(hMIDIControlItems[MIDICONTROL_PCNUMBER]);
		break;
	case 2:
	default:
		MisaProgressCombo_SetPercent(hMIDIControlItems[MIDICONTROL_PCCHANNEL],pInfo->channel);
		MisaProgressCombo_SetPercent(hMIDIControlItems[MIDICONTROL_PCSTRING],0);
		MisaProgressCombo_SetPercent(hMIDIControlItems[MIDICONTROL_PCNUMBER],pInfo->cc);
		WM_ShowWindow(hMIDIControlItems[MIDICONTROL_PCCHANNEL]);
		WM_ShowWindow(hMIDIControlItems[MIDICONTROL_PCNUMBER]);
		WM_HideWindow(hMIDIControlItems[MIDICONTROL_PCSTRING]);
	}
	MisaCheckbox_SetStatus(hMIDIControlItems[MIDICONTROL_INVERT],pInfo->inv);
	MisaRadio_SetStatus(hMIDIControlItems[MIDICONTROL_PITCH], 0==pInfo->type?1:0);
	MisaRadio_SetStatus(hMIDIControlItems[MIDICONTROL_VELOCITY], 1==pInfo->type?1:0);
	MisaRadio_SetStatus(hMIDIControlItems[MIDICONTROL_CC], (2==pInfo->type||3==pInfo->type)?1:0);
	return 0;
}

U8 GetMIDIControlInfo(PMIDICTRL_INFO pInfo)
{
	pInfo->control = control;
	pInfo->type = type;
	pInfo->channel = MisaProgressCombo_GetPercent(hMIDIControlItems[MIDICONTROL_PCCHANNEL]);
	pInfo->string = MisaProgressCombo_GetPercent(hMIDIControlItems[MIDICONTROL_PCSTRING]);
	pInfo->cc = MisaProgressCombo_GetPercent(hMIDIControlItems[MIDICONTROL_PCNUMBER]);
	pInfo->inv = MisaCheckbox_GetStatus(hMIDIControlItems[MIDICONTROL_INVERT]);
	return 0;
}

static void MIDIControlDrawControlcombo(LPMIDICTRL_CTCOMBO pItem)
{
	int textmode;
	GUI_RECT textRect;
	const GUI_FONT* font;
	GUI_DrawBitmap(&bmMC_CTRL_CENTRE, pItem->centre.x0, pItem->centre.y0);
	textmode = GUI_GetTextMode();
	GUI_SetTextMode(GUI_TEXTMODE_TRANS);
	font = GUI_GetFont();
	GUI_SetFont(&GUI_Font24B_ASCII);
	textRect.x0 = pItem->left.x0;
	textRect.x1 = pItem->right.x1;
	textRect.y0 = pItem->centre.y0-32;
	textRect.y1 = pItem->centre.y0;
	GUI_DispStringInRect("CONTROL",&textRect,GUI_TA_HCENTER|GUI_TA_VCENTER);
	GUI_DispStringInRect(strControltype[control],&pItem->centre,GUI_TA_HCENTER|GUI_TA_VCENTER);
	GUI_SetFont(font);
	GUI_SetTextMode(textmode);
}

static void MIDIControlProc(WM_MESSAGE* pMsg)
{
	int x, y;
	int NCode,Id;
	WM_HWIN hFocus;
	switch (pMsg->MsgId)
	{
	case WM_CREATE:
		MIDIControlCreateItems(pMsg->hWin);
		break;
	case WM_DELETE:
		MIDIControlDeleteItems();
		break;
	case WM_PAINT:
		GUI_SetBkColor(GUI_BLACK);
		GUI_Clear();
		GUI_SetTextMode(GUI_TEXTMODE_TRANS);
		x=1;
		y=0;
		GUI_DrawBitmap(&bmEMPTYTITLEBAR, 0, 0);
		GUI_SetFont(&GUI_Font32B_ASCII);
		x = WM_GetWindowSizeX(pMsg->hWin);
		{
			std::string title("MIDI Control");
			GUI_DispStringHCenterAt(GetTitleWithPreset(title).c_str(), x / 2, 5);
		}
		MIDIControlDrawControlcombo(&ctrlCombo);

		//GUI_DrawBitmap(&bmMENU_VLINE,VarMin.x0+13,VarMin.y0);
		//GUI_DrawBitmap(&bmMENU_CUBE,VarMin.x0,VarMax.y0+50);
		//GUI_DrawBitmap(&bmMENU_VLINE,VarMax.x0+13,VarMax.y0);
		//GUI_DrawBitmap(&bmMENU_CUBE,VarMax.x0,VarMax.y0+64);
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
			case MIDICONTROL_ID_LEFTARROW:
				--control;
				if(0>control)
				{
					control = sizeof(strControltype)/4-1;
				}
				WM_HideWindow(hMIDIControlItems[MIDICONTROL_VARIMIN]);
				WM_HideWindow(hMIDIControlItems[MIDICONTROL_VARIMAX]);
				WM_HideWindow(hMIDIControlItems[MIDICONTROL_DRAGCENTRE]);
				if(7 == control)
				{
					WM_HideWindow(hMIDIControlItems[MIDICONTROL_PITCH]);
					WM_HideWindow(hMIDIControlItems[MIDICONTROL_VELOCITY]);
					WM_HideWindow(hMIDIControlItems[MIDICONTROL_CC]);
					WM_HideWindow(hMIDIControlItems[MIDICONTROL_INVERT]);
					
					WM_ShowWindow(hMIDIControlItems[MIDICONTROL_PCCHANNEL]);
					WM_ShowWindow(hMIDIControlItems[MIDICONTROL_PCSTRING]);
					WM_ShowWindow(hMIDIControlItems[MIDICONTROL_PCNUMBER]);
					WM_MoveTo(hMIDIControlItems[MIDICONTROL_PCCHANNEL],5,373);
					MisaProgressCombo_SetPercent(hMIDIControlItems[MIDICONTROL_PCCHANNEL],0);
					MisaProgressCombo_SetRange(hMIDIControlItems[MIDICONTROL_PCCHANNEL], 127, 0);
					MisaProgressCombo_SetText(hMIDIControlItems[MIDICONTROL_PCCHANNEL], "VALUE");
					WM_MoveTo(hMIDIControlItems[MIDICONTROL_PCSTRING],5,160);
					WM_MoveTo(hMIDIControlItems[MIDICONTROL_PCNUMBER],5,266);
					type = 3;
					MisaRadio_SetStatus(hMIDIControlItems[MIDICONTROL_PITCH], 0);
					MisaRadio_SetStatus(hMIDIControlItems[MIDICONTROL_VELOCITY], 0);
					MisaRadio_SetStatus(hMIDIControlItems[MIDICONTROL_CC], 1);
				}
				else
				{
					WM_ShowWindow(hMIDIControlItems[MIDICONTROL_PITCH]);
					WM_ShowWindow(hMIDIControlItems[MIDICONTROL_VELOCITY]);
					WM_ShowWindow(hMIDIControlItems[MIDICONTROL_CC]);
					WM_ShowWindow(hMIDIControlItems[MIDICONTROL_INVERT]);
					WM_MoveTo(hMIDIControlItems[MIDICONTROL_PCCHANNEL],5,266);
					MisaProgressCombo_SetPercent(hMIDIControlItems[MIDICONTROL_PCCHANNEL],0);
					MisaProgressCombo_SetRange(hMIDIControlItems[MIDICONTROL_PCCHANNEL],15,1);
					MisaProgressCombo_SetText(hMIDIControlItems[MIDICONTROL_PCCHANNEL], "CHANNEL");
					WM_ShowWindow(hMIDIControlItems[MIDICONTROL_PCCHANNEL]);
					WM_MoveTo(hMIDIControlItems[MIDICONTROL_PCSTRING],5,266);
					WM_HideWindow(hMIDIControlItems[MIDICONTROL_PCSTRING]);
					WM_MoveTo(hMIDIControlItems[MIDICONTROL_PCNUMBER],5,373);
					WM_ShowWindow(hMIDIControlItems[MIDICONTROL_PCNUMBER]);
					switch(control)
					{
					case 0:
					case 1:
					case 4:
					case 5:
						break;
					case 2:
					case 3:
						if(type)
						{
							WM_ShowWindow(hMIDIControlItems[MIDICONTROL_DRAGCENTRE]);
						}
						break;
					case 6:
						WM_ShowWindow(hMIDIControlItems[MIDICONTROL_VARIMIN]);
						WM_ShowWindow(hMIDIControlItems[MIDICONTROL_VARIMAX]);
						break;
					default:;
					}
				}
				#if 0
				if(sizeof(strControltype)/4-1 == control)
				{
					WM_ShowWindow(hMIDIControlItems[MIDICONTROL_VARIMIN]);
					WM_ShowWindow(hMIDIControlItems[MIDICONTROL_VARIMAX]);
					WM_HideWindow(hMIDIControlItems[MIDICONTROL_DRAGCENTRE]);
				}
				else if((2==control || 3==control) && 0!=type)
				{
					WM_HideWindow(hMIDIControlItems[MIDICONTROL_VARIMIN]);
					WM_HideWindow(hMIDIControlItems[MIDICONTROL_VARIMAX]);
					WM_ShowWindow(hMIDIControlItems[MIDICONTROL_DRAGCENTRE]);
				}
				else
				{
					WM_HideWindow(hMIDIControlItems[MIDICONTROL_VARIMIN]);
					WM_HideWindow(hMIDIControlItems[MIDICONTROL_VARIMAX]);
					WM_HideWindow(hMIDIControlItems[MIDICONTROL_DRAGCENTRE]);
				}
				#endif
				WM_InvalidateRect(pMsg->hWin, &ctrlCombo.centre);
				break;
			case MIDICONTROL_ID_RIGHTARROW:
				++control;
				if(sizeof(strControltype)/4<=control)
				{
					control = 0;
				}
				WM_HideWindow(hMIDIControlItems[MIDICONTROL_VARIMIN]);
				WM_HideWindow(hMIDIControlItems[MIDICONTROL_VARIMAX]);
				WM_HideWindow(hMIDIControlItems[MIDICONTROL_DRAGCENTRE]);
				if(7 == control)
				{
					WM_HideWindow(hMIDIControlItems[MIDICONTROL_PITCH]);
					WM_HideWindow(hMIDIControlItems[MIDICONTROL_VELOCITY]);
					WM_HideWindow(hMIDIControlItems[MIDICONTROL_CC]);
					WM_HideWindow(hMIDIControlItems[MIDICONTROL_INVERT]);
					
					WM_ShowWindow(hMIDIControlItems[MIDICONTROL_PCCHANNEL]);
					WM_ShowWindow(hMIDIControlItems[MIDICONTROL_PCSTRING]);
					WM_ShowWindow(hMIDIControlItems[MIDICONTROL_PCNUMBER]);
					WM_MoveTo(hMIDIControlItems[MIDICONTROL_PCCHANNEL],5,373);
					MisaProgressCombo_SetPercent(hMIDIControlItems[MIDICONTROL_PCCHANNEL],0);
					MisaProgressCombo_SetRange(hMIDIControlItems[MIDICONTROL_PCCHANNEL], 127, 0);
					MisaProgressCombo_SetText(hMIDIControlItems[MIDICONTROL_PCCHANNEL], "VALUE");
					WM_MoveTo(hMIDIControlItems[MIDICONTROL_PCSTRING],5,160);
					WM_MoveTo(hMIDIControlItems[MIDICONTROL_PCNUMBER],5,266);
					type = 3;
					MisaRadio_SetStatus(hMIDIControlItems[MIDICONTROL_PITCH], 0);
					MisaRadio_SetStatus(hMIDIControlItems[MIDICONTROL_VELOCITY], 0);
					MisaRadio_SetStatus(hMIDIControlItems[MIDICONTROL_CC], 1);
				}
				else
				{
					WM_ShowWindow(hMIDIControlItems[MIDICONTROL_PITCH]);
					WM_ShowWindow(hMIDIControlItems[MIDICONTROL_VELOCITY]);
					WM_ShowWindow(hMIDIControlItems[MIDICONTROL_CC]);
					WM_ShowWindow(hMIDIControlItems[MIDICONTROL_INVERT]);
					WM_MoveTo(hMIDIControlItems[MIDICONTROL_PCCHANNEL],5,266);
					MisaProgressCombo_SetPercent(hMIDIControlItems[MIDICONTROL_PCCHANNEL],0);
					MisaProgressCombo_SetRange(hMIDIControlItems[MIDICONTROL_PCCHANNEL],15,1);
					MisaProgressCombo_SetText(hMIDIControlItems[MIDICONTROL_PCCHANNEL], "CHANNEL");
					WM_ShowWindow(hMIDIControlItems[MIDICONTROL_PCCHANNEL]);
					WM_MoveTo(hMIDIControlItems[MIDICONTROL_PCSTRING],5,266);
					WM_HideWindow(hMIDIControlItems[MIDICONTROL_PCSTRING]);
					WM_MoveTo(hMIDIControlItems[MIDICONTROL_PCNUMBER],5,373);
					WM_ShowWindow(hMIDIControlItems[MIDICONTROL_PCNUMBER]);
					switch(control)
					{
					case 0:
					case 1:
					case 4:
					case 5:
						break;
					case 2:
					case 3:
						if(type)
						{
							WM_ShowWindow(hMIDIControlItems[MIDICONTROL_DRAGCENTRE]);
						}
						break;
					case 6:
						WM_ShowWindow(hMIDIControlItems[MIDICONTROL_VARIMIN]);
						WM_ShowWindow(hMIDIControlItems[MIDICONTROL_VARIMAX]);
						break;
					default:;
					}
				}
				#if 0
				if(sizeof(strControltype)/4-1 == control)
				{
					WM_ShowWindow(hMIDIControlItems[MIDICONTROL_VARIMIN]);
					WM_ShowWindow(hMIDIControlItems[MIDICONTROL_VARIMAX]);
					WM_HideWindow(hMIDIControlItems[MIDICONTROL_DRAGCENTRE]);
				}
				else if((2==control || 3==control) && 0!=type)
				{
					WM_HideWindow(hMIDIControlItems[MIDICONTROL_VARIMIN]);
					WM_HideWindow(hMIDIControlItems[MIDICONTROL_VARIMAX]);
					WM_ShowWindow(hMIDIControlItems[MIDICONTROL_DRAGCENTRE]);
				}
				else
				{
					WM_HideWindow(hMIDIControlItems[MIDICONTROL_VARIMIN]);
					WM_HideWindow(hMIDIControlItems[MIDICONTROL_VARIMAX]);
					WM_HideWindow(hMIDIControlItems[MIDICONTROL_DRAGCENTRE]);
				}
				#endif
				WM_InvalidateRect(pMsg->hWin, &ctrlCombo.centre);
				break;
			case MIDICONTROL_ID_PITCH:
				MisaRadio_SetStatus(hMIDIControlItems[MIDICONTROL_VELOCITY], 0);
				MisaRadio_SetStatus(hMIDIControlItems[MIDICONTROL_CC], 0);
				WM_ShowWindow(hMIDIControlItems[MIDICONTROL_PCCHANNEL]);
				WM_HideWindow(hMIDIControlItems[MIDICONTROL_PCSTRING]);
				WM_HideWindow(hMIDIControlItems[MIDICONTROL_PCNUMBER]);
				WM_HideWindow(hMIDIControlItems[MIDICONTROL_DRAGCENTRE]);
				type = 0;
				break;
			case MIDICONTROL_ID_VELOCITY:
				MisaRadio_SetStatus(hMIDIControlItems[MIDICONTROL_PITCH], 0);
				MisaRadio_SetStatus(hMIDIControlItems[MIDICONTROL_CC], 0);
				WM_ShowWindow(hMIDIControlItems[MIDICONTROL_PCSTRING]);
				WM_HideWindow(hMIDIControlItems[MIDICONTROL_PCCHANNEL]);
				WM_HideWindow(hMIDIControlItems[MIDICONTROL_PCNUMBER]);
				if(2==control || 3==control)
				{
					WM_ShowWindow(hMIDIControlItems[MIDICONTROL_DRAGCENTRE]);
				}
				else
				{
					WM_HideWindow(hMIDIControlItems[MIDICONTROL_DRAGCENTRE]);
				}
				type = 1;
				break;
			case MIDICONTROL_ID_CC:
				MisaRadio_SetStatus(hMIDIControlItems[MIDICONTROL_PITCH], 0);
				MisaRadio_SetStatus(hMIDIControlItems[MIDICONTROL_VELOCITY], 0);
				WM_ShowWindow(hMIDIControlItems[MIDICONTROL_PCCHANNEL]);
				WM_ShowWindow(hMIDIControlItems[MIDICONTROL_PCNUMBER]);
				WM_HideWindow(hMIDIControlItems[MIDICONTROL_PCSTRING]);
				if(2==control || 3==control)
				{
					WM_ShowWindow(hMIDIControlItems[MIDICONTROL_DRAGCENTRE]);
				}
				else
				{
					WM_HideWindow(hMIDIControlItems[MIDICONTROL_DRAGCENTRE]);
				}
				type = 2;
				break;
			case MIDICONTROL_ID_CONFIRM:
				hFocus = WM_GetFocussedWindow();
				if(7==control)
				{
					y = CommitInfomation(type,
						MisaProgressCombo_GetPercent(hMIDIControlItems[MIDICONTROL_PCNUMBER]),
						MisaProgressCombo_GetPercent(hMIDIControlItems[MIDICONTROL_PCSTRING]),
						control+CTL_TOUCH_X_ID,
						MisaProgressCombo_GetPercent(hMIDIControlItems[MIDICONTROL_PCCHANNEL]),
						MisaVerSlider_GetPercent(hMIDIControlItems[MIDICONTROL_VARIMIN]),
						MisaVerSlider_GetPercent(hMIDIControlItems[MIDICONTROL_VARIMAX]));
				}
				else
				{
					if(MisaRadio_GetStatus(hMIDIControlItems[MIDICONTROL_VELOCITY]))
					{
						x = MisaProgressCombo_GetPercent(hMIDIControlItems[MIDICONTROL_PCSTRING]);
					}
					else
					{
						x = MisaProgressCombo_GetPercent(hMIDIControlItems[MIDICONTROL_PCCHANNEL]);
					}
					y = CommitInfomation(type,
						MisaProgressCombo_GetPercent(hMIDIControlItems[MIDICONTROL_PCNUMBER]),
						x,
						control+CTL_TOUCH_X_ID,
						MisaCheckbox_GetStatus(hMIDIControlItems[MIDICONTROL_INVERT]),
						MisaVerSlider_GetPercent(hMIDIControlItems[MIDICONTROL_VARIMIN]),
						MisaVerSlider_GetPercent(hMIDIControlItems[MIDICONTROL_VARIMAX]));
				}
				if(CONTROLASSIGNMENTS_SUCCESS!=y)
				{
					switch(y)
					{
					case CONTROLASSIGNMENTS_TOOMANY:
						Misa_WarningBox("Too many assignment!","Maximum 128 controls!",GUI_MESSAGEBOX_CF_MODAL);
						WM_SetFocus(hFocus);
						break;
					case CONTROLASSIGNMENTS_SAMEPAR:
						Misa_WarningBox("Can't assign same parameter","more than once!",GUI_MESSAGEBOX_CF_MODAL);
						WM_SetFocus(hFocus);
						break;
					default:
						Misa_WarningBox("Unknown error!","",GUI_MESSAGEBOX_CF_MODAL);
						WM_SetFocus(hFocus);
					}
					break;
				}
				if(2==control || 3==control)
				{
					switch(type)
					{
					case 0:
						MidiPitchSetDragCentre(x,MisaVerSlider_GetPercent(hMIDIControlItems[MIDICONTROL_DRAGCENTRE]));
						break;
					case 1:
						MidiVelocitySetDragCentre(x,MisaVerSlider_GetPercent(hMIDIControlItems[MIDICONTROL_DRAGCENTRE]));
						break;
					case 2:
						MidiCCSetDragCentre(x,
							MisaProgressCombo_GetPercent(hMIDIControlItems[MIDICONTROL_PCNUMBER]),
							MisaVerSlider_GetPercent(hMIDIControlItems[MIDICONTROL_DRAGCENTRE]));
					}
				}
			case MIDICONTROL_ID_RETURN:
				TopControlAssignmentsScreen(pMsg->hWin);
				break;
			case CTL_NONE_ID:
			case CTL_TOUCH_X_ID:
			case CTL_TOUCH_Y_ID:
			case CTL_DRAG_X_ID:
			case CTL_DRAG_Y_ID:
			case CTL_BALL_X_ID:
			case CTL_BALL_Y_ID:
			case CTL_VARIATION_ID:
				break;
			default:
				;
			}
			break;
		case WM_NOTIFICATION_VALUE_CHANGED:
			x = MisaVerSlider_GetPercent(pMsg->hWinSrc);
			switch(Id)
			{
			case MIDICONTROL_ID_VARIMIN:
				if(x > MisaVerSlider_GetPercent(hMIDIControlItems[MIDICONTROL_VARIMAX]))
				{
					MisaVerSlider_SetPercentEx(hMIDIControlItems[MIDICONTROL_VARIMAX], x, 0);
				}
				break;
			case MIDICONTROL_ID_VARIMAX:
				if(x < MisaVerSlider_GetPercent(hMIDIControlItems[MIDICONTROL_VARIMIN]))
				{
					MisaVerSlider_SetPercentEx(hMIDIControlItems[MIDICONTROL_VARIMIN], x, 0);
				}
			}
			break;
		}
		break;
	case WM_TOUCH:
		break;
	default:
		WM_DefaultProc(pMsg);
	}
}


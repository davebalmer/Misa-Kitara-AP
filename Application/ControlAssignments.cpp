
#include <cstdlib>
#include <iostream>
using namespace std;
#include <vector>

#include "GUI.h"
#include "WM.h"
#include "BUTTON.h"
#include "MisaDialogueBox.h"

#include "MisaWidget.h"

#include "MisaAppInc.h"

#include "MainMenu.h"
#include "SynthScreen.h"
#include "MIDIControlScreen.h"
#include "ControlAssignments.h"

#include "MisamenuConfig.h"

#include "Resource.h"

///////////////////////////////////////////////////////////////////////////////
//

#define CONTROLASSIGNMENTS_TYPE_XPOS 80
#define CONTROLASSIGNMENTS_TYPE_YPOS 120

#define CONTROLASSIGNMENTS_XOFFSET 2
#define CONTROLASSIGNMENTS_YOFFSET 2

///////////////////////////////////////////////////////////////////////////////
//

typedef enum __CONTROLASSIGNMENTSITEMS
{
	
	CONTROLASSIGNMENTS_INDICATOR,
	CONTROLASSIGNMENTS_SLIDEWINDOW,
	CONTROLASSIGNMENTS_NEW,
	CONTROLASSIGNMENTS_EDIT,
	CONTROLASSIGNMENTS_LEARN,
	CONTROLASSIGNMENTS_DEL,
	CONTROLASSIGNMENTS_DELALL,
	CONTROLASSIGNMENTS_CLOSE,
	CONTROLASSIGNMENTS_MAX
} CONTROLASSIGNMENTSITEMS;

typedef enum __CONTROLASSIGNMENTS_ID
{
	CONTROLASSIGNMENTS_ID_NEW=GUI_ID_CONTROLASSIGNMENTS_BASE,
	CONTROLASSIGNMENTS_ID_EDIT,
	CONTROLASSIGNMENTS_ID_LEARN,
	CONTROLASSIGNMENTS_ID_DEL,
	CONTROLASSIGNMENTS_ID_DELALL,
	CONTROLASSIGNMENTS_ID_CLOSE,
	CONTROLASSIGNMENTS_ID_ITEMSBASE,
	CONTROLASSIGNMENTS_ID_MAX
} CONTROLASSIGNMENTS_ID;

///////////////////////////////////////////////////////////////////////////////
//

static WM_HWIN hControlAssignments;
static void ControlAssignmentsProc(WM_MESSAGE* pMsg);
static WM_HWIN hControlAssignmentsItems[CONTROLASSIGNMENTS_MAX];

static WM_HWIN CreateSlidePanel(int x0, int y0, int width, int height, WM_HWIN hParent, U16 Style);
static U8 DeleteSlidePanel(WM_HWIN hSlidePanel);
static U8 SlidingBorder;
static vector<BUTTON_Handle> hCtrlItems;

static U8 ControlAssignmentsLoadParameter(int index, U8 update);
static void ControlAssignmentsControlMenuProc(int menuId);

static U8 ControlAddItem(int para, int cc, int channel, int control, int invert);
static U8 ControlDelAllItem();
static U8 ControlAssignmentParse(char* content,PMIDICTRL_INFO pInfo);

COMMITINFORMATION CommitCallback;

///////////////////////////////////////////////////////////////////////////////
//

static U8 ControlAssignmentsCreateItems(WM_HWIN hParent)
{
	int x,y;
	memset(hControlAssignmentsItems,0,sizeof(hControlAssignmentsItems));
	x = 4;
	y = bmEMPTYTITLEBAR.YSize+bmVOICE_LIST.YSize;
	hControlAssignmentsItems[CONTROLASSIGNMENTS_INDICATOR] = CreateSlidePanel(x,y,WM_GetWindowSizeX(hParent)-8,WM_GetWindowSizeY(hParent)-100-bmEMPTYTITLEBAR.YSize-bmVOICE_LIST.YSize,hParent,WM_CF_SHOW|WM_CF_MEMDEV);
	x = 1;
	y = 502;
	hControlAssignmentsItems[CONTROLASSIGNMENTS_NEW] = BUTTON_CreateAsChild(x,y,bmVOICE_NEW_UN.XSize,bmVOICE_NEW_UN.YSize,hParent,CONTROLASSIGNMENTS_ID_NEW,WM_CF_SHOW|WM_CF_MEMDEV);
	BUTTON_SetFocussable(hControlAssignmentsItems[CONTROLASSIGNMENTS_NEW],0);
	BUTTON_SetBitmap(hControlAssignmentsItems[CONTROLASSIGNMENTS_NEW],BUTTON_CI_UNPRESSED,&bmVOICE_NEW_UN);
	BUTTON_SetBitmap(hControlAssignmentsItems[CONTROLASSIGNMENTS_NEW],BUTTON_CI_PRESSED,&bmVOICE_NEW_SE);
	x += bmVOICE_NEW_UN.XSize;
	x += CONTROLASSIGNMENTS_XOFFSET;
//	MZ hControlAssignmentsItems[CONTROLASSIGNMENTS_LEARN] = BUTTON_CreateAsChild(x,y,bmVOICE_LEARN_UN.XSize,bmVOICE_LEARN_UN.YSize,hParent,CONTROLASSIGNMENTS_ID_LEARN,WM_CF_SHOW|WM_CF_MEMDEV);
	hControlAssignmentsItems[CONTROLASSIGNMENTS_LEARN] = BUTTON_CreateAsChild(x,y,bmVOICE_EDIT_UN.XSize,bmVOICE_EDIT_UN.YSize,hParent,CONTROLASSIGNMENTS_ID_LEARN,WM_CF_SHOW|WM_CF_MEMDEV);
	BUTTON_SetFocussable(hControlAssignmentsItems[CONTROLASSIGNMENTS_LEARN],0);
//	MZ BUTTON_SetBitmap(hControlAssignmentsItems[CONTROLASSIGNMENTS_LEARN],BUTTON_CI_UNPRESSED,&bmVOICE_LEARN_UN);
//	MZ BUTTON_SetBitmap(hControlAssignmentsItems[CONTROLASSIGNMENTS_LEARN],BUTTON_CI_PRESSED,&bmVOICE_LEARN_SE);
	BUTTON_SetBitmap(hControlAssignmentsItems[CONTROLASSIGNMENTS_LEARN],BUTTON_CI_UNPRESSED,&bmVOICE_EDIT_UN);
	BUTTON_SetBitmap(hControlAssignmentsItems[CONTROLASSIGNMENTS_LEARN],BUTTON_CI_PRESSED,&bmVOICE_EDIT_SE);
	// Remove edit button temporary
	hControlAssignmentsItems[CONTROLASSIGNMENTS_EDIT] = 0;//BUTTON_CreateAsChild(x,y,bmVOICE_EDIT_UN.XSize,bmVOICE_EDIT_UN.YSize,hParent,CONTROLASSIGNMENTS_ID_EDIT,WM_CF_SHOW|WM_CF_MEMDEV);
	//BUTTON_SetFocussable(hControlAssignmentsItems[CONTROLASSIGNMENTS_EDIT],0);
	//BUTTON_SetBitmap(hControlAssignmentsItems[CONTROLASSIGNMENTS_EDIT],BUTTON_CI_UNPRESSED,&bmVOICE_EDIT_UN);
	//BUTTON_SetBitmap(hControlAssignmentsItems[CONTROLASSIGNMENTS_EDIT],BUTTON_CI_PRESSED,&bmVOICE_EDIT_SE);
	x += bmVOICE_EDIT_UN.XSize;
	x += CONTROLASSIGNMENTS_XOFFSET;
	hControlAssignmentsItems[CONTROLASSIGNMENTS_DEL] = BUTTON_CreateAsChild(x,y,bmVOICE_DEL_UN.XSize,bmVOICE_DEL_UN.YSize,hParent,CONTROLASSIGNMENTS_ID_DEL,WM_CF_SHOW|WM_CF_MEMDEV);
	BUTTON_SetFocussable(hControlAssignmentsItems[CONTROLASSIGNMENTS_DEL],0);
	BUTTON_SetBitmap(hControlAssignmentsItems[CONTROLASSIGNMENTS_DEL],BUTTON_CI_UNPRESSED,&bmVOICE_DEL_UN);
	BUTTON_SetBitmap(hControlAssignmentsItems[CONTROLASSIGNMENTS_DEL],BUTTON_CI_PRESSED,&bmVOICE_DEL_SE);
	x += bmVOICE_DEL_UN.XSize;
	x += CONTROLASSIGNMENTS_XOFFSET;
	hControlAssignmentsItems[CONTROLASSIGNMENTS_DELALL] = BUTTON_CreateAsChild(x,y,bmVOICE_DELALL_UN.XSize,bmVOICE_DELALL_UN.YSize,hParent,CONTROLASSIGNMENTS_ID_DELALL,WM_CF_SHOW|WM_CF_MEMDEV);
	BUTTON_SetFocussable(hControlAssignmentsItems[CONTROLASSIGNMENTS_DELALL],0);
	BUTTON_SetBitmap(hControlAssignmentsItems[CONTROLASSIGNMENTS_DELALL],BUTTON_CI_UNPRESSED,&bmVOICE_DELALL_UN);
	BUTTON_SetBitmap(hControlAssignmentsItems[CONTROLASSIGNMENTS_DELALL],BUTTON_CI_PRESSED,&bmVOICE_DELALL_SE);
	x += bmVOICE_DELALL_UN.XSize;
	x += CONTROLASSIGNMENTS_XOFFSET;
	hControlAssignmentsItems[CONTROLASSIGNMENTS_CLOSE] = BUTTON_CreateAsChild(x,y,bmCONTROL_BACK_UN.XSize,bmCONTROL_BACK_UN.YSize,hParent,CONTROLASSIGNMENTS_ID_CLOSE,WM_CF_SHOW|WM_CF_MEMDEV);
	BUTTON_SetFocussable(hControlAssignmentsItems[CONTROLASSIGNMENTS_CLOSE],0);
	BUTTON_SetBitmap(hControlAssignmentsItems[CONTROLASSIGNMENTS_CLOSE],BUTTON_CI_UNPRESSED,&bmCONTROL_BACK_UN);
	BUTTON_SetBitmap(hControlAssignmentsItems[CONTROLASSIGNMENTS_CLOSE],BUTTON_CI_PRESSED, &bmCONTROL_BACK_SE);
	return 0;
}

static U8 ControlAssignmentsDeleteItems()
{
	if(hControlAssignmentsItems[CONTROLASSIGNMENTS_INDICATOR])
	{
		DeleteSlidePanel(hControlAssignmentsItems[CONTROLASSIGNMENTS_INDICATOR]);
	}
	if(hControlAssignmentsItems[CONTROLASSIGNMENTS_NEW])
	{
		BUTTON_Delete(hControlAssignmentsItems[CONTROLASSIGNMENTS_NEW]);
	}
	if(hControlAssignmentsItems[CONTROLASSIGNMENTS_LEARN])
	{
		BUTTON_Delete(hControlAssignmentsItems[CONTROLASSIGNMENTS_LEARN]);
	}
	if(hControlAssignmentsItems[CONTROLASSIGNMENTS_EDIT])
	{
		BUTTON_Delete(hControlAssignmentsItems[CONTROLASSIGNMENTS_EDIT]);
	}
	if(hControlAssignmentsItems[CONTROLASSIGNMENTS_DEL])
	{
		BUTTON_Delete(hControlAssignmentsItems[CONTROLASSIGNMENTS_DEL]);
	}
	if(hControlAssignmentsItems[CONTROLASSIGNMENTS_DELALL])
	{
		BUTTON_Delete(hControlAssignmentsItems[CONTROLASSIGNMENTS_DELALL]);
	}
	if(hControlAssignmentsItems[CONTROLASSIGNMENTS_CLOSE])
	{
		BUTTON_Delete(hControlAssignmentsItems[CONTROLASSIGNMENTS_CLOSE]);
	}
	return 0;
}

U8 CreateControlAssignmentsScreen(void* pPara)
{
	int x,y;
	x=GUI_GetScreenSizeX();
	y=GUI_GetScreenSizeY();
	SlidingBorder = 1;
	hControlAssignments = WM_CreateWindow(0, 0, x, y, /*WM_CF_SHOW | */WM_CF_MEMDEV, ControlAssignmentsProc, 0);
	return 0;
}

U8 DeleteControlAssignmentsScreen()
{
	ControlAssignmentsDeleteItems();
	WM_DeleteWindow(hControlAssignments);
	return 0;
}

U8 TopControlAssignmentsScreen(WM_HWIN hPreWin)
{
	WM_HideWindow(hPreWin);
	WM_ShowWindow(hControlAssignments);
	//WM_BringToTop(hControlAssignments);
	CommitCallback = 0;
	SetWindowToUpdateIfPresetModified(hControlAssignments);
	return 0;
}

U8 UpdateControlAssignmentsInfo(PCONTROLASSIGNMENTS_INFO pInfo)
{
	int channel,i,size;
	std::vector<int> effect;
	std::vector<ASSIGNED_EFFECT> cc;
	ControlDelAllItem();
	for(channel=0;channel<=15;channel++)
	{
		effect = MidiPitchFindEffect(channel);
		size = effect.size();
		for(i=0;i<size;i++)
		{
			ControlAddItem(0,0,channel,AssigneMentMenuTranslate(effect[i]&0x000000FF),(effect[i]>>8)&0x000000FF);
		}
	}
	for(channel=0;channel<6;channel++)
	{
		effect = MidiVelocityFindEffect(channel);
		size = effect.size();
		for(i=0;i<size;i++)
		{
			ControlAddItem(1,0,channel,AssigneMentMenuTranslate(effect[i]&0x000000FF),(effect[i]>>8)&0x000000FF);
		}
	}
	for(channel=0;channel<=15;channel++)
	{
		cc = MidiCCFindEffect(channel);
		size = cc.size();
		for(i=0;i<size;i++)
		{
			ControlAddItem(2,cc[i].cc,channel,AssigneMentMenuTranslate(cc[i].type),cc[i].inv);
		}
	}
	for(channel=0;channel<6;channel++)
	{
		std::vector<int> ccs;
		ccs = MidiStopSoundFind(channel);
		size = ccs.size();
		for(i=0;i<size;i++)
		{
			ControlAddItem(2,ccs[i],channel,CTL_STOPSOUND,0);
		}
	}
	return 0;
}

U8 GetControlAssignmentsInfo(PCONTROLASSIGNMENTS_INFO pInfo)
{
#if 0
	WM_InvalidateRect(hControlAssignments, &rectModType);
	pInfo->mod_rate = MisaProgressbar_GetPercent(hControlAssignmentsItems[CONTROLASSIGNMENTS_PROGMODRATE]);
	pInfo->mod_depth = MisaProgressbar_GetPercent(hControlAssignmentsItems[CONTROLASSIGNMENTS_PROGMODDEPTH]);
	pInfo->level = MisaProgressbar_GetPercent(hControlAssignmentsItems[CONTROLASSIGNMENTS_PROGLEVEL]);
	pInfo->fdbk = MisaProgressbar_GetPercent(hControlAssignmentsItems[CONTROLASSIGNMENTS_PROGFDBK]);
	pInfo->fdbkfilter = MisaProgressbar_GetPercent(hControlAssignmentsItems[CONTROLASSIGNMENTS_PROGFDBKFILTER]);
	pInfo->shape = MisaProgressbar_GetPercent(hControlAssignmentsItems[CONTROLASSIGNMENTS_PROGSHAPE]);
	pInfo->time = MisaProgressbar_GetPercent(hControlAssignmentsItems[CONTROLASSIGNMENTS_PROGTIME]);
#endif
	return 0;
}

static void ControlAssignmentsProc(WM_MESSAGE* pMsg)
{
	int x, y;
	int NCode,Id;
	char buf[32];
	MIDICTRL_INFO midinfo;
	WM_HWIN hFocus;
	switch (pMsg->MsgId)
	{
	case WM_CREATE:
		ControlAssignmentsCreateItems(pMsg->hWin);
		break;
	case WM_DELETE:
		ControlAssignmentsDeleteItems();
		break;
	case WM_PAINT:
		GUI_SetBkColor(GUI_BLACK);
		GUI_Clear();
		GUI_SetTextMode(GUI_TEXTMODE_TRANS);
		GUI_DrawBitmap(&bmEMPTYTITLEBAR, 0, 0);
		x = 4;
		y = bmEMPTYTITLEBAR.YSize;
		GUI_DrawBitmap(&bmVOICE_LIST, x, y);
		//y += bmVOICE_LIST.YSize;
		//GUI_DrawBitmap(&bmVOICE_SELECTED, x, y);
		GUI_SetFont(&GUI_Font32B_ASCII);
		x = WM_GetWindowSizeX(pMsg->hWin);
		{
			std::string title("Control Assignments");
			GUI_DispStringHCenterAt(GetTitleWithPreset(title).c_str(), x / 2, 5);
		}
		// Draw slide indicator
		/*y = WM_GetWindowOrgY(hControlAssignmentsItems[CONTROLASSIGNMENTS_SLIDEWINDOW]);
		y += 170;
		if(0x02 &SlidingBorder)
		{
			x = 4;
			GUI_DrawBitmap(&bmLARROW,x,y);
		}
		if(0x01 & SlidingBorder)
		{
			x = 750 + 15;
			GUI_DrawBitmap(&bmRARROW,x,y);
		}*/
		break;
	case WM_NOTIFY_PARENT:
		Id = WM_GetId(pMsg->hWinSrc);
		NCode = pMsg->Data.v;
		switch(NCode)
		{
		case WM_NOTIFICATION_CLICKED:
			DEBUG_STDOUT("CONTROLASSIGNMENTS button clicked!");
			break;
		case WM_NOTIFICATION_RELEASED:
			switch(Id)
			{
			case CONTROLASSIGNMENTS_ID_NEW:
				CommitCallback = ControlAssignmentsAddItem;
				memset(&midinfo,0,sizeof(MIDICTRL_INFO));
				UpdateMIDIControlInfo(&midinfo);
				TopMIDIControlScreen(pMsg->hWin);
				break;
			case CONTROLASSIGNMENTS_ID_LEARN:
				hFocus = WM_GetFocussedWindow();
				if(hFocus)
				{
					if(GUI_ID_OK == Misa_ConfirmBox("Prepare receiver...","Learn",GUI_MESSAGEBOX_CF_MODAL))
					{
						WM_SetFocus(hFocus);
						ControlAssignmentsSendLearn(hControlAssignmentsItems[CONTROLASSIGNMENTS_SLIDEWINDOW]);
					}
					else
					{
						WM_SetFocus(hFocus);
					}
				}
				break;
			case CONTROLASSIGNMENTS_ID_EDIT:
				hFocus = WM_GetFocussedWindow();
				if(hFocus)
				{
					BUTTON_GetText(hFocus,buf,32);
					ControlAssignmentParse(buf,&midinfo);
					CommitCallback = ControlAssignmentsEditItem;
					UpdateMIDIControlInfo(&midinfo);
					TopMIDIControlScreen(pMsg->hWin);
				}
				break;
			case CONTROLASSIGNMENTS_ID_DEL:
				hFocus = WM_GetFocussedWindow();
				if(hFocus)
				{
					if(GUI_ID_OK == Misa_ConfirmBox("Are you sure?","Delete this voice!",GUI_MESSAGEBOX_CF_MODAL))
					{
						WM_SetFocus(hFocus);
						ControlAssignmentsDelItem(hControlAssignmentsItems[CONTROLASSIGNMENTS_SLIDEWINDOW]);
					}
					else
					{
						WM_SetFocus(hFocus);
					}
				}
				break;
			case CONTROLASSIGNMENTS_ID_DELALL:
				if(GUI_ID_OK == Misa_ConfirmBox("Are you sure? This will delete ALL items.","Delete this voice!",GUI_MESSAGEBOX_CF_MODAL))
				{
					ControlAssignmentsDelAllItem();
				}
				break;
			case CONTROLASSIGNMENTS_ID_CLOSE:
				TopSynthScreen(pMsg->hWin);
				break;
			case CTL_NONE_ID:
			case CTL_TOUCH_X_ID:
			case CTL_TOUCH_Y_ID:
			case CTL_DRAG_X_ID:
			case CTL_DRAG_Y_ID:
			case CTL_BALL_X_ID:
			case CTL_BALL_Y_ID:
			case CTL_VARIATION_ID:
				ControlAssignmentsControlMenuProc(Id);
				break;
			default:
				;
			}
			DEBUG_STDOUT("CONTROLASSIGNMENTS button released!");
			break;
		}
		break;
	case WM_TOUCH:
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
static U8 SlideDeleteItems();

static U8 IndicatorCreateItems(WM_HWIN hParent)
{
	hControlAssignmentsItems[CONTROLASSIGNMENTS_SLIDEWINDOW] = WM_CreateWindowAsChild(0,0,WM_GetWindowSizeX(hParent),WM_GetWindowSizeY(hParent),hParent,WM_CF_SHOW|WM_CF_MEMDEV,SlideWindowProc,0);
	return 0;
}

static U8 IndicatorDeleteItems()
{
	if(hControlAssignmentsItems[CONTROLASSIGNMENTS_SLIDEWINDOW])
	{
		WM_DeleteWindow(hControlAssignmentsItems[CONTROLASSIGNMENTS_SLIDEWINDOW]);
	}
	return 0;
}

WM_HWIN CreateSlidePanel(int x0, int y0, int width, int height, WM_HWIN hParent, U16 Style)
{
	return WM_CreateWindowAsChild(x0,y0,width,height,hParent,Style,IndicatorProc,0);
}

static U8 DeleteSlidePanel(WM_HWIN hSlidePanel)
{
	SlideDeleteItems();
	IndicatorDeleteItems();
	WM_DeleteWindow(hSlidePanel);
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
	hCtrlItems.clear();
	/*BUTTON_Handle hMisaRadio = MisaItem_CreateEx(0, 0, bmVOICE_SELECTED.XSize, bmVOICE_SELECTED.YSize,
			hParent, CONTROLASSIGNMENTS_ID_ITEMSBASE, WM_CF_SHOW|WM_CF_MEMDEV, "\tvelocity\t0\ttouch_x\tno",0, &bmVOICE_SELECTED);
	BUTTON_Handle hMisaRadio1 = MisaItem_CreateEx(0, bmVOICE_SELECTED.YSize, bmVOICE_SELECTED.XSize, bmVOICE_SELECTED.YSize,
			hParent, CONTROLASSIGNMENTS_ID_ITEMSBASE+1, WM_CF_SHOW|WM_CF_MEMDEV, "\tpitch\t0\ttouch_x\tyes",0, &bmVOICE_SELECTED);
	hCtrlItems.push_back(hMisaRadio);
	hCtrlItems.push_back(hMisaRadio1);*/
	return 0;
}

static U8 SlideDeleteItems()
{
	int i;
	for(i=0; i<hCtrlItems.size();i++)
	{
		MisaItem_Delete(hCtrlItems[i]);
	}
	hCtrlItems.clear();
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
		//GUI_SetBkColor(GUI_BLACK);
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
		case WM_NOTIFICATION_MOVED_OUT:
			if(!WM_HasCaptured(pMsg->hWin))
			{
				WM_SetCapture(pMsg->hWin,0);
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
				int dy;
				if(0 == PID_LastState.Pressed)
				{
					WM_SetCapture(pMsg->hWin,0);
					PID_LastState = *pPID_State;
				}
				else
				{	//sliding
					y = WM_GetWindowOrgY(pMsg->hWin);
					x = WM_GetWindowSizeY(pMsg->hWin);
					dy = pPID_State->y-PID_LastState.y;
					if(dy > 0)
					{
						WM_MoveWindow(pMsg->hWin,0,dy>bmEMPTYTITLEBAR.YSize+bmVOICE_LIST.YSize-y?bmEMPTYTITLEBAR.YSize+bmVOICE_LIST.YSize-y:dy);
					}
					else if(dy < 0)
					{
						WM_MoveWindow(pMsg->hWin,0,dy<GUI_GetScreenSizeY()-100-y-x?GUI_GetScreenSizeY()-100-y-x:dy);
					}
					//WM_MoveWindow(pMsg->hWin,0,pPID_State->y-PID_LastState.y);
				}
			}
			else
			{
				WM_ReleaseCapture();
				PID_LastState.Pressed = 0;
				y = WM_GetWindowOrgY(pMsg->hWin);
				x = WM_GetWindowSizeY(pMsg->hWin);
				if(bmEMPTYTITLEBAR.YSize+bmVOICE_LIST.YSize < y)
				{
					WM_MoveWindow(pMsg->hWin,0,bmEMPTYTITLEBAR.YSize+bmVOICE_LIST.YSize-y);
				}
				else if(GUI_GetScreenSizeY()-bmVOICE_LIST.YSize > y+x)
				{
					WM_MoveWindow(pMsg->hWin,0,GUI_GetScreenSizeY()-100-y-x);
				}
				SlidingBorder = GetSlidingBordercheck(pMsg->hWin,hControlAssignments);
			}
		}
		else
		{
			WM_ReleaseCapture();
			PID_LastState.Pressed = 0;
		}
		break;
	default:
		WM_DefaultProc(pMsg);
	}
}

static void ControlAssignmentsControlMenuProc(int menuId)
{
}

static void SorControlList(int pos)
{
	int i,x,y;
	x = WM_GetWindowOrgX(hControlAssignmentsItems[CONTROLASSIGNMENTS_SLIDEWINDOW]);
	y = WM_GetWindowOrgY(hControlAssignmentsItems[CONTROLASSIGNMENTS_SLIDEWINDOW]);
	vector<BUTTON_Handle>::iterator iter;
	if(pos < hCtrlItems.size())
	{
		for(i=pos,iter=hCtrlItems.begin()+pos;iter!=hCtrlItems.end();iter++,i++)
		{
			WM_SetId(*iter, CONTROLASSIGNMENTS_ID_ITEMSBASE+i);
			WM_MoveTo(*iter, x, y+i*bmVOICE_SELECTED.YSize);
		}
	}
}

static int IsControlExist(BUTTON_Handle hBt,char* content)
{
	int cmplen,ret;
	char* pdest;
	char buf[32];
	ret = 0;
	pdest = strrchr(content,'\t');
	if(0==pdest)
	{
		return -1;
	}
	cmplen = pdest-content;
	vector<BUTTON_Handle>::iterator iter;
	for(iter=hCtrlItems.begin();iter!=hCtrlItems.end();iter++)
	{
		if(hBt==*iter)
		{
			continue;
		}
		BUTTON_GetText(*iter, buf, 32);
		if(!memcmp(content,buf,cmplen))
		{
			ret = 1;
			break;
		}
	}
	return ret;
}

static U8 ControlAssignmentParse(char* content,PMIDICTRL_INFO pInfo)
{
	int ret;
	char *pcur,*pdest;
	char buf[32];
	if(!memcmp(content,"\tpitch",6))
	{
		pInfo->type= 0;
		pcur = content+=7;
	}
	else if(!memcmp(content,"\tvelocity",9))
	{
		pInfo->type = 1;
		pcur = content+=10;
	}
	else if(!memcmp(content,"\tCC   ",6))
	{
		pInfo->type = 2;
		pcur = content+1;
		pdest = strchr(pcur,'\t');
		pcur = content+6;
		memcpy(buf,pcur,pdest-pcur);
		buf[pdest-pcur] = 0;
		pInfo->cc = atoi(buf);
		pcur = pdest+1;
	}
	else
	{
	}
	pdest = strchr(pcur,'\t');
	memcpy(buf,pcur,pdest-pcur);
	buf[pdest-pcur] = 0;
	ret = atoi(buf);
	if(ret>0)
	{
		--ret;
	}
	pInfo->channel = pInfo->string = ret;
	pcur = pdest+1;
	if(!memcmp(pcur,"touch_x",7))
	{
		pInfo->control = CTL_TOUCH_X_ID;
		pcur+=8;
	}
	else if(!memcmp(pcur,"touch_y",7))
	{
		pInfo->control = CTL_TOUCH_Y_ID;
		pcur+=8;
	}
	else if(!memcmp(pcur,"ball_x",6))
	{
		pInfo->control = CTL_BALL_X_ID;
		pcur+=7;
	}
	else if(!memcmp(pcur,"ball_y",6))
	{
		pInfo->control = CTL_BALL_Y_ID;
		pcur+=7;
	}
	else if(!memcmp(pcur,"drag_x",6))
	{
		pInfo->control = CTL_DRAG_X_ID;
		pcur+=7;
	}
	else if(!memcmp(pcur,"drag_y",6))
	{
		pInfo->control = CTL_DRAG_Y_ID;
		pcur+=7;
	}
	else if(!memcmp(pcur,"variation",9))
	{
		pInfo->control = CTL_VARIATION_ID;
		pcur+=10;
	}
	else if(!memcmp(pcur,"stop_sound",10))
	{
		pInfo->control = CTL_STOPSOUND_ID;
		pInfo->type = 3;
		pcur+=11;
	}
	return ret;
}

U8 ControlAssignmentsAddItem(int para, int cc, int channel, int control, int invert, unsigned int variation_start, unsigned int variation_end)
{
	int pos,x,y;
	char buf[32],temp[16];
	pos = hCtrlItems.size();
	if(pos>128)
	{
		return CONTROLASSIGNMENTS_TOOMANY;
	}
	memset(buf,0,32);
	switch(para)
	{
	case 0:
		strcpy(buf,"\tpitch");
		break;
	case 1:
		strcpy(buf,"\tvelocity");
		break;
	case 2:
	default:
		sprintf(buf,"\tCC   %d",cc);
	}
	strcat(buf,"\t");
	sprintf(temp,"%d",channel+1);
	strcat(buf,temp);
	strcat(buf,"\t");
	switch(control)
	{
	case CTL_TOUCH_X_ID:
		strcat(buf,"touch_x");
		break;
	case CTL_TOUCH_Y_ID:
		strcat(buf,"touch_y");
		break;
	case CTL_DRAG_X_ID:
		strcat(buf,"drag_x");
		break;
	case CTL_DRAG_Y_ID:
		strcat(buf,"drag_y");
		break;
	case CTL_BALL_X_ID:
		strcat(buf,"ball_x");
		break;
	case CTL_BALL_Y_ID:
		strcat(buf,"ball_y");
		break;
	case CTL_VARIATION_ID:
		strcat(buf,"variation");
		break;
	case CTL_STOPSOUND_ID:
	default:
		strcat(buf,"stop_sound");
	}
	strcat(buf,"\t");
	if(control == CTL_STOPSOUND_ID)
	{
		strcat(buf, " ");
	}
	else
	{
		if(invert)
		{
			strcat(buf,"yes");
		}
		else
		{
			strcat(buf,"no");
		}
	}
	if(IsControlExist(0,buf))
	{
		return CONTROLASSIGNMENTS_SAMEPAR;
	}
	BUTTON_Handle hMisaItem = MisaItem_CreateEx(0, bmVOICE_SELECTED.YSize*pos, bmVOICE_SELECTED.XSize, bmVOICE_SELECTED.YSize,
			hControlAssignmentsItems[CONTROLASSIGNMENTS_SLIDEWINDOW], CONTROLASSIGNMENTS_ID_ITEMSBASE+pos, WM_CF_SHOW|WM_CF_MEMDEV, buf,0, &bmVOICE_SELECTED);
	hCtrlItems.push_back(hMisaItem);
	x = WM_GetWindowSizeX(hControlAssignmentsItems[CONTROLASSIGNMENTS_SLIDEWINDOW]);
	y = bmVOICE_SELECTED.YSize*hCtrlItems.size();
	WM_SetSize(hControlAssignmentsItems[CONTROLASSIGNMENTS_SLIDEWINDOW], x, WM_GetWindowSizeY(hControlAssignmentsItems[CONTROLASSIGNMENTS_INDICATOR])<y?y:WM_GetWindowSizeY(hControlAssignmentsItems[CONTROLASSIGNMENTS_INDICATOR]));
	switch(para)
	{
	case 0:
		SynthAssignMidiPitch(SynthTranslateCtrl(control),channel,invert?true:false,variation_start,variation_end);
		break;
	case 1:
		SynthAssignMidiVelocity(SynthTranslateCtrl(control),channel,invert?true:false,variation_start,variation_end);
		break;
	case 2:
	default:
		SynthAssignCC(SynthTranslateCtrl(control),channel,cc,invert?true:false,variation_start,variation_end);
		break;
	case 3:
		SynthAssignStopSound(channel,cc,invert); //chaniel hacked this - invert is actually the value to send
	}
	return CONTROLASSIGNMENTS_SUCCESS;
}

U8 ControlAssignmentsEditItem(int para, int cc, int channel, int control, int invert, unsigned int variation_start, unsigned int variation_end)
{
	int pos,ret;
	char buf[32],temp[32];
	ret = CONTROLASSIGNMENTS_UNKNOWN;
	WM_HWIN hFocus = WM_GetFocussedWindow();
	if(WM_GetParent(hFocus) != hControlAssignmentsItems[CONTROLASSIGNMENTS_SLIDEWINDOW])
	{
		return ret;
	}
	if(hFocus)
	{
		pos = WM_GetId(hFocus)-CONTROLASSIGNMENTS_ID_ITEMSBASE;
		if(pos < hCtrlItems.size())
		{
			memset(buf,0,32);
			switch(para)
			{
			case 0:
				strcpy(buf,"\tpitch");
				break;
			case 1:
				strcpy(buf,"\tvelocity");
				break;
			case 2:
			default:
				sprintf(buf,"\tCC   %d",cc);
			}
			strcat(buf,"\t");
			sprintf(temp,"%d",channel+1);
			strcat(buf,temp);
			strcat(buf,"\t");
			switch(control)
			{
			case CTL_TOUCH_X_ID:
				strcat(buf,"touch_x");
				break;
			case CTL_TOUCH_Y_ID:
				strcat(buf,"touch_y");
				break;
			case CTL_DRAG_X_ID:
				strcat(buf,"drag_x");
				break;
			case CTL_DRAG_Y_ID:
				strcat(buf,"drag_y");
				break;
			case CTL_BALL_X_ID:
				strcat(buf,"ball_x");
				break;
			case CTL_BALL_Y_ID:
				strcat(buf,"ball_y");
				break;
			case CTL_VARIATION_ID:
				strcat(buf,"variation");
				break;
			case CTL_STOPSOUND_ID:
			default:
				strcat(buf,"stop_sound");
			}
			strcat(buf,"\t");
			if(control == CTL_STOPSOUND_ID)
			{
				strcat(buf, " ");
			}
			else
			{
				if(invert)
				{
					strcat(buf,"yes");
				}
				else
				{
					strcat(buf,"no");
				}
			}
			if(!IsControlExist(hFocus,buf))
			{
				BUTTON_SetText(hFocus,buf);
				ret = CONTROLASSIGNMENTS_SUCCESS;
			}
		}
	}
	
	return ret;
}

U8 ControlAssignmentsDelItem(int pos)
{
	char buf[32];
	int x,y;
	MIDICTRL_INFO midiinfo;
	WM_HWIN hFocus = WM_GetFocussedWindow();
	if(WM_GetParent(hFocus) != hControlAssignmentsItems[CONTROLASSIGNMENTS_SLIDEWINDOW])
	{
		return CONTROLASSIGNMENTS_UNKNOWN;
	}
	if(hFocus)
	{
		pos = WM_GetId(hFocus)-CONTROLASSIGNMENTS_ID_ITEMSBASE;
		if(pos < hCtrlItems.size())
		{
			BUTTON_GetText(hCtrlItems[pos],buf,32);
			ControlAssignmentParse(buf,&midiinfo);
			switch(midiinfo.type)
			{
			case 0:
				SynthUnassignMidiPitch(SynthTranslateCtrl(midiinfo.control),midiinfo.channel);
				break;
			case 1:
				SynthUnassignMidiVelocity(SynthTranslateCtrl(midiinfo.control),midiinfo.string);
				break;
			case 2:
			default:
				SynthUnassignCC(SynthTranslateCtrl(midiinfo.control),midiinfo.channel,midiinfo.cc);
				break;
			case 3:
				SynthUnassignStopSound(midiinfo.string, midiinfo.cc);
			}
			MisaItem_Delete(hCtrlItems[pos]);
			hCtrlItems.erase(hCtrlItems.begin()+pos);
			SorControlList(pos);
			x = WM_GetWindowSizeX(hControlAssignmentsItems[CONTROLASSIGNMENTS_SLIDEWINDOW]);
			y = bmVOICE_SELECTED.YSize*hCtrlItems.size();
			WM_SetSize(hControlAssignmentsItems[CONTROLASSIGNMENTS_SLIDEWINDOW], x, WM_GetWindowSizeY(hControlAssignmentsItems[CONTROLASSIGNMENTS_INDICATOR])<y?y:WM_GetWindowSizeY(hControlAssignmentsItems[CONTROLASSIGNMENTS_INDICATOR]));
			y = WM_GetWindowOrgY(hControlAssignmentsItems[CONTROLASSIGNMENTS_SLIDEWINDOW]);
			x = WM_GetWindowSizeY(hControlAssignmentsItems[CONTROLASSIGNMENTS_SLIDEWINDOW]);
			if(bmEMPTYTITLEBAR.YSize+bmVOICE_LIST.YSize < y)
			{
				WM_MoveWindow(hControlAssignmentsItems[CONTROLASSIGNMENTS_SLIDEWINDOW],0,bmEMPTYTITLEBAR.YSize+bmVOICE_LIST.YSize-y);
			}
			else if(GUI_GetScreenSizeY()-bmVOICE_LIST.YSize > y+x)
			{
				WM_MoveWindow(hControlAssignmentsItems[CONTROLASSIGNMENTS_SLIDEWINDOW],0,GUI_GetScreenSizeY()-100-y-x);
			}
			return CONTROLASSIGNMENTS_SUCCESS;
		}
		else
		{
			return CONTROLASSIGNMENTS_TOOMANY;
		}
	}
	return CONTROLASSIGNMENTS_UNKNOWN;
}

U8 ControlAssignmentsSendLearn(int pos)
{
	char buf[32];
	int x,y;
	MIDICTRL_INFO midiinfo;
	WM_HWIN hFocus = WM_GetFocussedWindow();
	if(WM_GetParent(hFocus) != hControlAssignmentsItems[CONTROLASSIGNMENTS_SLIDEWINDOW])
	{
		return CONTROLASSIGNMENTS_UNKNOWN;
	}
	if(hFocus)
	{
		pos = WM_GetId(hFocus)-CONTROLASSIGNMENTS_ID_ITEMSBASE;
		if(pos < hCtrlItems.size())
		{
			BUTTON_GetText(hCtrlItems[pos],buf,32);
			ControlAssignmentParse(buf,&midiinfo);
			switch(midiinfo.type)
			{
			case 0:
				//don't send anything for pitch
				break;
			case 1:
				//don't send anything for velocity
				break;
			case 2:
			default:
				SynthSendLearnCC(midiinfo.channel,midiinfo.cc);
				break;
			case 3:
				SynthSendLearnStopSound(midiinfo.string, midiinfo.cc);
				break;
			}
			/*MisaItem_Delete(hCtrlItems[pos]);
			hCtrlItems.erase(hCtrlItems.begin()+pos);
			SorControlList(pos);
			x = WM_GetWindowSizeX(hControlAssignmentsItems[CONTROLASSIGNMENTS_SLIDEWINDOW]);
			y = bmVOICE_SELECTED.YSize*hCtrlItems.size();
			WM_SetSize(hControlAssignmentsItems[CONTROLASSIGNMENTS_SLIDEWINDOW], x, WM_GetWindowSizeY(hControlAssignmentsItems[CONTROLASSIGNMENTS_INDICATOR])<y?y:WM_GetWindowSizeY(hControlAssignmentsItems[CONTROLASSIGNMENTS_INDICATOR]));
			y = WM_GetWindowOrgY(hControlAssignmentsItems[CONTROLASSIGNMENTS_SLIDEWINDOW]);
			x = WM_GetWindowSizeY(hControlAssignmentsItems[CONTROLASSIGNMENTS_SLIDEWINDOW]);
			if(bmEMPTYTITLEBAR.YSize+bmVOICE_LIST.YSize < y)
			{
				WM_MoveWindow(hControlAssignmentsItems[CONTROLASSIGNMENTS_SLIDEWINDOW],0,bmEMPTYTITLEBAR.YSize+bmVOICE_LIST.YSize-y);
			}
			else if(GUI_GetScreenSizeY()-bmVOICE_LIST.YSize > y+x)
			{
				WM_MoveWindow(hControlAssignmentsItems[CONTROLASSIGNMENTS_SLIDEWINDOW],0,GUI_GetScreenSizeY()-100-y-x);
			}*/
			return CONTROLASSIGNMENTS_SUCCESS;
		}
		else
		{
			return CONTROLASSIGNMENTS_TOOMANY;
		}
	}
	return CONTROLASSIGNMENTS_UNKNOWN;
}

U8 ControlAssignmentsDelAllItem()
{
	char buf[32];
	MIDICTRL_INFO midiinfo;
	int x,y,i,size;
	size = hCtrlItems.size();
	for(i=0;i<size;i++)
	{
		BUTTON_GetText(hCtrlItems[i],buf,32);
		ControlAssignmentParse(buf,&midiinfo);
		switch(midiinfo.type)
		{
		case 0:
			SynthUnassignMidiPitch(SynthTranslateCtrl(midiinfo.control),midiinfo.channel);
			break;
		case 1:
			SynthUnassignMidiVelocity(SynthTranslateCtrl(midiinfo.control),midiinfo.string);
			break;
		case 2:
		default:
			SynthUnassignCC(SynthTranslateCtrl(midiinfo.control),midiinfo.channel,midiinfo.cc);
			break;
		case 3:
			SynthUnassignStopSound(midiinfo.string, midiinfo.cc);
		}
		MisaItem_Delete(hCtrlItems[i]);
	}
	hCtrlItems.clear();
	x = WM_GetWindowSizeX(hControlAssignmentsItems[CONTROLASSIGNMENTS_SLIDEWINDOW]);
	WM_SetSize(hControlAssignmentsItems[CONTROLASSIGNMENTS_SLIDEWINDOW], x, WM_GetWindowSizeY(hControlAssignmentsItems[CONTROLASSIGNMENTS_INDICATOR]));
	y = WM_GetWindowOrgY(hControlAssignmentsItems[CONTROLASSIGNMENTS_SLIDEWINDOW]);
	x = WM_GetWindowSizeY(hControlAssignmentsItems[CONTROLASSIGNMENTS_SLIDEWINDOW]);
	if(bmEMPTYTITLEBAR.YSize+bmVOICE_LIST.YSize < y)
	{
		WM_MoveWindow(hControlAssignmentsItems[CONTROLASSIGNMENTS_SLIDEWINDOW],0,bmEMPTYTITLEBAR.YSize+bmVOICE_LIST.YSize-y);
	}
	else if(GUI_GetScreenSizeY()-bmVOICE_LIST.YSize > y+x)
	{
		WM_MoveWindow(hControlAssignmentsItems[CONTROLASSIGNMENTS_SLIDEWINDOW],0,GUI_GetScreenSizeY()-100-y-x);
	}
	return CONTROLASSIGNMENTS_SUCCESS;
}

U8 CommitInfomation(int para, int cc, int channel, int control, int invert, unsigned int variation_start, unsigned int variation_end)
{
	if(CommitCallback)
	{
		return CommitCallback(para, cc, channel, control, invert,variation_start,variation_end);
	}
	return CONTROLASSIGNMENTS_UNKNOWN;
}

static U8 ControlAddItem(int para, int cc, int channel, int control, int invert)
{
	int pos,x,y;
	char buf[32],temp[16];	
	pos = hCtrlItems.size();
	if(pos>128)
	{
		return CONTROLASSIGNMENTS_TOOMANY;
	}
	memset(buf,0,32);
	switch(para)
	{
	case 0:
		strcpy(buf,"\tpitch");
		break;
	case 1:
		strcpy(buf,"\tvelocity");
		break;
	case 2:
	default:
		sprintf(buf,"\tCC   %d",cc);
	}
	strcat(buf,"\t");
	sprintf(temp,"%d",channel+1);
	strcat(buf,temp);
	strcat(buf,"\t");
	switch(control)
	{
	case CTL_TOUCH_X:
		strcat(buf,"touch_x");
		break;
	case CTL_TOUCH_Y:
		strcat(buf,"touch_y");
		break;
	case CTL_DRAG_X:
		strcat(buf,"drag_x");
		break;
	case CTL_DRAG_Y:
		strcat(buf,"drag_y");
		break;
	case CTL_BALL_X:
		strcat(buf,"ball_x");
		break;
	case CTL_BALL_Y:
		strcat(buf,"ball_y");
		break;
	case CTL_VARIATION:
		strcat(buf,"variation");
		break;
	case CTL_STOPSOUND:
	default:
		strcat(buf,"stop_sound");
	}
	strcat(buf,"\t");
	if(control == CTL_STOPSOUND)
	{
		strcat(buf, " ");
	}
	else
	{
		if(invert)
		{
			strcat(buf,"yes");
		}
		else
		{
			strcat(buf,"no");
		}
	}
	if(IsControlExist(0,buf))
	{
		return CONTROLASSIGNMENTS_SAMEPAR;
	}
	BUTTON_Handle hMisaItem = MisaItem_CreateEx(0, bmVOICE_SELECTED.YSize*pos, bmVOICE_SELECTED.XSize, bmVOICE_SELECTED.YSize,
			hControlAssignmentsItems[CONTROLASSIGNMENTS_SLIDEWINDOW], CONTROLASSIGNMENTS_ID_ITEMSBASE+pos, WM_CF_SHOW|WM_CF_MEMDEV, buf,0, &bmVOICE_SELECTED);
	hCtrlItems.push_back(hMisaItem);
	x = WM_GetWindowSizeX(hControlAssignmentsItems[CONTROLASSIGNMENTS_SLIDEWINDOW]);
	y = bmVOICE_SELECTED.YSize*hCtrlItems.size();
	WM_SetSize(hControlAssignmentsItems[CONTROLASSIGNMENTS_SLIDEWINDOW], x, WM_GetWindowSizeY(hControlAssignmentsItems[CONTROLASSIGNMENTS_INDICATOR])<y?y:WM_GetWindowSizeY(hControlAssignmentsItems[CONTROLASSIGNMENTS_INDICATOR]));
	return CONTROLASSIGNMENTS_SUCCESS;
}

static U8 ControlDelAllItem()
{
	int x,y,i,size;
	size = hCtrlItems.size();
	for(i=0;i<size;i++)
	{
		MisaItem_Delete(hCtrlItems[i]);
	}
	hCtrlItems.clear();
	x = WM_GetWindowSizeX(hControlAssignmentsItems[CONTROLASSIGNMENTS_SLIDEWINDOW]);
	WM_SetSize(hControlAssignmentsItems[CONTROLASSIGNMENTS_SLIDEWINDOW], x, WM_GetWindowSizeY(hControlAssignmentsItems[CONTROLASSIGNMENTS_INDICATOR]));
	y = WM_GetWindowOrgY(hControlAssignmentsItems[CONTROLASSIGNMENTS_SLIDEWINDOW]);
	x = WM_GetWindowSizeY(hControlAssignmentsItems[CONTROLASSIGNMENTS_SLIDEWINDOW]);
	if(bmEMPTYTITLEBAR.YSize+bmVOICE_LIST.YSize < y)
	{
		WM_MoveWindow(hControlAssignmentsItems[CONTROLASSIGNMENTS_SLIDEWINDOW],0,bmEMPTYTITLEBAR.YSize+bmVOICE_LIST.YSize-y);
	}
	else if(GUI_GetScreenSizeY()-bmVOICE_LIST.YSize > y+x)
	{
		WM_MoveWindow(hControlAssignmentsItems[CONTROLASSIGNMENTS_SLIDEWINDOW],0,GUI_GetScreenSizeY()-100-y-x);
	}
	return CONTROLASSIGNMENTS_SUCCESS;
}


#include "PresetsScreen.h"
#include "PresetSlideList.h"
#include "BUTTON.h"
#include "MisaWidget.h"
#include "Resource.h"

static void SlideWindowProc(WM_MESSAGE* pMsg);



static void DelimiterWindowProc(WM_MESSAGE* pMsg)
{
	PresetSlideList *pPresetSlideListObj;
	WM_HWIN hSlideWin;
	WM_GetUserData(pMsg->hWin, &pPresetSlideListObj, sizeof(void *));

	switch (pMsg->MsgId)
	{
	case WM_CREATE:
		hSlideWin = WM_CreateWindowAsChild(0, 0, WM_GetWindowSizeX(pMsg->hWin), WM_GetWindowSizeY(pMsg->hWin), pMsg->hWin, WM_CF_SHOW|WM_CF_MEMDEV, SlideWindowProc,0);
		WM_SetUserData(pMsg->hWin, &hSlideWin, sizeof(void *));
		break;
	case WM_DELETE:
		pPresetSlideListObj->DeleteSlideWin();
		break;
	case WM_PAINT:
		GUI_SetBkColor(GUI_BLACK);
		GUI_Clear();
		break;
	default:
		WM_DefaultProc(pMsg);
	}
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
		//PresetSlideCreateItems(pMsg->hWin);
		break;
	case WM_DELETE:
		//PresetSlideDeleteItems();
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
				if(!WM_HasCaptured(pMsg->hWin))
				{
					WM_SetCapture(pMsg->hWin,0);
					PID_LastState = *pPID_State;
				}
				else
				{
					y = WM_GetWindowOrgY(pMsg->hWin);
					x = WM_GetWindowSizeY(pMsg->hWin);
					dy = pPID_State->y-PID_LastState.y;
					if(dy > 0)
					{
						if (y + dy > 20)
							dy = 20 - y;
						WM_MoveWindow(pMsg->hWin,0, dy);
					}
					else if(dy < 0)
					{
						WM_MoveWindow(pMsg->hWin,0,dy<GUI_GetScreenSizeY()-INDICATORFRAME-100-y-x?GUI_GetScreenSizeY()-100-INDICATORFRAME-y-x:dy);
					}
					//WM_MoveWindow(pMsg->hWin,0,pPID_State->y-PID_LastState.y);
				}
			}
			else
			{
				WM_ReleaseCapture();
				y = WM_GetWindowOrgY(pMsg->hWin);
				x = WM_GetWindowSizeY(pMsg->hWin);
				if(bmEMPTYTITLEBAR.YSize+INDICATORFRAME < y)
				{
					WM_MoveWindow(pMsg->hWin,0,bmEMPTYTITLEBAR.YSize+INDICATORFRAME-y);
				}
				else if(GUI_GetScreenSizeY()-INDICATORFRAME-100 > y+x)
				{
					WM_MoveWindow(pMsg->hWin,0,GUI_GetScreenSizeY()-100-INDICATORFRAME-y-x);
				}
				// SlidingBorder = GetSlidingBordercheck(pMsg->hWin,hPresets);
			}
		}
		else
		{
			WM_ReleaseCapture();
		}
		break;
	default:
		WM_DefaultProc(pMsg);
	}
}


bool PresetSlideList::CreateWidget(int x0, int y0, int width, int height, WM_HWIN hParent, U32 Style)
{
	singleColumn = ((Style & SLIDE_PANEL_SINGLE_COLUMN) != 0);

	hWinDelimiter = WM_CreateWindowAsChild(x0,y0,width,height,hParent, (U16)Style, DelimiterWindowProc, sizeof(void *));
	WM_GetUserData(hWinDelimiter, &hWinSlide, sizeof(void *));
	PresetSlideList *pObj = this;
	WM_SetUserData(hWinDelimiter, &pObj, sizeof(void *));

	return (hWinDelimiter != 0);
}

void PresetSlideList::DeleteWidget()
{
	if(hWinDelimiter)
	{
		DeletePresetSlidePanel(hWinDelimiter);
		hWinDelimiter = 0;
	}
}

void PresetSlideList::DeleteSlideWin()
{
	if(hWinSlide)
	{
		PresetSlideDeleteItems();
		WM_DeleteWindow(hWinSlide);
		hWinSlide = 0;
	}
}

bool PresetSlideList::CreateSlideItem()
{
	if(hWinSlide)
	{
		PresetSlideDeleteItems();
		PresetSlideCreateItems(hWinSlide, singleColumn);
	}
	return true;
}

bool PresetSlideList::IsPresetItem(WM_HWIN hFocus)
{
	return (WM_GetParent(hFocus) == hWinSlide);
}

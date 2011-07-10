
#include <string.h>             /* for memset */
#include "GUI.h"
#include "WM.h"
#include "BUTTON.h"
#include "FRAMEWIN.h"
#include "TEXT.h"
#include "DIALOG.h"
#include "DIALOG_Intern.h"

#include "MisaDialogueBox.h"

#include "Resource.h"

/*********************************************************************
*
*       Defaults
*
**********************************************************************
*/

#ifndef MISACONFIRMBOX_BORDER
  #define MISACONFIRMBOX_BORDER 4
#endif

#ifndef MISACONFIRMBOX_XSIZEFRAME
  #define MISACONFIRMBOX_XSIZEFRAME 800
#endif

#ifndef MISACONFIRMBOX_YSIZEFRAME
  #define MISACONFIRMBOX_YSIZEFRAME 180
#endif

#ifndef MISACONFIRMBOX_XSIZEBUTTON
  #define MISACONFIRMBOX_XSIZEBUTTON 396
#endif

#ifndef MISACONFIRMBOX_YSIZEBUTTON
  #define MISACONFIRMBOX_YSIZEBUTTON 99
#endif

#ifndef MISACONFIRMBOX_BKCOLOR
  #define MISACONFIRMBOX_BKCOLOR GUI_BLACK
#endif

#define ID_FRAME 100

/*********************************************************************
*
*       Exported routines
*
**********************************************************************
*/

void KitaraStep();

WM_HWIN DLWIN_CreateIndirect(const GUI_WIDGET_CREATE_INFO* pCreateInfo, WM_HWIN hWinParent,int x0, int y0, WM_CALLBACK* cb);
WM_HWIN DLBUTTON_CreateIndirect(const GUI_WIDGET_CREATE_INFO* pCreateInfo, WM_HWIN hWinParent,int x0, int y0, WM_CALLBACK* cb);
WM_HWIN DLTEXT_CreateIndirect(const GUI_WIDGET_CREATE_INFO* pCreateInfo, WM_HWIN hWinParent,int x0, int y0, WM_CALLBACK* cb);

WM_HWIN DLWIN_CreateIndirect(const GUI_WIDGET_CREATE_INFO* pCreateInfo, WM_HWIN hWinParent,int x0, int y0, WM_CALLBACK* cb)
{
	FRAMEWIN_Handle hObj;
	hObj = FRAMEWIN_CreateEx(pCreateInfo->x0 + x0, pCreateInfo->y0 + y0, pCreateInfo->xSize, pCreateInfo->ySize,
		hWinParent, 0, pCreateInfo->Flags, pCreateInfo->Id, pCreateInfo->pName, cb);
	FRAMEWIN_SetBorderSize(hObj,0);
	FRAMEWIN_SetTitleVis(hObj,0);
	return hObj;
}

WM_HWIN DLBUTTON_CreateIndirect(const GUI_WIDGET_CREATE_INFO* pCreateInfo, WM_HWIN hWinParent,int x0, int y0, WM_CALLBACK* cb)
{
	BUTTON_Handle  hThis;
	GUI_USE_PARA(cb);
	hThis = BUTTON_CreateEx(pCreateInfo->x0 + x0, pCreateInfo->y0 + y0,
		pCreateInfo->xSize, pCreateInfo->ySize,
		hWinParent, WM_CF_SHOW|WM_CF_MEMDEV, pCreateInfo->Flags, pCreateInfo->Id);
	//BUTTON_SetText(hThis, pCreateInfo->pName);
	BUTTON_SetFocussable(hThis, 0);
	switch(pCreateInfo->Id)
	{		
	case GUI_ID_OK:
		BUTTON_SetBitmap(hThis,BUTTON_CI_UNPRESSED,&bmMC_OK_UN);
		BUTTON_SetBitmap(hThis,BUTTON_CI_PRESSED,&bmMC_OK_SE);
		break;
	case GUI_ID_CANCEL:
		BUTTON_SetBitmap(hThis,BUTTON_CI_UNPRESSED,&bmCM_CANCEL_UN);
		BUTTON_SetBitmap(hThis,BUTTON_CI_PRESSED,&bmCM_CANCEL_SE);
		break;
	}
	return hThis;
}

WM_HWIN DLTEXT_CreateIndirect(const GUI_WIDGET_CREATE_INFO* pCreateInfo, WM_HWIN hWinParent,int x0, int y0, WM_CALLBACK* cb)
{
	TEXT_Handle  hThis;
	GUI_USE_PARA(cb);
	hThis = TEXT_CreateEx(pCreateInfo->x0 + x0, pCreateInfo->y0 + y0, pCreateInfo->xSize, pCreateInfo->ySize,
		hWinParent, WM_CF_SHOW|WM_CF_MEMDEV, pCreateInfo->Flags, pCreateInfo->Id, pCreateInfo->pName);
	TEXT_SetFont(hThis, &GUI_Font32B_ASCII);
	TEXT_SetTextColor(hThis, GUI_WHITE);
	return hThis;
}

int Misa_ExecCreatedDialog (WM_HWIN hDialog)
{
	WM_DIALOG_STATUS DialogStatus = {0};
	/* Let window know how to send feedback (close info & return value) */
	GUI_SetDialogStatusPtr(hDialog, &DialogStatus);
	while (!DialogStatus.Done)
	{
		KitaraStep();
#if 0
		if (!GUI_Exec())
		{
			GUI_WaitEvent();      /* Wait for event (keyboard, mouse or whatever) */
		}
#endif
	}
	return DialogStatus.ReturnValue;
}

/*********************************************************************
*
*       ConfirmBox_Callback
*/
void ConfirmBox_Callback(WM_MESSAGE * pMsg)
{
	WM_HWIN hWin = pMsg->hWin;
	switch (pMsg->MsgId)
	{
	case WM_INIT_DIALOG:
		FRAMEWIN_SetClientColor(hWin, MISACONFIRMBOX_BKCOLOR);
		break;
	case WM_KEY:
	{
		int Key = ((const WM_KEY_INFO*)(pMsg->Data.p))->Key;
		if (((const WM_KEY_INFO*)(pMsg->Data.p))->PressedCnt)
		{
			switch (Key)
			{
			case GUI_KEY_ESCAPE:
				GUI_EndDialog(hWin, 1);             /* End dialog with return value 1 if <ESC> is pressed */
				break;
			case GUI_KEY_ENTER:
				GUI_EndDialog(hWin, 0);             /* End dialog with return value 0 if <ENTER> is pressed */
				break;
			}
		}
	}
		break;
	case WM_NOTIFY_PARENT:
	{
		int NCode = pMsg->Data.v;             /* Get notification code */
		int Id    = WM_GetId(pMsg->hWinSrc);  /* Get control ID */
		switch (NCode)
		{
		case WM_NOTIFICATION_RELEASED:      /* React only if released */
			if(Id == GUI_ID_OK || Id == GUI_ID_CANCEL)
			{
				GUI_EndDialog(hWin, Id);         /* End dialog with return value 0 if OK */
			}
			break;
		}
	}
		break;
	default:
		WM_DefaultProc(pMsg);
	}
}

/*********************************************************************
*
*       ConfirmBox_Create
*/
WM_HWIN ConfirmBox_Create(const char * sMessage, const char * sCaption, int Flags)
{
	GUI_WIDGET_CREATE_INFO _aDialogCreate[4];                                     /* 0: FrameWin, 1: Text, 2: Button */
	int BorderSize = FRAMEWIN_GetDefaultBorderSize();                             /* Default border size of frame window */
	int xSizeFrame = MISACONFIRMBOX_XSIZEFRAME; /* XSize of frame window */
	int ySizeFrame = MISACONFIRMBOX_YSIZEFRAME;                                                               /* YSize of frame window */
	int x0, y0;                                                                   /* Position of frame window */
	int xSizeMessage;                                                             /* Length in pixels of message */
	int xSizeCaption;                                                             /* Length in pixels of caption */
	int ySizeCaption;                                                             /* YSize of caption */
	int ySizeMessage;                                                             /* YSize of message */
	WM_HWIN hBox;
	GUI_RECT Rect;
	const GUI_FONT GUI_UNI_PTR * pOldFont;
	/* Zeroinit variables */
	memset(_aDialogCreate, 0, sizeof(_aDialogCreate));
	/* Get dimension of message */
	pOldFont = GUI_SetFont(&GUI_Font32B_ASCII);//TEXT_GetDefaultFont()
	GUI_GetTextExtend(&Rect, sMessage, 255);
	xSizeMessage = Rect.x1 - Rect.x0 + MISACONFIRMBOX_BORDER * 2;
	ySizeMessage = Rect.y1 - Rect.y0 + 1;
	ySizeCaption = GUI_GetYSizeOfFont(FRAMEWIN_GetDefaultFont());
	//ySizeFrame = ySizeMessage +            /* size of message */
	//            MISACONFIRMBOX_XSIZEBUTTON +      /* size of button */
	//             ySizeCaption +            /* caption size */
	//             MISACONFIRMBOX_BORDER * 3 +   /* inner border - text, text - button, button - bottom */
	//             BorderSize * 2 +          /* top & bottom border */
	//             1;                        /* inner border */
	/* Get xsize of caption */
	xSizeCaption = GUI_GetStringDistX(sCaption);
	/* Check maximum */
	if (xSizeFrame > LCD_GET_XSIZE())
	{
		xSizeFrame = LCD_GET_XSIZE();
	}
	if (ySizeFrame > LCD_GET_YSIZE())
	{
		ySizeFrame = LCD_GET_YSIZE();
	}
	/* Calculate position of framewin */
	x0 = GUI_OrgX + (LCD_GET_XSIZE() - xSizeFrame) / 2;
	y0 = 420;//GUI_OrgY + (LCD_GET_YSIZE() - ySizeFrame) / 2;
	/* restore modified Context */
	GUI_SetFont(pOldFont);
	/* Fill frame win resource */
	_aDialogCreate[0].pfCreateIndirect = DLWIN_CreateIndirect;
	_aDialogCreate[0].pName            = sCaption;
	_aDialogCreate[0].x0               = x0;
	_aDialogCreate[0].y0               = y0;
	_aDialogCreate[0].xSize            = xSizeFrame;
	_aDialogCreate[0].ySize            = ySizeFrame;
	if (Flags & GUI_MESSAGEBOX_CF_MOVEABLE)
	{
		_aDialogCreate[0].Flags          = FRAMEWIN_CF_MOVEABLE;
	}
	/* Fill text resource */
	_aDialogCreate[1].pfCreateIndirect = DLTEXT_CreateIndirect;
	_aDialogCreate[1].pName            = sMessage;
	_aDialogCreate[1].x0               = (xSizeFrame - xSizeMessage - BorderSize * 2) / 2;
	_aDialogCreate[1].y0               = 20;
	_aDialogCreate[1].xSize            = xSizeMessage;
	_aDialogCreate[1].ySize            = ySizeMessage;
	_aDialogCreate[1].Para             = GUI_TA_TOP | GUI_TA_HCENTER;
	/* Fill button resource */
	_aDialogCreate[2].pfCreateIndirect = DLBUTTON_CreateIndirect;
	_aDialogCreate[2].pName            = "OK";
	_aDialogCreate[2].Id               = GUI_ID_OK;
	_aDialogCreate[2].x0               = 2;
	_aDialogCreate[2].y0               = 81;
	_aDialogCreate[2].xSize            = MISACONFIRMBOX_XSIZEBUTTON;
	_aDialogCreate[2].ySize            = MISACONFIRMBOX_YSIZEBUTTON-3;
	_aDialogCreate[3].pfCreateIndirect = DLBUTTON_CreateIndirect;
	_aDialogCreate[3].pName            = "CANCEL";
	_aDialogCreate[3].Id               = GUI_ID_CANCEL;
	_aDialogCreate[3].x0               = 402;
	_aDialogCreate[3].y0               = 80;
	_aDialogCreate[3].xSize            = MISACONFIRMBOX_XSIZEBUTTON;
	_aDialogCreate[3].ySize            = MISACONFIRMBOX_YSIZEBUTTON;
	/* Create dialog */
	hBox = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), ConfirmBox_Callback, 0, 0, 0);
	if (Flags & GUI_MESSAGEBOX_CF_MODAL)
	{
		WM_MakeModal(hBox);
	}
	return hBox;
}

/*********************************************************************
*
*       GUI_MessageBox
*/
int Misa_ConfirmBox(const char * sMessage, const char * sCaption, int Flags)
{
	WM_HWIN hWin;
	hWin = ConfirmBox_Create(sMessage, sCaption, Flags);
	/* Exec dialog */
	return Misa_ExecCreatedDialog(hWin);
}

void WarningBox_Callback(WM_MESSAGE * pMsg)
{
	//GUI_PID_STATE* pPID_State;
	WM_HWIN hWin = pMsg->hWin;
	switch (pMsg->MsgId)
	{
	case WM_INIT_DIALOG:
		FRAMEWIN_SetClientColor(hWin, MISACONFIRMBOX_BKCOLOR);
		FRAMEWIN_SetBorderSize(hWin,1);
		break;
#if 0
	case WM_TOUCH:
		if(pMsg->Data.p)
		{
			pPID_State = (GUI_PID_STATE*)pMsg->Data.p;
			if(pPID_State->Pressed)
			{
			}
			else
			{
				GUI_EndDialog(hWin, 0);
			}
		}
		break;
	case WM_KEY:
	{
		int Key = ((const WM_KEY_INFO*)(pMsg->Data.p))->Key;
		if (((const WM_KEY_INFO*)(pMsg->Data.p))->PressedCnt)
		{
			switch (Key)
			{
			case GUI_KEY_ESCAPE:
				GUI_EndDialog(hWin, 1);             /* End dialog with return value 1 if <ESC> is pressed */
				break;
			case GUI_KEY_ENTER:
				GUI_EndDialog(hWin, 0);             /* End dialog with return value 0 if <ENTER> is pressed */
				break;
			}
		}
	}
		break;
#endif
	case WM_NOTIFY_PARENT:
	{
		int NCode = pMsg->Data.v;             /* Get notification code */
		int Id    = WM_GetId(pMsg->hWinSrc);  /* Get control ID */
		switch (NCode)
		{
		case WM_NOTIFICATION_RELEASED:      /* React only if released */
			if(Id == GUI_ID_OK || Id == GUI_ID_CANCEL)
			{
				GUI_EndDialog(hWin, Id);         /* End dialog with return value 0 if OK */
			}
			break;
		}
	}
	break;
	default:
		WM_DefaultProc(pMsg);
	}
}

WM_HWIN WarningBox_Create(const char * sMessage, const char * sCaption, int Flags)
{
	GUI_WIDGET_CREATE_INFO _aDialogCreate[4];                                     /* 0: FrameWin, 1: Text, 2: Button */
	int BorderSize = FRAMEWIN_GetDefaultBorderSize();                             /* Default border size of frame window */
	int xSizeFrame = 400; /* XSize of frame window */
	int ySizeFrame = 300;                                                               /* YSize of frame window */
	int x0, y0;                                                                   /* Position of frame window */
	int xSizeMessage;                                                             /* Length in pixels of message */
	int xSizeCaption;                                                             /* Length in pixels of caption */
	int ySizeCaption;                                                             /* YSize of caption */
	int ySizeMessage;                                                             /* YSize of message */
	WM_HWIN hBox;
	GUI_RECT Rect;
	const GUI_FONT GUI_UNI_PTR * pOldFont;
	/* Zeroinit variables */
	memset(_aDialogCreate, 0, sizeof(_aDialogCreate));
	/* Get dimension of message */
	pOldFont = GUI_SetFont(&GUI_Font32B_ASCII);//TEXT_GetDefaultFont()
	GUI_GetTextExtend(&Rect, sMessage, 255);
	xSizeMessage = 400;
	ySizeMessage = Rect.y1 - Rect.y0 + 1;
	ySizeCaption = GUI_GetYSizeOfFont(FRAMEWIN_GetDefaultFont());
	//ySizeFrame = ySizeMessage +            /* size of message */
	//            MISACONFIRMBOX_XSIZEBUTTON +      /* size of button */
	//             ySizeCaption +            /* caption size */
	//             MISACONFIRMBOX_BORDER * 3 +   /* inner border - text, text - button, button - bottom */
	//             BorderSize * 2 +          /* top & bottom border */
	//             1;                        /* inner border */
	/* Get xsize of caption */
	xSizeCaption = GUI_GetStringDistX(sCaption);
	/* Check maximum */
	if (xSizeFrame > LCD_GET_XSIZE())
	{
		xSizeFrame = LCD_GET_XSIZE();
	}
	if (ySizeFrame > LCD_GET_YSIZE())
	{
		ySizeFrame = LCD_GET_YSIZE();
	}
	/* Calculate position of framewin */
	x0 = GUI_OrgX + (LCD_GET_XSIZE() - xSizeFrame) / 2;
	y0 = GUI_OrgY + (LCD_GET_YSIZE() - ySizeFrame) / 2;
	/* restore modified Context */
	GUI_SetFont(pOldFont);
	/* Fill frame win resource */
	_aDialogCreate[0].pfCreateIndirect = DLWIN_CreateIndirect;
	_aDialogCreate[0].pName            = sCaption;
	_aDialogCreate[0].x0               = x0;
	_aDialogCreate[0].y0               = y0;
	_aDialogCreate[0].xSize            = xSizeFrame;
	_aDialogCreate[0].ySize            = ySizeFrame;
	if (Flags & GUI_MESSAGEBOX_CF_MOVEABLE)
	{
		_aDialogCreate[0].Flags          = FRAMEWIN_CF_MOVEABLE;
	}
	/* Fill text resource */
	_aDialogCreate[1].pfCreateIndirect = DLTEXT_CreateIndirect;
	_aDialogCreate[1].pName            = sMessage;
	_aDialogCreate[1].x0               = 0;//(xSizeFrame - xSizeMessage - BorderSize * 2) / 2;
	_aDialogCreate[1].y0               = MISACONFIRMBOX_BORDER;
	_aDialogCreate[1].xSize            = xSizeMessage;
	_aDialogCreate[1].ySize            = ySizeMessage;
	_aDialogCreate[1].Flags             = GUI_TA_HCENTER|GUI_TA_VCENTER;
	_aDialogCreate[2].pfCreateIndirect = DLTEXT_CreateIndirect;
	_aDialogCreate[2].pName            = sCaption;
	_aDialogCreate[2].x0               = 0;//(xSizeFrame - xSizeMessage - BorderSize * 2) / 2;
	_aDialogCreate[2].y0               = MISACONFIRMBOX_BORDER+36;
	_aDialogCreate[2].xSize            = xSizeMessage;
	_aDialogCreate[2].ySize            = ySizeMessage;
	_aDialogCreate[2].Flags             = GUI_TA_HCENTER|GUI_TA_VCENTER;
	/* Fill button resource */
	_aDialogCreate[3].pfCreateIndirect = DLBUTTON_CreateIndirect;
	_aDialogCreate[3].pName            = "OK";
	_aDialogCreate[3].Id               = GUI_ID_OK;
	_aDialogCreate[3].x0               = 1;
	_aDialogCreate[3].y0               = 300-MISACONFIRMBOX_YSIZEBUTTON;
	_aDialogCreate[3].xSize            = MISACONFIRMBOX_XSIZEBUTTON;
	_aDialogCreate[3].ySize            = MISACONFIRMBOX_YSIZEBUTTON-3;
	/* Create dialog */
	hBox = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), WarningBox_Callback, 0, 0, 0);
	if (Flags & GUI_MESSAGEBOX_CF_MODAL)
	{
		WM_MakeModal(hBox);
	}
	return hBox;
}

int Misa_WarningBox(const char * sMessage, const char * sCaption, int Flags)
{
	WM_HWIN hWin;
	hWin = WarningBox_Create(sMessage, sCaption, Flags);
	/* Exec dialog */
	return Misa_ExecCreatedDialog(hWin);
}

WM_HWIN hwarningMessage;

static void Misa_WarningMessageCB(WM_MESSAGE* pMsg)
{
	//int x, y;
	//int NCode,Id;
	GUI_RECT rect;
	switch (pMsg->MsgId)
	{
	case WM_CREATE:
		break;
	case WM_DELETE:
		break;
	case WM_PAINT:
		rect.x0 = 0;
		rect.y0 = 0;
		rect.x1 = 400;
		rect.y1 = 50;
		GUI_SetBkColor(GUI_RED);
		GUI_Clear();
		GUI_SetFont(&GUI_Font32B_ASCII);
		GUI_DispStringInRect("Updating!", &rect, GUI_TA_HCENTER|GUI_TA_VCENTER);
		rect.x0 = 0;
		rect.y0 = 50;
		rect.x1 = 400;
		rect.y1 = 100;
		GUI_DispStringInRect("Don't turn off the power!", &rect, GUI_TA_HCENTER|GUI_TA_VCENTER);
		break;
#if 0
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
			case SYSTEM_ID_UPDATESOUNDBANK:
				break;
			default:
				;
			}
			break;
		}
		break;
	case WM_TOUCH:
		break;
#endif
	default:
		WM_DefaultProc(pMsg);
	}
}


int Misa_WarningMessageBegin()
{
	hwarningMessage = WM_CreateWindow(200,150,400,300,WM_CF_SHOW|WM_CF_MEMDEV,Misa_WarningMessageCB,0);
	return 0;
}

int Misa_WarningMessageEnd()
{
	WM_DeleteWindow(hwarningMessage);
	return 0;
}


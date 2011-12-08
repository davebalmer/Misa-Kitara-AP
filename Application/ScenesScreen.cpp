#include <cstdlib>
#include <iostream>

#include "GUI.h"
#include "WM.h"
#include "BUTTON.h"

#include "MisaDialogueBox.h"
#include "MisaAppInc.h"
#include "MisamenuConfig.h"

#include "Resource.h"

///////////////////////////////////////////////////////////////////////////////
// layout
//*****************************************************************************
//
//	1	title:798*60
//	3
//	4	Button6:73*71		SceneName6:459*71	Save:130*71		Delete:130*71	4
//	1
//  4	Button5:73*71		SceneName5:459*71
//	1
//  4	Button4:73*71		SceneName4:459*71
//	1
//  4	Button3:73*71		SceneName3:459*71
//	1
//  4	Button2:73*71		SceneName2:459*71
//	1
//  4	Button1:73*71		SceneName1:459*71
//	2  (adjsuted to 6 to align with close button in com components)
//	4	KitaraNeck:396*99	Back:396*99
//	5

#define SCENE_SAVE__XOFFSET (4 + 73 + 459)
#define SCENE_NAMETXT__XOFFSET (4 + 73 + 30)
#define SCENE_NAMETXT__YOFFSET	 23

typedef enum __SCENESITEMS
{
	SCENES_SAVE = 0,
	SCENES_DELETE = SCENES_NUMBER,
	SCENES_MAX = 2 * SCENES_NUMBER
} SCENESITEMS;

static void ScenesProc(WM_MESSAGE* pMsg);


static COMMCOMPONENT hCOMPcomm;
static WM_HWIN hScenes;
static WM_HWIN hScenesItems[SCENES_MAX];

U8 CreateScenesScreen(void* pPara)
{
	int x,y;
	x=GUI_GetScreenSizeX();
	y=GUI_GetScreenSizeY();
	hScenes = WM_CreateWindow(0, 0, x, y, /*WM_CF_SHOW | */WM_CF_MEMDEV, ScenesProc, 0);
	return 0;
}

U8 DeleteScenesScreen()
{
	WM_DeleteWindow(hScenes);
	return 0;
}


U8 TopScenesScreen(WM_HWIN hPreWin)
{
	WM_HideWindow(hPreWin);
	WM_ShowWindow(hScenes);
	return 0;
}


static U8 ScenesCreateItems(WM_HWIN hParent)
{
	int x, y, i;
	hCOMPcomm = CreateCommComponentEx(hParent,COMMON_ITEM_CLOSE);
	memset(hScenesItems,0,sizeof(hScenesItems));

	y = bmEMPTYTITLEBAR.YSize + 1;		// = 63
	for (i = 0; i < SCENES_NUMBER; i++)
	{
		x = SCENE_SAVE__XOFFSET;
		hScenesItems[SCENES_SAVE + i] = BUTTON_CreateAsChild(x, y, bmSCENESAVE_UN.XSize, bmSCENESAVE_UN.YSize, hParent, GUI_ID_SCENE_BASE + SCENES_SAVE + i, WM_CF_SHOW|WM_CF_MEMDEV);
		BUTTON_SetFocussable(hScenesItems[SCENES_SAVE + i], 0);
		BUTTON_SetBitmap(hScenesItems[SCENES_SAVE + i], BUTTON_CI_UNPRESSED,&bmSCENESAVE_UN);
		BUTTON_SetBitmap(hScenesItems[SCENES_SAVE + i], BUTTON_CI_PRESSED,&bmSCENESAVE_SE);
		x+= bmSCENESAVE_UN.XSize;

		hScenesItems[SCENES_DELETE + i] = BUTTON_CreateAsChild(x, y, bmSCENEDELETE_UN.XSize, bmSCENEDELETE_UN.YSize, hParent, GUI_ID_SCENE_BASE + SCENES_DELETE + i, WM_CF_SHOW|WM_CF_MEMDEV);
		BUTTON_SetFocussable(hScenesItems[SCENES_DELETE + i], 0);
		BUTTON_SetBitmap(hScenesItems[SCENES_DELETE + i], BUTTON_CI_UNPRESSED,&bmSCENEDELETE_UN);
		BUTTON_SetBitmap(hScenesItems[SCENES_DELETE + i], BUTTON_CI_PRESSED,&bmSCENEDELETE_SE);

		y += bmSCENEDELETE_UN.YSize + 1;
	}

	return 0;
}


static U8 ScenesDeleteItems()
{
	DeleteCommComponent(hCOMPcomm);

	for (int i = 0; i < SCENES_MAX; i++)
		BUTTON_Delete(hScenesItems[i]);

	return 0;
}



static void ScenesProc(WM_MESSAGE* pMsg)
{
	int x, y, i;
	int NCode, Id, rId;

	ProcessCommComponent(pMsg, &hCOMPcomm);
	switch (pMsg->MsgId)
	{
	case WM_CREATE:
		ScenesCreateItems(pMsg->hWin);
		break;

	case WM_DELETE:
		ScenesDeleteItems();
		break;

	case WM_PAINT:
		GUI_SetBkColor(GUI_BLACK);
		GUI_Clear();
		GUI_SetTextMode(GUI_TEXTMODE_TRANS);
		GUI_DrawBitmap(&bmEMPTYTITLEBAR, 0, 0);
		GUI_SetFont(&GUI_Font32B_ASCII);
		y = bmEMPTYTITLEBAR.YSize;
		x = WM_GetWindowSizeX(pMsg->hWin);
		GUI_DispStringHCenterAt("Scenes", x / 2, 5);

		y = bmEMPTYTITLEBAR.YSize + 1;		// = 63
		for (i = 0; i < SCENES_NUMBER; i++)
		{
			x = 4;
			GUI_DrawBitmap(&bmSCENESQUARE[SCENES_NUMBER - 1 - i], x, y);
			x += bmSCENESQUARE[i].XSize;
			GUI_DrawBitmap(&bmSCENENAME, x, y);
			GUI_DispStringAt(Scene_GetName(i).c_str(), SCENE_NAMETXT__XOFFSET, y + SCENE_NAMETXT__YOFFSET);

			y += bmSCENESQUARE[i].YSize + 1;
		}
		GUI_DrawBitmap(&bmSCENEINFO, 4, y + 5);
		break;

	case WM_NOTIFY_PARENT:
		if(DispatchCommComponent(pMsg,&hCOMPcomm))
		{
			Id = WM_GetId(pMsg->hWinSrc);
			NCode = pMsg->Data.v;
			switch(NCode)
			{
			case WM_NOTIFICATION_CLICKED:
				DEBUG_STDOUT("Scenes button clicked!");
				break;
			case WM_NOTIFICATION_RELEASED:
				{
					bool lUpdateName = false;
					rId = Id;
					rId -= GUI_ID_SCENE_BASE;
					if (rId < SCENES_DELETE)
					{
						// save the scene
						if (Scene_GetName(rId) != "")
						{
							// ask for confirmation
							if(Misa_ConfirmBox("Are you sure?","Overwrite this scene!", GUI_MESSAGEBOX_CF_MODAL) != GUI_ID_OK)
								break;
						}
						SaveScene(rId, GetCurrentPresetName(), SynthGetMastervolume(), SynthGetBallmode(), SynthGetStringmode(),
							MisaGetBallTravel(), MisaGetEnableSustain(), MisaGetRingingNotes(), SynthGetTapmode());
						lUpdateName = true;	
					}
					else if (rId < SCENES_MAX)
					{
						// delete the scene
						rId -= SCENES_DELETE;
						if (Scene_GetName(rId) != "")
						{
							if(Misa_ConfirmBox("Are you sure?", "Delete this scene!", GUI_MESSAGEBOX_CF_MODAL) == GUI_ID_OK)
							{
								InitScene(rId);
								lUpdateName = true;	
							}
						}
					}

					if (lUpdateName)
					{
						GUI_RECT lrect;
						lrect.x0 = 4 + bmSCENESQUARE[0].XSize;
						lrect.y0 = bmEMPTYTITLEBAR.YSize + 1 + rId * (bmSCENESQUARE[0].YSize + 1);
						lrect.x1 = lrect.x0 + bmSCENENAME.XSize;
						lrect.y1 = lrect.y0 + bmSCENENAME.YSize;
						WM_InvalidateRect(pMsg->hWin, &lrect);
					}

					DEBUG_STDOUT("Scenes button released!");
					break;
				}

			default:
				;
			}
		}
		break;
			
	default:
		WM_DefaultProc(pMsg);
	}
}

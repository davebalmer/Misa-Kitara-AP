#include "QuickScreen.h"
#include "MisaWidget.h"
#include "MainMenu.h"
#include "Resource.h"



///////////////////////////////////////////////////////////////////////////////
//
static QuickScreen *pQuickScreen = NULL;

// extern function
void TerminateApplication();

///////////////////////////////////////////////////////////////////////////////
//




U8 CreateMisaquicksetScreen(void* pPara)
{
	if (pQuickScreen == NULL)
		pQuickScreen = new QuickScreen;

	return 0;
}

U8 DeleteMisaquicksetScreen()
{
	delete pQuickScreen;
	pQuickScreen = NULL;

	return 0;
}


U8 BeginMisaquicksetScreen()
{
	pQuickScreen->Show();
	return 0;
}

U8 EndMisaquicksetScreen()
{
	pQuickScreen->Hide();
	return 0;
}


void MisaquicksetProc(WM_MESSAGE* pMsg)
{
	int Vol;

	int NCode,Id;
	GUI_PID_STATE* pPID_State;
	switch (pMsg->MsgId)
	{
	case WM_CREATE:
		break;

	case WM_DELETE:
		break;

	case WM_PAINT:
		GUI_SetBkColor(GUI_BLACK);
		GUI_Clear();
		GUI_DrawBitmap(&bmQS_PRESET_BKG, 250, 0);
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
//				TerminateApplication();
//				pQuickScreen->Hide();
				break;

			case MISAQUICKSET_ID_LOAD:
#ifdef PRESET_IN_LIST
				{
					string presetName;
					if (pQuickScreen->GetSelectedPreset(presetName))
					{
						SynthLoadPreset(presetName);
						UpdateSynthSettingEx(MISAMODULE_ALL);
						Misa_Delay(200);
#if !defined(Linux) && defined(MISA_APPLICATION)
						pQuickScreen->Hide();
						TerminateApplication();
#endif
					}
				}
#else // PRESET_IN_SLIDEWIN
				pQuickScreen->LoadPreset();
#endif  // PRESET_IN_SLIDEWIN

				break;

			case MISAQUICKSET_ID_BALL:
				SynthSetBallmode(MisaCheckbox_GetStatus(pMsg->hWinSrc));
				RunFramework();
//				TerminateApplication();
//				pQuickScreen->Hide();				
				break;

			case MISAQUICKSET_ID_STRINGS:
				SynthSetStringmode(MisaCheckbox_GetStatus(pMsg->hWinSrc));
				RunFramework();
//				TerminateApplication();
//				pQuickScreen->Hide();
				break;

			case COMMON_ID_CLOSE:
				RunFramework();
				TerminateApplication();
				pQuickScreen->Hide();
				break;
			case MISAQUICKSET_ID_ADVANCED:
				pQuickScreen->Hide();
				TopMainMenu(0);
				break;
			}
			break;

		//case WM_NOTIFICATION_SEL_CHANGED:
		//	break;

		case WM_NOTIFICATION_VALUE_CHANGED:
			if (Id == MISAQUICKSET_ID_MASTERVOLUME)
			{
				Vol = MisaProgressbar_GetPercent(pMsg->hWinSrc);
				SynthSetMastervolume(Vol < 127 ? Vol : 127);
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
			}
			else
			{
//				TerminateApplication();
			}
		}
		break;
	default:
		WM_DefaultProc(pMsg);
	}
}


#include "QuickScreen.h"
#include "PresetsScreen.h"

#include "GUI.h"
#include "WM.h"
#include "LISTBOX.h"

#include "MisamenuConfig.h"
#include "MisaWidget.h"


#ifdef Linux
#include <dirent.h>
#endif

#ifdef WIN32
#include <Windows.h>
#endif

#include <vector>

#include "Resource.h"


///////////////////////////////////////////////////////////////////////////////
// extern var
extern std::string working_directory;

// extern function
void MisaquicksetProc(WM_MESSAGE* pMsg);

///////////////////////////////////////////////////////////////////////////////


// QuickScreen
QuickScreen::QuickScreen()
{
	int x,y;
	x=GUI_GetScreenSizeX();
	y=GUI_GetScreenSizeY();
	hQuickWin = WM_CreateWindow(0, 0, x, y, WM_CF_MEMDEV, MisaquicksetProc, 0);
	CreateItems();
}


QuickScreen::~QuickScreen()
{
	DeleteItems();
	WM_DeleteWindow(hQuickWin);
}




U8 QuickScreen::CreateItems()
{

	int x1 = 2, x2 = 250, x3 = 523;
	int y = 0;

	hWinMasterVolume = MisaProgressbar_CreateEx(x1, y, bmQS_VOLUME_BKG.XSize, bmQS_VOLUME_BKG.YSize, hQuickWin,MISAQUICKSET_ID_MASTERVOLUME, NULL,"",MISAPROG_FONT32, 0);

#ifdef PRESET_IN_LIST
	hPresetList =  LISTBOX_CreateAsChild (NULL, hQuickWin, 400, 150, 250, 250, WM_CF_SHOW | WM_CF_MEMDEV);
	LISTBOX_SetAutoScrollV(hPresetList, 1);
	LISTBOX_SetFont(hPresetList, &GUI_Font32B_ASCII);

	// List colors
	LISTBOX_SetBkColor(hPresetList, LISTBOX_CI_UNSEL, 0);
	LISTBOX_SetTextColor(hPresetList, LISTBOX_CI_UNSEL, 0xffffff);
	LISTBOX_SetBkColor(hPresetList, LISTBOX_CI_SEL, 0);
	LISTBOX_SetBkColor(hPresetList, LISTBOX_CI_SELFOCUS, 0);
	LISTBOX_SetTextColor(hPresetList, LISTBOX_CI_SEL, 0x909090);
	LISTBOX_SetTextColor(hPresetList, LISTBOX_CI_SELFOCUS, 0x909090);

	//LISTBOX_SetScrollbarWidth(hPresetList, 20);
	//LISTBOX_SetScrollbarColor(LISTBOX_Handle hObj, SCROLLBAR_CI_THUMB or SCROLLBAR_CI_SHAFT or SCROLLBAR_CI_ARROW, GUI_COLOR Color);

#else // PRESET_IN_SLIDEWIN
	hPresetList = presetSlideList.CreateWidget(x2 + 16, y + 20, bmQS_PRESET_BKG.XSize - 32, bmQS_PRESET_BKG.YSize - 32, hQuickWin, WM_CF_SHOW | WM_CF_MEMDEV | SLIDE_PANEL_SINGLE_COLUMN);
#endif  // PRESET_IN_SLIDEWIN

	hWinTapMode = MisaCheckbox_Create(x3, y , bmQS_TAP_UN.XSize, bmQS_TAP_UN.YSize, hQuickWin,MISAQUICKSET_ID_TAPMODE,WM_CF_SHOW|WM_CF_MEMDEV, &bmQS_TAP_UN, &bmQS_TAP_SE);

	y += bmQS_TAP_UN.YSize + 1;
	hWinBallMode = MisaCheckbox_Create(x3, y, bmQS_BALL_UN.XSize,bmQS_BALL_UN.YSize, hQuickWin, MISAQUICKSET_ID_BALL,WM_CF_SHOW|WM_CF_MEMDEV, &bmQS_BALL_UN, &bmQS_BALL_SE);

	y += bmQS_BALL_UN.YSize;
	hWinStringMode = MisaCheckbox_Create(x3, y, bmQS_STRING_UN.XSize,bmQS_STRING_UN.YSize, hQuickWin, MISAQUICKSET_ID_STRINGS,WM_CF_SHOW|WM_CF_MEMDEV, &bmQS_STRING_UN, &bmQS_STRING_SE);

	y += bmQS_STRING_UN.YSize + 1;
	hWinAdvanced = BUTTON_CreateAsChild(x3, y, bmQS_ADVANCED_UN.XSize,bmQS_ADVANCED_UN.YSize,hQuickWin, MISAQUICKSET_ID_ADVANCED,WM_CF_SHOW|WM_CF_MEMDEV);
	BUTTON_SetFocussable(hWinAdvanced,0);
	BUTTON_SetBitmap(hWinAdvanced,BUTTON_CI_UNPRESSED,&bmQS_ADVANCED_UN);
	BUTTON_SetBitmap(hWinAdvanced,BUTTON_CI_PRESSED,&bmQS_ADVANCED_SE);

	y += bmQS_ADVANCED_UN.YSize + 2;
	hWinClose = BUTTON_CreateAsChild(x3, y, bmQS_CLOSE_UN.XSize,bmQS_CLOSE_UN.YSize,hQuickWin, COMMON_ID_CLOSE,WM_CF_SHOW | WM_CF_MEMDEV);
	BUTTON_SetFocussable(hWinClose,0);
	BUTTON_SetBitmap(hWinClose,BUTTON_CI_UNPRESSED,&bmQS_CLOSE_UN);
	BUTTON_SetBitmap(hWinClose,BUTTON_CI_PRESSED,&bmQS_CLOSE_SE);

	hWinLoad = BUTTON_CreateAsChild(x2, y, bmQS_LOAD_UN.XSize,bmQS_LOAD_UN.YSize,hQuickWin, MISAQUICKSET_ID_LOAD, WM_CF_SHOW|WM_CF_MEMDEV);
	BUTTON_SetFocussable(hWinLoad,0);
	BUTTON_SetBitmap(hWinLoad,BUTTON_CI_UNPRESSED,&bmQS_LOAD_UN);
	BUTTON_SetBitmap(hWinLoad,BUTTON_CI_PRESSED,&bmQS_LOAD_SE);


	return 0;
}

U8 QuickScreen::DeleteItems()
{

	//DeleteCommComponent(hCOMPcomm);
	if(hWinTapMode)
		MisaCheckbox_Delete(hWinTapMode);

#if 0
	if(hWinStrings])
		MisaCheckbox_Delete(hWinStrings]);
#endif

	if(hWinAdvanced)
		BUTTON_Delete(hWinAdvanced);

	if(hWinMasterVolume)
		MisaProgressbar_Delete(hWinMasterVolume);

	if(hPresetList)
#ifdef PRESET_IN_LIST
		LISTBOX_Delete(hPresetList);
#else // PRESET_IN_SLIDEWIN
		presetSlideList.DeleteWidget();
#endif  // PRESET_IN_SLIDEWIN

	return 0;
}




void QuickScreen::Show()
{
	MisaCheckbox_SetStatus(hWinTapMode, SynthGetTapmode());
#if 0
	MisaCheckbox_SetStatus(hWinStrings,SynthGetStringmode());
#endif
	MisaProgressbar_SetPercentEx(hWinMasterVolume,SynthGetMastervolume(),0);
#ifdef PRESET_IN_LIST
	FillPresetList();
#else // PRESET_IN_SLIDEWIN
	presetSlideList.CreateSlideItem();
#endif  // PRESET_IN_SLIDEWIN

	WM_ShowWindow(hQuickWin);

}

void QuickScreen::Hide()
{
	WM_HideWindow(hQuickWin);
#ifdef PRESET_IN_LIST
	ClearPresetList();
#else // PRESET_IN_SLIDEWIN
	PresetSlideDeleteItems();
#endif  // PRESET_IN_SLIDEWIN


}

#ifdef PRESET_IN_LIST
bool QuickScreen::FillPresetList()
{
	int size;
	vector<string> preset_filenames;

	ReadPresetsDir(preset_filenames);
	size = preset_filenames.size();

	for (int idx = 0; idx < size; idx++)
	{
		LISTBOX_AddString(hPresetList, preset_filenames[idx].c_str());
	}

	preset_filenames.clear();
	return true;
}

void QuickScreen::ClearPresetList()
{
	for (unsigned int n = 0; n < LISTBOX_GetNumItems(hPresetList); n++)
		LISTBOX_DeleteItem(hPresetList, n);
}

bool QuickScreen::GetSelectedPreset(string &Name)
{
	char BuffName[256];
	int Sel = LISTBOX_GetSel(hPresetList);
	if (Sel < 0)
		return false;

	LISTBOX_GetItemText(hPresetList, Sel, BuffName, 256);
	Name = BuffName;
	return true;
}

#else  // PRESET_IN_SLIDEWIN

bool QuickScreen::LoadPreset()
{
	return (LoadEffect(presetSlideList) != 0);
}

#endif  // PRESET_IN_SLIDEWIN

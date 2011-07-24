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
	int x,y;

	x = 400;
	y = 0;
	hWinClose = BUTTON_CreateAsChild(x,y,bmCLOSE_NORMAL.XSize,bmCLOSE_NORMAL.YSize,hQuickWin, COMMON_ID_CLOSE,WM_CF_SHOW | WM_CF_MEMDEV);
	BUTTON_SetFocussable(hWinClose,0);
	BUTTON_SetBitmap(hWinClose,BUTTON_CI_UNPRESSED,&bmCLOSE_NORMAL);
	BUTTON_SetBitmap(hWinClose,BUTTON_CI_PRESSED,&bmCLOSE_PRESSED);

	x = 0;
	y = 0;
	hWinTapMode = MisaCheckbox_Create(x,y,bmQS_TAPMODE_UN.XSize,bmQS_TAPMODE_UN.YSize, hQuickWin,MISAQUICKSET_ID_TAPMODE,WM_CF_SHOW|WM_CF_MEMDEV, &bmQS_TAPMODE_UN, &bmQS_TAPMODE_SE);

	y = 60;
	hWinStringMode = MisaCheckbox_Create(x,y,bmQS_STRING_UN.XSize,bmQS_STRING_UN.YSize, hQuickWin, MISAQUICKSET_ID_STRINGS,WM_CF_SHOW|WM_CF_MEMDEV, &bmQS_STRING_UN, &bmQS_STRING_SE);

	y = 120;
	hWinBallMode = MisaCheckbox_Create(x,y,bmQS_STRING_UN.XSize,bmQS_STRING_UN.YSize, hQuickWin, MISAQUICKSET_ID_STRINGS,WM_CF_SHOW|WM_CF_MEMDEV, &bmQS_STRING_UN, &bmQS_STRING_SE);

	y =500;
	hWinAdvanced = BUTTON_CreateAsChild(x,y,bmQS_CONF_UN.XSize,bmQS_CONF_UN.YSize,hQuickWin, MISAQUICKSET_ID_ADVANCED,WM_CF_SHOW|WM_CF_MEMDEV);
	BUTTON_SetFocussable(hWinAdvanced,0);
	BUTTON_SetBitmap(hWinAdvanced,BUTTON_CI_UNPRESSED,&bmQS_CONF_UN);
	BUTTON_SetBitmap(hWinAdvanced,BUTTON_CI_PRESSED,&bmQS_CONF_UN);
	x = 0;//bmQS_CONF_UN.XSize+100;
	y = 100+16;
	hWinMasterVolume = MisaProgressbar_CreateEx(x,y,0,0,hQuickWin,MISAQUICKSET_ID_MASTERVOLUME,"Master","volume",MISAPROG_FONT24, 0);

	//LISTBOX_SetDefaultBkColor     (0, 0);
	//LISTBOX_SetDefaultTextColor     (0, 0xffffffff);
	//LISTBOX_SetDefaultTextColor     (1, 0x808080);


	hWinLoad = BUTTON_CreateAsChild(400, 400 ,bmPRESET_LOAD_UN.XSize,bmPRESET_LOAD_UN.YSize,hQuickWin, MISAQUICKSET_ID_LOAD, WM_CF_SHOW|WM_CF_MEMDEV);
	BUTTON_SetFocussable(hWinLoad,0);
	BUTTON_SetBitmap(hWinLoad,BUTTON_CI_UNPRESSED,&bmPRESET_LOAD_UN);
	BUTTON_SetBitmap(hWinLoad,BUTTON_CI_PRESSED,&bmPRESET_LOAD_SE);

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
	hPresetList = presetSlideList.CreateWidget(400, 150, 250, 250, hQuickWin, WM_CF_SHOW | WM_CF_MEMDEV | SLIDE_PANEL_SINGLE_COLUMN);
#endif  // PRESET_IN_SLIDEWIN





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

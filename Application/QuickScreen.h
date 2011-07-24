#ifndef _QUICK_SCREEN_DEF
#define _QUICK_SCREEN_DEF

#include <string>

using namespace std;

#include "BUTTON.h"
#include "MisaAppInc.h"
#include "PresetSlideList.h"


// Control Ids
typedef enum __MISAQUICKSET_ID
{
	MISAQUICKSET_ID_TAPMODE=GUI_ID_QUICKSET_BASE,
	MISAQUICKSET_ID_LOAD,
	MISAQUICKSET_ID_STRINGS,
	MISAQUICKSET_ID_ADVANCED,
	MISAQUICKSET_ID_MASTERVOLUME,
	MISAQUICKSET_ID_MAX
} MISAQUICKSET_ID;


class QuickScreen
{
public:
	QuickScreen();
	~QuickScreen();

	void Show();
	void Hide();

#ifdef PRESET_IN_LIST
	bool FillPresetList();
	void ClearPresetList();
	bool GetSelectedPreset(string &Name);
#else	// PRESET_IN_SLIDEWIN
	//PresetSlideList &GetPresetSlideList() {return presetSlideList;}
	bool LoadPreset();
#endif	// PRESET_IN_SLIDEWIN

protected:
	U8 CreateItems();
	U8 DeleteItems();

private:
	WM_HWIN hQuickWin;

	WM_HWIN hWinClose;
	WM_HWIN hWinTapMode;
	WM_HWIN hWinStringMode;
	WM_HWIN hWinBallMode;
	WM_HWIN hWinAdvanced;
	WM_HWIN hWinMasterVolume;

	WM_HWIN hPresetList;
	WM_HWIN hWinLoad;

	string LoadedPreset;

	PresetSlideList presetSlideList;

};

#endif _QUICK_SCREEN_DEF
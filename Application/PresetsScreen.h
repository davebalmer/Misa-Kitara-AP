
#ifndef __PRESETSSCREEN_H__
#define __PRESETSSCREEN_H__

#include <string>
#include <vector>
#include "PresetSlideList.h"

#define SLIDE_PANEL_SINGLE_COLUMN (1 << 16)
U8 CreatePresetsScreen(void* pPara);
U8 DeletePresetsScreen();
U8 TopPresetsScreen(WM_HWIN hPreWin);
void UpdateSynthSetting();



U8 PresetSlideCreateItems(WM_HWIN hParent, bool singleColumn);
U8 PresetSlideDeleteItems();
U8 DeletePresetSlidePanel(WM_HWIN hSlidePanel);
int LoadEffect(PresetSlideList &thePresetSlideList);
bool ReadPresetsDir(std::vector<std::string> &preset_filenames);

#endif // __PRESETSSCREEN_H__


#ifndef _SLIDE_LIST_DEF
#define _SLIDE_LIST_DEF


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa Sliding window support
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#include "GUI.h"
#include "WM.h"


class PresetSlideList
{
public:
	PresetSlideList(): singleColumn(false) {} 
	bool CreateWidget(int x0, int y0, int width, int height, WM_HWIN hParent, U32 Style);
	void DeleteWidget();

	void DeleteSlideWin();

	bool CreateSlideItem();

	bool IsPresetItem(WM_HWIN hFocus);


private:

	WM_HWIN hWinDelimiter;
	WM_HWIN hWinSlide;
	bool singleColumn;

};

#endif		// _SLIDE_LIST_DEF


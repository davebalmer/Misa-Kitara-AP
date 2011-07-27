#ifndef _KEYBOARD_DEF
#define _KEYBOARD_DEF

#include "MisaAppInc.h"
#include <vector>
#include <string>

class Keyboard
{
public:
	friend void KeyboardWindowProc(WM_MESSAGE* pMsg);

	Keyboard();
	void CreateSubWin(WM_HWIN hParent);
	void DeleteSubWin();
	void UpdateText(int line, int column, WM_HWIN hWin);

	// static
	static void Open(WM_HWIN Parent, int x0, int y0);
	static void Close();
	static bool IsOpen();

private:
	int X_highlight;
	int Y_highlight;
	const GUI_BITMAP *highlighted_Bmp;
	WM_HWIN hWinKbd;
	WM_HWIN hTextDisplay;

	std::string currentText;
};

#endif		// _KEYBOARD_DEF

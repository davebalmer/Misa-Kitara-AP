#include "Keyboard.h"
#include "Resource.h"
#include "TEXT.h"

#define KBD_NUM_LINES		4
#define KBD_NUM_COL			10
#define KBD_COLUMN_WIDTH	80
#define KBD_LINE_HEIGHT		81
#define KEYS_OFFSET_Y		98
#define MAX_CHAR_ALLOWED	12

#define KBD_TXT_ID (GUI_ID_PRESETS_BASE + 20)
static Keyboard *keyboard = NULL;
static std::vector<const GUI_BITMAP *> highlightedCharBmp[KBD_NUM_LINES];
static std::vector<std::string> KbdLines;


// Keyboard


void KeyboardWindowProc(WM_MESSAGE* pMsg)
{
	GUI_PID_STATE* pState = (GUI_PID_STATE*)pMsg->Data.p;
	WM_HWIN hWinString = 0;

	switch (pMsg->MsgId)
	{
	case WM_CREATE:
		keyboard->CreateSubWin(pMsg->hWin);
		break;

	case WM_DELETE:
		keyboard->DeleteSubWin();
		break;

	case WM_PAINT:
		GUI_DrawBitmap(&bm_KEYBOARD, 0 , 0);
		if (keyboard->highlighted_Bmp)
			GUI_DrawBitmap(keyboard->highlighted_Bmp, keyboard->X_highlight , keyboard->Y_highlight);
		break;

	case WM_TOUCH:
		if (pState)
		{
			int line = (pState->y - KEYS_OFFSET_Y)/ KBD_LINE_HEIGHT;
			int column = pState->x / KBD_COLUMN_WIDTH;
			if (line >= KBD_NUM_LINES || line < 0 || column < 0 || column >= KBD_NUM_COL)
				break;

			if (keyboard->highlighted_Bmp)
			{
				GUI_RECT RectToInvalidate ={keyboard->X_highlight, keyboard->Y_highlight, keyboard->X_highlight +  2 * KBD_COLUMN_WIDTH, keyboard->Y_highlight + KBD_LINE_HEIGHT};
				WM_InvalidateRect(pMsg->hWin, &RectToInvalidate);
			}
			if (pState->Pressed)
			{
				// highlight the key
				keyboard->X_highlight = column * KBD_COLUMN_WIDTH;		// 0, 80, 160, etc...
				keyboard->Y_highlight = line * KBD_LINE_HEIGHT + KEYS_OFFSET_Y;
				keyboard->highlighted_Bmp = highlightedCharBmp[line][column];
				if (keyboard->highlighted_Bmp == &bm_CHAR_o)
					keyboard->X_highlight = 7 * KBD_COLUMN_WIDTH;		// 0, 80, 160, etc...

				GUI_RECT RectToInvalidate = {keyboard->X_highlight, keyboard->Y_highlight, keyboard->X_highlight + 2 * KBD_COLUMN_WIDTH, keyboard->Y_highlight + KBD_LINE_HEIGHT};
				WM_InvalidateRect(pMsg->hWin, &RectToInvalidate);
			}
			else
			{
				keyboard->highlighted_Bmp = NULL;
				keyboard->UpdateText(line, column, pMsg->hWin);
			}
		}

		break;

	default:
		WM_DefaultProc(pMsg);
	};

}


// statics
void Keyboard::Open(WM_HWIN Parent, int x0, int y0)
{
	if(keyboard == NULL)
	{
		keyboard = new Keyboard;
		keyboard->hWinKbd = WM_CreateWindowAsChild(x0, y0, bm_KEYBOARD.XSize, bm_KEYBOARD.YSize, Parent, WM_CF_SHOW|WM_CF_MEMDEV, KeyboardWindowProc, 0);
	}
	//else
	//	WM_ShowWindow(keyboard->hWinKbd);
}


void Keyboard::Close()
{
	if(keyboard)
	{
		WM_DeleteWindow(keyboard->hWinKbd);
		delete keyboard;
		keyboard = NULL;
	}
}

bool Keyboard::IsOpen()
{
	return (keyboard != NULL);
}

// members
Keyboard::Keyboard() : hWinKbd(0), highlighted_Bmp(NULL)
{
	currentText = "";

	KbdLines.push_back("0123456789"); 
	KbdLines.push_back("qwertyuiop"); 
	KbdLines.push_back("asdfghjklB"); 
	KbdLines.push_back("zxcvbnm**C");

	highlightedCharBmp[0].push_back(&bm_CHAR_0);
	highlightedCharBmp[0].push_back(&bm_CHAR_1);
	highlightedCharBmp[0].push_back(&bm_CHAR_2);
	highlightedCharBmp[0].push_back(&bm_CHAR_3);
	highlightedCharBmp[0].push_back(&bm_CHAR_4);
	highlightedCharBmp[0].push_back(&bm_CHAR_5);
	highlightedCharBmp[0].push_back(&bm_CHAR_6);
	highlightedCharBmp[0].push_back(&bm_CHAR_7);
	highlightedCharBmp[0].push_back(&bm_CHAR_8);
	highlightedCharBmp[0].push_back(&bm_CHAR_9);

	highlightedCharBmp[1].push_back(&bm_CHAR_Q);
	highlightedCharBmp[1].push_back(&bm_CHAR_W);
	highlightedCharBmp[1].push_back(&bm_CHAR_E);
	highlightedCharBmp[1].push_back(&bm_CHAR_R);
	highlightedCharBmp[1].push_back(&bm_CHAR_T);
	highlightedCharBmp[1].push_back(&bm_CHAR_Y);
	highlightedCharBmp[1].push_back(&bm_CHAR_U);
	highlightedCharBmp[1].push_back(&bm_CHAR_I);
	highlightedCharBmp[1].push_back(&bm_CHAR_O);
	highlightedCharBmp[1].push_back(&bm_CHAR_P);

	highlightedCharBmp[2].push_back(&bm_CHAR_A);
	highlightedCharBmp[2].push_back(&bm_CHAR_S);
	highlightedCharBmp[2].push_back(&bm_CHAR_D);
	highlightedCharBmp[2].push_back(&bm_CHAR_F);
	highlightedCharBmp[2].push_back(&bm_CHAR_G);
	highlightedCharBmp[2].push_back(&bm_CHAR_H);
	highlightedCharBmp[2].push_back(&bm_CHAR_J);
	highlightedCharBmp[2].push_back(&bm_CHAR_K);
	highlightedCharBmp[2].push_back(&bm_CHAR_L);
	highlightedCharBmp[2].push_back(&bm_CHAR_b);

	highlightedCharBmp[3].push_back(&bm_CHAR_Z);
	highlightedCharBmp[3].push_back(&bm_CHAR_X);
	highlightedCharBmp[3].push_back(&bm_CHAR_C);
	highlightedCharBmp[3].push_back(&bm_CHAR_V);
	highlightedCharBmp[3].push_back(&bm_CHAR_B);
	highlightedCharBmp[3].push_back(&bm_CHAR_N);
	highlightedCharBmp[3].push_back(&bm_CHAR_M);
	highlightedCharBmp[3].push_back(&bm_CHAR_o);
	highlightedCharBmp[3].push_back(&bm_CHAR_o);
	highlightedCharBmp[3].push_back(&bm_CHAR_c);

}


void Keyboard::CreateSubWin(WM_HWIN hParent)
{
	hTextDisplay = TEXT_CreateAsChild(246, 27, 309, 43, hParent, KBD_TXT_ID, WM_CF_SHOW|WM_CF_MEMDEV, "",GUI_TA_HCENTER|GUI_TA_VCENTER);
	TEXT_SetFont(hTextDisplay, &GUI_Font32_ASCII);
	TEXT_SetTextColor(hTextDisplay, GUI_WHITE);
}

void Keyboard::DeleteSubWin()
{
	if (hTextDisplay)
	{
		TEXT_Delete(hTextDisplay);
		hTextDisplay = 0;
	}
}

void Keyboard::UpdateText(int line, int column, WM_HWIN hWin)
{
	char c = KbdLines[line][column];
	if (c == 'B')
	{
		if (keyboard->currentText.size() > 0)
		{
			currentText.erase(keyboard->currentText.size() - 1, 1);
			TEXT_SetText(hTextDisplay, currentText.c_str());
		}
	}
	else if (c == '*')
	{
		WM_MESSAGE msg;
		msg.MsgId = WM_CUST_KEYBOARD_DONE;
		msg.hWin = WM_GetParent(hWin);
		msg.hWinSrc = hWin;
		msg.Data.p = currentText.c_str();
		WM_SendMessage(WM_GetParent(hWin), &msg);
	}

	else if (c == 'C')
	{
		WM_MESSAGE msg;
		msg.MsgId = WM_CUST_KEYBOARD_CANCEL;
		msg.hWin = WM_GetParent(hWin);
		msg.hWinSrc = hWin;
		msg.Data.v = 0;
		WM_SendMessage(WM_GetParent(hWin), &msg);	//	WM_NotifyParent
	}
	else
	{
		if (currentText.size() < MAX_CHAR_ALLOWED)
			currentText.push_back(c);
		TEXT_SetText(hTextDisplay, currentText.c_str());
	}
	

}

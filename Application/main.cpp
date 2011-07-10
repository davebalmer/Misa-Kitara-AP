
#include "APPTYPES.h"

#include <cstdlib>
#include <iostream>

#include "GUI.h"

#include "ucGuiMain.h"

int main(int argc, char **argv)
{
#if 1
	ucGuiMain(0);
#else
	GUI_PID_STATE TouchState;
	GUI_Init();
	Touchpanel tp;
	GUI_CURSOR_Show();
	GUI_Delay(1000);
	GUI_CURSOR_Select(&GUI_CursorCrossL);
	GUI_Delay(1000);
	unThreadRun = 1000;
	while(unThreadRun--)
	{
		GUI_TOUCH_GetState(&TouchState);
		GUI_DispStringAt("Analog input: ", 0, 40);
		GUI_DispDec(GUI_TOUCH_GetxPhys(),4);
		GUI_DispString(",");
		GUI_DispDec(GUI_TOUCH_GetyPhys(),4);
		
		GUI_DispStringAt("TOUCH_X_MeasureX: ",0,60);
		GUI_DispDec(TouchState.x,4);
		GUI_DispString(",");
		GUI_DispDec(TouchState.y,4);
		
		GUI_Delay(100);
	};
	tp.WaitForExit();

	LCD_L0_Exit();
#endif
	return 0;
}

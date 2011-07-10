

#ifndef __MISAQUICKSETSCREEN_H__
#define __MISAQUICKSETSCREEN_H__

U8 CreateMisaquicksetScreen(void* pPara);
U8 DeleteMisaquicksetScreen();

U8 BeginMisaquicksetScreen();
U8 EndMisaquicksetScreen();
U8 TestMisaquicksetScreen(int x, int y,int Pressed);

#endif // __MISAQUICKSETSCREEN_H__


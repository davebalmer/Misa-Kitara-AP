
#ifndef __MISACHECKBOX_H__
#define __MISACHECKBOX_H__


// OR : Drag & Drop
#include "MisaAppInc.h"
#include "VoiceScreen.h"
#include "MisamenuConfig.h"

class DragVoice
{
public:
	DragVoice(): hWinOverlay(0) {Finish();}
	void Start(int string, int voice) {strIdx = string; voiceIdx = voice;}
	void Finish();
	bool IsDragged() {return strIdx >= 0;}
	struct voice *GetVoiceDataPtr() {return &GetCurrentSettingPtr()->voices[strIdx].at(voiceIdx);}
	int GetStringIdx() {return strIdx;}
	int GetVoiceIdx() {return voiceIdx;}
	void MoveOn(int x, int y, GUI_HWIN parent);
	bool Moving() {return (hWinOverlay != 0);}

private:
	int strIdx;
	int voiceIdx;
	WM_HWIN hWinOverlay;
};
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa common structure
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

typedef struct __MISACOMBO
{
	GUI_RECT body;
	GUI_RECT leftbt;
	GUI_RECT rightbt;
	GUI_RECT centre;
} MISACOMBO,*PMISACOMBO;

typedef struct __MISASWITCH
{
	GUI_RECT body;
	GUI_RECT leftbt;
	GUI_RECT rightbt;
} MISASWITCH,*PMISASWITCH;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa initialize
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void MisaInitWidget();

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa checkbox
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

BUTTON_Handle MisaCheckbox_Create(int x0, int y0, int xsize, int ysize, WM_HWIN hParent, int Id, int Flags,const GUI_BITMAP * pUncheck,const GUI_BITMAP * pCheck);
void MisaCheckbox_Delete(BUTTON_Handle hMisaCheckbox);
void MisaCheckbox_SetStatus(BUTTON_Handle hMisaCheckbox,int State);
U8 MisaCheckbox_GetStatus(BUTTON_Handle hMisaCheckbox);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa radio button
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

typedef struct __MISARADIO_GROUP
{
	int datasize;
	BUTTON_Handle Radios[2];
} MISARADIO_GROUP;

BUTTON_Handle MisaRadio_Create(int x0, int y0, int xsize, int ysize, WM_HWIN hParent, int Id, int Flags,const GUI_BITMAP * pUncheck,const GUI_BITMAP * pCheck);
BUTTON_Handle MisaRadio_CreateEx(int x0, int y0, int xsize, int ysize, WM_HWIN hParent, int Id, int Flags, const char* pName, const GUI_BITMAP * pUncheck,const GUI_BITMAP * pCheck);
void MisaRadio_Delete(BUTTON_Handle hMisaRadio);
void MisaRadio_SetGoup(MISARADIO_GROUP radioGroup);
void MisaRadio_SetStatus(BUTTON_Handle hMisaRadio,int State);
U8 MisaRadio_GetStatus(BUTTON_Handle hMisaRadio);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa progress bar
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#define MISAPROG_FONT8 0
#define MISAPROG_FONT16 1
#define MISAPROG_FONT24 2
#define MISAPROG_FONT32 3

typedef void (*MISAPROGINFOCB)(int value,char* buf);

WM_HWIN MisaProgressbar_Create(int x, int y, int width, int height, WM_HWIN hParent, int Id, const char* label, int fontSize);
WM_HWIN MisaProgressbar_CreateEx(int x, int y, int width, int height, WM_HWIN hParent, int Id, const char* label, const char* seclab, int fontSize,MISAPROGINFOCB TipCB);
void MisaProgressbar_Delete(WM_HWIN hMisaProgress);
int MisaProgressbar_GetPercent(WM_HWIN hMisaProgress);
void MisaProgressbar_SetPercent(WM_HWIN hMisaProgress, int percent);
void MisaProgressbar_SetPercentEx(WM_HWIN hMisaProgress, int percent, U8 update);
void MisaProgressbar_Increase(WM_HWIN hMisaProgress);
void MisaProgressbar_Decrease(WM_HWIN hMisaProgress);
int MisaProgressbar_GetCheck(WM_HWIN hMisaProgress);
void MisaProgressbar_SetCheck(WM_HWIN hMisaProgress, int state);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa popup menu
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

typedef enum __CONTROLMENU_ITEM
{
	CTL_NONE,
	CTL_TOUCH_X,
	CTL_TOUCH_Y,
	CTL_DRAG_X,
	CTL_DRAG_Y,
	CTL_BALL_X,
	CTL_BALL_Y,
	CTL_VARIATION,
	CTL_STOPSOUND,
	CTL_VARMIN,
	CTL_VARMAX,
	CTL_MAX,
} CONTROLMENU_ITEM;

typedef void (*MISAPOPMENUINFOCB)(int value,char* buf);

WM_HWIN MisaMenu_Create(int x, int y, int width, int height, WM_HWIN hParent, int Id, const char* label, int fontSize);
void MisaMenu_Delete(WM_HWIN hMisaMenu);
void MisaMenu_SetStatus(U32 index, int status,int val,int inverse);
void MisaMenu_Popup(WM_HWIN hMisaMenu,WM_HWIN hParent,WM_HWIN hSrcWin,int x,int y);
void MisaMenu_PopupEx(WM_HWIN hMisaMenu,WM_HWIN hParent,WM_HWIN hSrcWin,int x, int y, int min, int max, MISAPOPMENUINFOCB MinTip, MISAPOPMENUINFOCB MaxTip);
U8 MisaSetcontrolmenu_Create();
U8 MisaSetcontrolmenu_Delete();
U8 MisaSetcontrolmenu_Popup(WM_HWIN hParent,WM_HWIN hSrcWin,int x,int y);
U8 MisaSetcontrolmenu_PopupEx(WM_HWIN hParent,WM_HWIN hSrcWin,int x, int y, int min, int max, MISAPOPMENUINFOCB MinTip, MISAPOPMENUINFOCB MaxTip);
WM_HWIN MisaSetcontrolmenu_GetSrcWin();
int MisaSetcontrolmenu_GetVarMin();
int MisaSetcontrolmenu_GetVarMax();
int MisaSetcontrolmenu_GetInverse();

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa Synth string panel
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#define STRING_PANEL_WIDTH 792
#define STRING_PANEL_HEIGHT 72

WM_HWIN MisaSynthString_Create(int x, int y, int width, int height, WM_HWIN hParent, int Id);
void MisaSynthString_Delete(WM_HWIN hMisaSynthString);
void MisaSynthString_SetTune(WM_HWIN hMisaSynthString, U8 tune);
U8 MisaSynthString_GetTune(WM_HWIN hMisaSynthString);
U8 MisaSynthString_GetChannel(WM_HWIN hMisaSynthString);
U8 MisaSynthString_DelSynthoice(WM_HWIN hMisaSynthString);
U32 MisaSynthString_GetVoiceIndex(WM_HWIN hMisaSynthString);
U8 MisaSynthString_SetVoice(WM_HWIN hMisaSynthString,int voice);
void* MisaSynthString_GetVoice(WM_HWIN hMisaSynthString,int index);
U8 MisaSynthString_SwitchSynthMode(WM_HWIN hMisaSynthString,U8 mode, U8 channel);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa progress combo
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

WM_HWIN MisaProgressCombo_Create(int x, int y, int width, int height, WM_HWIN hParent, int Flags, int Id, const char* label);
WM_HWIN MisaProgressCombo_CreateEx(int x, int y, int width, int height, WM_HWIN hParent, int Flags, int Id, const char* label, MISAPROGINFOCB TipCB);
void MisaProgressCombo_Delete(WM_HWIN hMisaProgress);
void MisaProgressCombo_SetRange(WM_HWIN hMisaProgcombo, int max, int offset);
int MisaProgressCombo_GetPercent(WM_HWIN hMisaProgcombo);
void MisaProgressCombo_SetPercent(WM_HWIN hMisaProgcombo, int percent);
void MisaProgressCombo_SetText(WM_HWIN hMisaProgcombo, char* str);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa small progress combo
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

WM_HWIN MisaSmallProgressCombo_Create(int x, int y, int width, int height, WM_HWIN hParent, int Flags, int Id, const char* label);
WM_HWIN MisaSmallProgressCombo_CreateEx(int x, int y, int width, int height, WM_HWIN hParent, int Flags, int Id, const char* label, MISAPROGINFOCB TipCB);
void MisaSmallProgressCombo_Delete(WM_HWIN hMisaProgress);
void MisaSmallProgressCombo_SetRange(WM_HWIN hMisaProgcombo, int max, int offset);
int MisaSmallProgressCombo_GetPercent(WM_HWIN hMisaProgcombo);
void MisaSmallProgressCombo_SetPercent(WM_HWIN hMisaProgcombo, int percent);
void MisaSmallProgressCombo_SetPercentEx(WM_HWIN hMisaProgcombo, int percent, U8 update);
void MisaSmallProgressCombo_SetCheck(WM_HWIN hMisaProgcombo, int state);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa item button
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

BUTTON_Handle MisaItem_Create(int x0, int y0, int xsize, int ysize, WM_HWIN hParent, int Id, int Flags,const GUI_BITMAP * pUncheck,const GUI_BITMAP * pCheck);
BUTTON_Handle MisaItem_CreateEx(int x0, int y0, int xsize, int ysize, WM_HWIN hParent, int Id, int Flags, const char* pName, const GUI_BITMAP * pUncheck,const GUI_BITMAP * pCheck);
void MisaItem_Delete(BUTTON_Handle hMisaItem);
//void MisaItem_SetGoup(MISARADIO_GROUP radioGroup);
void MisaItem_SetStatus(BUTTON_Handle hMisaItem,int State);
U8 MisaItem_GetStatus(BUTTON_Handle hMisaItem);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa vertical slide bar
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

WM_HWIN MisaVerSlider_Create(int x, int y, int width, int height, WM_HWIN hParent, int Id);
WM_HWIN MisaVerSlider_CreateEx(int x, int y, int width, int height, WM_HWIN hParent, int Id,MISAPROGINFOCB TipCB);
void MisaVerSlider_Delete(WM_HWIN hVerslider);
int MisaVerSlider_GetPercent(WM_HWIN hVerslider);
void MisaVerSlider_SetPercentEx(WM_HWIN hVerslider, int percent, U8 update);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Misa Sliding window support
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#define INDICATORFRAME 50

#endif // __MISACHECKBOX_H__


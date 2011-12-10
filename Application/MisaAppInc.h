
#ifndef __MISAAPPINC_H__
#define __MISAAPPINC_H__

#include "GUI.h"
#include "BUTTON.h"

#ifdef MISA_DEBUG_PRINT
#define DEBUG_STDOUT(content) Debug_stdout(content)
#else
#define DEBUG_STDOUT(content)
#endif

///////////////////////////////////////////////////////////////////////////////
// Misa Sliding window support
///////////////////////////////////////////////////////////////////////////////
#define INDICATORFRAME 50


void Debug_stdout(char* content);

#define WM_CUST_CHANGE_TEXT WM_USER+1
#define WM_CUST_GET_MISAMENUSRCID WM_USER+2
#define WM_CUST_GET_MISAMENUVARMIN WM_USER+3
#define WM_CUST_GET_MISAMENUVARMAX WM_USER+4
#define WM_CUST_GET_MISASYNTHVOICE WM_USER+5
#define WM_CUST_KEYBOARD_CANCEL	WM_USER + 6
#define WM_CUST_KEYBOARD_DONE	WM_USER + 7


#define GUI_ID_COMMON_BASE GUI_ID_USER
#define GUI_ID_MAINMENU_BASE GUI_ID_USER+50
#define GUI_ID_PRESETS_BASE GUI_ID_USER+100
#define GUI_ID_SYNTH_BASE GUI_ID_USER+150
#define GUI_ID_MIDI_BASE GUI_ID_USER+200
#define GUI_ID_COMPRESSION_BASE GUI_ID_USER+250
#define GUI_ID_DISTORTION_BASE GUI_ID_USER+300
#define GUI_ID_MODULATION_BASE GUI_ID_USER+350
#define GUI_ID_DELAY_BASE GUI_ID_USER+400
#define GUI_ID_REVERB_BASE GUI_ID_USER+450
#define GUI_ID_MIXER_BASE GUI_ID_USER+500
#define GUI_ID_EQ_BASE GUI_ID_USER+550
#define GUI_ID_CTL_BASE GUI_ID_USER+600
#define GUI_ID_CONTROLASSIGNMENTS_BASE GUI_ID_USER+650
#define GUI_ID_MIDICONTROL_BASE GUI_ID_USER+700
#define GUI_ID_VOICE_BASE GUI_ID_USER+750
#define GUI_ID_SYSTEM_BASE GUI_ID_USER+800
#define GUI_ID_QUICKSET_BASE GUI_ID_USER+850
#define GUI_ID_BITCRUSHER_BASE GUI_ID_USER+900
#define GUI_ID_SCENE_BASE GUI_ID_USER+950

#define WM_NOTIFICATION_STRINGPANEL_SYNTH WM_NOTIFICATION_USER+1
#define WM_NOTIFICATION_ADDVOICE WM_NOTIFICATION_USER+2

#define COMMON_ITEM_ALL 0x0000000F
#define COMMON_ITEM_CLOSE 0x00000001
#define COMMON_ITEM_CHECK 0x00000002
#define COMMON_ITEM_FX1 0x00000004
#define COMMON_ITEM_FX2 0x00000008


typedef enum __COMMCOMPONENT_ID
{
	COMMON_ID_CLOSE=GUI_ID_COMMON_BASE,
	COMMON_ID_CHECKON,
	COMMON_ID_FX1,
	COMMON_ID_FX2,
	COMMON_ID_COMCHANGE,
	COMMON_ID_MAX
} COMMCOMPONENT_ID;

typedef enum __CONTROLMENU_ITEM_ID
{
	CTL_NONE_ID=GUI_ID_CTL_BASE,
	CTL_TOUCH_X_ID,
	CTL_TOUCH_Y_ID,
	CTL_DRAG_X_ID,
	CTL_DRAG_Y_ID,
	CTL_BALL_X_ID,
	CTL_BALL_Y_ID,
	CTL_VARIATION_ID,
	CTL_STOPSOUND_ID,
	CTL_VARMIN_ID,
	CTL_VARMAX_ID
} CONTROLMENU_ITEM_ID;

typedef struct __COMMCOMPONENT
{
	BUTTON_Handle hClose;
	BUTTON_Handle hCheckOn;
	BUTTON_Handle hFX1;
	BUTTON_Handle hFX2;
} COMMCOMPONENT,*LPCOMMCOMPONENT;

typedef struct __COMMON_STATUS
{
	U8 Fx_Sel;
	U8 OnCheck;
} COMMON_STATUS,*LPCOMMON_STATUS;

COMMCOMPONENT CreateCommComponent(WM_HWIN hParent);
COMMCOMPONENT CreateCommComponentEx(WM_HWIN hParent,U32 items);
U8 DeleteCommComponent(COMMCOMPONENT hComponents);
U8 ProcessCommComponent(WM_MESSAGE* pMsg, LPCOMMCOMPONENT pCommComp);
U8 DispatchCommComponent(WM_MESSAGE* pMsg, LPCOMMCOMPONENT pCommComp);
U8 GetSlidingBordercheck(WM_HWIN hSlide, WM_HWIN hParent, int IndicatorFrame = INDICATORFRAME);
U8 SlideGoNextPage(WM_HWIN hWin, int IndicatorFrame = INDICATORFRAME);
U8 SlideGoPreviousPage(WM_HWIN hWin, int IndicatorFrame = INDICATORFRAME);
void AssignEffectControlMenuProc(int menuId,int fx);
void AssignEffectControlMenuProc_SPC(int menuId,int fx);
void AssignVoiceControlMenuProc(int menuId, unsigned char str, unsigned char voice_index);
void Misa_SmoothlyMoveWindow(WM_HWIN hWin,WM_HWIN hParent,int Dx);

#define MISAMODULE_COMPRESSION 0x00000001
#define MISAMODULE_DISTORTION 0x00000002
#define MISAMODULE_MODULATION 0x00000004
#define MISAMODULE_DELAY 0x00000008
#define MISAMODULE_REVERB 0x00000010
#define MISAMODULE_MIXER 0x00000020
#define MISAMODULE_EQ 0x00000040
#define MISAMODULE_SYNTH 0x00000080
#define MISAMODULE_VOICE 0x00000100
#define MISAMODULE_CTRLASSIGN 0x00000200
#define MISAMODULE_BITCRUSHER 0x00000400

#define MISAMODULE_ALL 0xFFFFFFFF

#define PAGE_POSITIVE_FACTOR 175
#define PAGE_NEGATIVE_FACTOR -175

void UpdateSynthSettingEx(U32 module, U8 reloadFromSystem=1);

void RunFramework(void);
void Misa_Delay(int ms);

#endif // __MISAAPPINC_H__


#ifndef __MODULATIONSCREEN_H__
#define __MODULATIONSCREEN_H__

typedef enum __MODULATION_ID
{
	MODULATION_ID_LEFTARROW=GUI_ID_MODULATION_BASE,
	MODULATION_ID_RIGHTARROW,
	MODULATION_ID_PROGMODRATE,
	MODULATION_ID_PROGMODDEPTH,
	MODULATION_ID_PROGLEVEL,
	MODULATION_ID_PROGFDBK,
	MODULATION_ID_PROGFDBKFILTER,
	MODULATION_ID_PROGSHAPE,
	MODULATION_ID_PROGTIME,
	MODULATION_ID_MAX
} MODULATION_ID;

typedef struct __MODULATION_INFO
{
	U8 Fx;
	U8 On;
	U8 type;
	U8 mod_rate;
	U8 mod_depth;
	U8 level;
	U8 fdbk;
	U8 fdbkfilter;
	U8 shape;
	U8 time;
} MODULATION_INFO,*PMODULATION_INFO;

U8 CreateModulationScreen(void* pPara);
U8 DeleteModulationScreen();
U8 TopModulationScreen(WM_HWIN hPreWin);
U8 UpdateModulationInfo(PMODULATION_INFO pInfo);
U8 GetModulationInfo(PMODULATION_INFO pInfo);
U8 ModulationLoadParameter(int index, U8 update=0);

#endif // __MODULATIONSCREEN_H__


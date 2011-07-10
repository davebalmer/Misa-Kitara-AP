
#ifndef __EQSCREEN_H__
#define __EQSCREEN_H__

typedef enum __EQ_ID
{
	EQ_TOMIXERSCREEN_ID=GUI_ID_EQ_BASE,
	EQ_LOWBANDGAIN_ID,
	EQ_LOWBANDFREQ_ID,
	EQ_LOWMIDBANDQ_ID,
	EQ_LOWMIDBANDGAIN_ID,
	EQ_LOWMIDBANDFREQ_ID,
	EQ_HIGHMIDBANDGAIN_ID,
	EQ_HIGHMIDBANDFREQ_ID,
	EQ_HIGHMIDBANDQ_ID,
	EQ_HIGHBANDGAIN_ID,
	EQ_HIGHBANDFREQ_ID,
	EQ_ID_MAX
} EQ_ID;

typedef struct __EQ_INFO
{
	U8 lowbandgain;
	U8 lowbandfreq;
	U8 lowmidbandQ;
	U8 lowmidbandgain;
	U8 lowmidbandfreq;
	U8 highmidbandgain;
	U8 highmidbandfreq;
	U8 highmidbandQ;
	U8 highbandgain;
	U8 highbandfreq;
	U8 On;
} EQ_INFO,*PEQ_INFO;

U8 CreateEqScreen(void* pPara);
U8 DeleteEqScreen();
U8 TopEqScreen(WM_HWIN hPreWin);
U8 UpdateEqInfo(PEQ_INFO pInfo);
U8 GetEqInfo(PEQ_INFO pInfo);

#endif // __EQSCREEN_H__


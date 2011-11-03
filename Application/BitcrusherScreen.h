
#ifndef __BITCRUSHER_H__
#define __BITCRUSHER_H__

typedef enum __BITCRUSHER_ID
{
	BITCRUSHER_ID_BITRESOLUTION=GUI_ID_BITCRUSHER_BASE,
	BITCRUSHER_ID_DOWNSAMPLINGFACTOR,
	BITCRUSHER_ID_MAX
} BITCRUSHER_ID;

typedef struct __BITCRUSHER_INFO
{
	U8 Fx;
	U8 On;
	U8 bitresolution;
	U8 downsamplingFactor;		// 1-16
} BITCRUSHER_INFO,*PBITCRUSHER_INFO;

U8 CreateBitCrusherScreen(void* pPara);
U8 DeleteBitCrusherScreen();
U8 TopBitCrusherScreen(WM_HWIN hPreWin);
U8 UpdateBitCrusherInfo(PBITCRUSHER_INFO pInfo);
U8 GetBitCrusherInfo(PBITCRUSHER_INFO pInfo);

#endif // __BITCRUSHER_H__


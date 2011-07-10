
#ifndef __COMPRESSIONSCREEN_H__
#define __COMPRESSIONSCREEN_H__

typedef enum __COMPRESSION_ID
{
	COMPRESSION_ID_HARD=GUI_ID_COMPRESSION_BASE,
	COMPRESSION_ID_SOFT,
	COMPRESSION_ID_PROGATTACK,
	COMPRESSION_ID_PROGRELEASE,
	COMPRESSION_ID_PROGBOOST,
	COMPRESSION_ID_PROGTRES,
	COMPRESSION_ID_PROGRATIO,
	COMPRESSION_ID_MAX
} COMPRESSION_ID;

typedef struct __COMPRESSION_INFO
{
	U8 Fx;
	U8 On;
	U8 softhard;
	U8 attack;
	U8 release;
	U8 boost;
	U8 thres;
	U8 ratio;
} COMPRESSION_INFO,*PCOMPRESSION_INFO;

U8 CreateCompressionScreen(void* pPara);
U8 DeleteCompressionScreen();
U8 TopCompressionScreen(WM_HWIN hPreWin);
U8 UpdateCompressionInfo(PCOMPRESSION_INFO pInfo);
U8 GetCompressionInfo(PCOMPRESSION_INFO pInfo);

#endif // __COMPRESSIONSCREEN_H__


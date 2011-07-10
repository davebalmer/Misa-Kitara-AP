
#ifndef __MODULATIONPRESETS_H__
#define __MODULATIONPRESETS_H__

#define MODULATIONPRESETS_NUM 24

typedef struct __MODPRESET
{
	char* name;
	U8 level;
	U8 delay;
	U8 Feedback;
	U8 hpf;
	U8 HDAMP;
	U8 rate;
	U8 depth;
	U8 shape;
	U8 rotary;
} MODPRESET,*PMODPRESET;

extern MODPRESET const *pModpresets[MODULATIONPRESETS_NUM];

#endif // __MODULATIONPRESETS_H__


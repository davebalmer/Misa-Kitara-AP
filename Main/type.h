#ifndef _TYPE_H_
#define _TYPE_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef signed char INT8;
typedef signed short INT16;
typedef signed int INT32;
typedef unsigned char UINT8;
typedef unsigned short UINT16;
typedef unsigned int UINT32;

typedef unsigned char CARD8;
typedef unsigned short CARD16;
typedef unsigned int  CARD32;
typedef volatile CARD8 VOL8;
typedef volatile CARD16 VOL16;
typedef volatile CARD32 VOL32;
typedef CARD8 BOOL;

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef NULL
#define NULL (void *)0
#endif

#ifdef __cplusplus
}
#endif

#endif


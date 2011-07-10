
#ifndef __MISADIALOGUEBOX_H__
#define __MISADIALOGUEBOX_H__

#include "WM.h"

#if defined(__cplusplus)
extern "C"
{
/* Make sure we have C-declarations in C++ programs */
#endif

int Misa_ConfirmBox(const char * sMessage, const char * sCaption, int Flags);
int Misa_WarningBox(const char * sMessage, const char * sCaption, int Flags);
int Misa_WarningMessageBegin();
int Misa_WarningMessageEnd();

#if defined(__cplusplus)
}
#endif


#endif // __MISADIALOGUEBOX_H__

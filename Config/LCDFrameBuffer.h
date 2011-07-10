#ifndef __LCDFRAMEBUFFER_H__
#define __LCDFRAMEBUFFER_H__

#ifdef WIN32
extern "C"
{
#endif

void SetLcdFrameBuffer(unsigned long* pFrame);
void SetLcdFlip(unsigned char flip);
unsigned char GetLcdFlip();

#ifdef WIN32
}
#endif

#endif // __LCDFRAMEBUFFER_H__


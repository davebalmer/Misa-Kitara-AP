/*
 *********************************************************************************************************
 *                                                uC/GUI
 *                        Universal graphic software for embedded applications
 *
 *                       (c) Copyright 2002, Micrium Inc., Weston, FL
 *                       (c) Copyright 2002, SEGGER Microcontroller Systeme GmbH
 *
 *              礐/GUI is protected by international copyright laws. Knowledge of the
 *              source code may not be used to write a similar product. This file may
 *              only be used in accordance with a license and should not be redistributed
 *              in any way. We appreciate your understanding and fairness.
 *
 ----------------------------------------------------------------------
File        : LCDFrameBuffer.c
Purpose     : This is emu a lcd use FrameBuffer
----------------------------------------------------------------------   
Version-Date---Author-Explanation                                        
----------------------------------------------------------------------   
0.00.01 061122 PeterChan a) Only for test on linux Framebuffer in 16bit
---------------------------END-OF-HEADER------------------------------
 */
#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include "mapm.h"

#include "LCD_Private.h"      /* private modul definitions & config */
#include "GUI_Private.h"
#include "GUIDebug.h"

#define D2DUPDATE

#if (LCD_CONTROLLER == 9999) \
		     && (!defined(WIN32) | defined(LCD_SIMCONTROLLER))

/*********************************************************************
 *
 *       Defines
 *
 **********************************************************************
 */

#ifndef LCD_INIT_CONTROLLER
#define LCD_INIT_CONTROLLER()
#endif

/*********************************************
 *
 *       Macros for MIRROR_, SWAP_ and LUT_
 *
 **********************************************
 */

#if (!defined (LCD_LUT_COM) && !defined(LCD_LUT_SEG))
#if   (!LCD_MIRROR_X && !LCD_MIRROR_Y && !LCD_SWAP_XY) 
#define LOG2PHYS_X(x, y) x
#define LOG2PHYS_Y(x, y) y
#elif (!LCD_MIRROR_X && !LCD_MIRROR_Y &&  LCD_SWAP_XY) 
#define LOG2PHYS_X(x, y) y
#define LOG2PHYS_Y(x, y) x
#elif (!LCD_MIRROR_X &&  LCD_MIRROR_Y && !LCD_SWAP_XY) 
#define LOG2PHYS_X(x, y) x
#define LOG2PHYS_Y(x, y) LCD_YSIZE - 1 - (y)
#elif (!LCD_MIRROR_X &&  LCD_MIRROR_Y &&  LCD_SWAP_XY) 
#define LOG2PHYS_X(x, y) LCD_YSIZE - 1 - (y)
#define LOG2PHYS_Y(x, y) x
#elif ( LCD_MIRROR_X && !LCD_MIRROR_Y && !LCD_SWAP_XY) 
#define LOG2PHYS_X(x, y) LCD_XSIZE - 1 - (x)
#define LOG2PHYS_Y(x, y) y
#elif ( LCD_MIRROR_X && !LCD_MIRROR_Y &&  LCD_SWAP_XY) 
#define LOG2PHYS_X(x, y) LCD_YSIZE - 1 - (y)
#define LOG2PHYS_Y(x, y) x
#elif ( LCD_MIRROR_X &&  LCD_MIRROR_Y && !LCD_SWAP_XY) 
#define LOG2PHYS_X(x, y) LCD_XSIZE - 1 - (x)
#define LOG2PHYS_Y(x, y) LCD_YSIZE - 1 - (y)
#elif ( LCD_MIRROR_X &&  LCD_MIRROR_Y &&  LCD_SWAP_XY) 
#error This combination of mirroring/swapping not yet supported
#endif
#else
#if   ( defined (LCD_LUT_COM) && !defined(LCD_LUT_SEG))
#define LOG2PHYS_X(x, y) x
#define LOG2PHYS_Y(x, y) LCD__aLine2Com0[y]
#elif (!defined (LCD_LUT_COM) &&  defined(LCD_LUT_SEG))
#define LOG2PHYS_X(x, y) LCD__aCol2Seg0[x]
#define LOG2PHYS_Y(x, y) y
#elif ( defined (LCD_LUT_COM) &&  defined(LCD_LUT_SEG))
#define LOG2PHYS_X(x, y) LCD__aCol2Seg0[x]
#define LOG2PHYS_Y(x, y) LCD__aLine2Com0[y]
#endif
#endif

/*********************************************
 *
 *       Macros for simulation
 *
 **********************************************
 */

#ifdef WIN32
#undef LCD_WRITE_A0
#undef LCD_WRITE_A1
#undef LCD_READ_A0
#undef LCD_READ_A1
void SIM_WriteA1C0(U8 Data);
void SIM_WriteA0C0(U8 cmd);
U8   SIM_ReadA1C0(void);
U8   SIM_ReadA0C0(void);
#define LCD_WRITE_A1(Data) SIM_WriteA1C0(Data) 
#define LCD_WRITE_A0(cmd)  SIM_WriteA0C0(cmd)
#define LCD_READ_A1()      SIM_ReadA1C0()
#define LCD_READ_A0()      SIM_ReadA0C0()
#endif

/*********************************************************************
 *
 *       Static Val
 *
 **********************************************************************
 */
static volatile unsigned long *D2D;
static unsigned short *off_screen_buffer;

int phys_mem_fd;

//#ifndef D2DUPDATE
static int fbfd = 0;
static unsigned char* pFrameBuffer = 0;
static int xres = 0;
static int yres = 0;
static int bits_per_pixel = 0;
static unsigned long screensize = 0;
//#endif
static unsigned char DisplayFlip = 0;

#define RGB(r,g,b) ((unsigned long)(((unsigned char)(r)|((unsigned short)((unsigned char)(g)) <<8))|(((unsigned long)(unsigned char)(b)) <<16)))
#define RGB565(r,g,b) (r>>3)<<11 | (g>>2)<<5 | b>>3

static int OpenMemmap()
{
	phys_mem_fd = open("/dev/mem", O_RDWR | O_SYNC);
	if(phys_mem_fd < 0)
	{
		std::cerr << "Cannot open memory device." << std::endl;
		return 0;
	}
}

static void CloseMemmap()
{
	close(phys_mem_fd);
}

static unsigned long* getMmapAddr(unsigned int physAddr, size_t size)
{
	void *addr;

	addr = mmap((void *) 0, size, PROT_READ | PROT_WRITE, MAP_SHARED, phys_mem_fd, physAddr);
	if((long) addr == (long) -1)
	{
		CloseMemmap();
		return NULL;
	}

	return (unsigned long*)addr;
}

static void unMmapAddr(void *start, size_t length)
{
	munmap(start, length);
}

static void LCD_OFF_SetPixelIndex(int x, int y, int PixelIndex);
static unsigned int LCD_OFF_GetPixelIndex(int x, int y);

void SetLcdFlip(unsigned char flip)
{
	struct fb_var_screeninfo vinfo;
	if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo))
	{
		close(fbfd);
		std::cout<<"Error reading variable information."<<std::endl<<std::flush;
		exit(1);
	}
	if(flip)
	{
		vinfo.rotate = FB_ROTATE_UD;
	}
	else
	{
		vinfo.rotate = FB_ROTATE_UR;
	}
	if(ioctl(fbfd, FBIOPUT_VSCREENINFO, &vinfo))
	{
		close(fbfd);
		std::cout<<"Error writing variable information."<<std::endl<<std::flush;
		exit(1);
	}
	DisplayFlip = flip;
}

unsigned char GetLcdFlip()
{
	return DisplayFlip;
}

/*********************************************************************
 *
 *       Static functions
 *
 **********************************************************************
 */

/*********************************************
 *
 *       Draw Bitmap 1 BPP
 *
 **********************************************
 */

static void  _DrawBitLine1BPP(int x, int y, U8 const*p, int Diff, int xsize, const LCD_PIXELINDEX*pTrans) {
	LCD_PIXELINDEX Index0 = *(pTrans+0);
	LCD_PIXELINDEX Index1 = *(pTrans+1);
	x += Diff;
	switch (GUI_Context.DrawMode & (LCD_DRAWMODE_TRANS | LCD_DRAWMODE_XOR)) {
		case 0:
			do {
				LCD_L0_SetPixelIndex(x++, y, (*p & (0x80 >> Diff)) ? Index1 : Index0);
				if (++Diff == 8) {
					Diff = 0;
					p++;
				}
			} while (--xsize);
			break;
		case LCD_DRAWMODE_TRANS:
			do {
				if (*p & (0x80 >> Diff))
					LCD_L0_SetPixelIndex(x, y, Index1);
				x++;
				if (++Diff == 8) {
					Diff = 0;
					p++;
				}
			} while (--xsize);
			break;
		case LCD_DRAWMODE_XOR:
				      do {
					      if (*p & (0x80 >> Diff)) {
						      int Pixel = LCD_L0_GetPixelIndex(x, y);
						      LCD_L0_SetPixelIndex(x, y, LCD_NUM_COLORS - 1 - Pixel);
					      }
					      x++;
					      if (++Diff == 8) {
						      Diff = 0;
						      p++;
					      }
				      } while (--xsize);
				      break;
	}
}

/*********************************************
 *
 *       Draw Bitmap 2 BPP
 *
 **********************************************
 */

#if (LCD_MAX_LOG_COLORS > 2)
static void  _DrawBitLine2BPP(int x, int y, U8 const * p, int Diff, int xsize, const LCD_PIXELINDEX * pTrans) {
	LCD_PIXELINDEX Pixels = *p;
	int CurrentPixel = Diff;
	x += Diff;
	switch (GUI_Context.DrawMode & (LCD_DRAWMODE_TRANS | LCD_DRAWMODE_XOR)) {
		case 0:
			do {
				int Shift = (3 - CurrentPixel) << 1;
				int Index = (Pixels & (0xC0 >> (6 - Shift))) >> Shift;
				LCD_PIXELINDEX PixelIndex = *(pTrans + Index);
				LCD_L0_SetPixelIndex(x++, y, PixelIndex);
				if (++CurrentPixel == 4) {
					CurrentPixel = 0;
					Pixels = *(++p);
				}
			} while (--xsize);
			break;
		case LCD_DRAWMODE_TRANS:
			do {
				int Shift = (3 - CurrentPixel) << 1;
				int Index = (Pixels & (0xC0 >> (6 - Shift))) >> Shift;
				if (Index) {
					LCD_PIXELINDEX PixelIndex = *(pTrans + Index);
					LCD_L0_SetPixelIndex(x, y, PixelIndex);
				}
				x++;
				if (++CurrentPixel == 4) {
					CurrentPixel = 0;
					Pixels = *(++p);
				}
			} while (--xsize);
			break;
	}
}
#endif

/*********************************************
 *
 *       Draw Bitmap 4 BPP
 *
 **********************************************
 */

#if (LCD_MAX_LOG_COLORS > 4)
static void  _DrawBitLine4BPP(int x, int y, U8 const * p, int Diff, int xsize, const LCD_PIXELINDEX * pTrans) {
	LCD_PIXELINDEX Pixels = *p;
	int CurrentPixel = Diff;
	switch (GUI_Context.DrawMode & (LCD_DRAWMODE_TRANS | LCD_DRAWMODE_XOR)) {
		case 0:
			do {
				int Shift = (1 - CurrentPixel) << 2;
				int Index = (Pixels & (0xF0 >> (4 - Shift))) >> Shift;
				LCD_PIXELINDEX PixelIndex = *(pTrans + Index);
				LCD_L0_SetPixelIndex(x++, y, PixelIndex);
				if (++CurrentPixel == 2) {
					CurrentPixel = 0;
					Pixels = *(++p);
				}
			} while (--xsize);
			break;
		case LCD_DRAWMODE_TRANS:
			do {
				int Shift = (1 - CurrentPixel) << 2;
				int Index = (Pixels & (0xF0 >> (4 - Shift))) >> Shift;
				if (Index) {
					LCD_PIXELINDEX PixelIndex = *(pTrans + Index);
					LCD_L0_SetPixelIndex(x, y, PixelIndex);
				}
				x++;
				if (++CurrentPixel == 2) {
					CurrentPixel = 0;
					Pixels = *(++p);
				}
			} while (--xsize);
			break;
	}
}
#endif

/*********************************************
 *
 *       Draw Bitmap 8 BPP
 *
 **********************************************
 */

#if (LCD_MAX_LOG_COLORS > 16)
static void  _DrawBitLine8BPP(int x, int y, U8 const * p, int xsize, const LCD_PIXELINDEX * pTrans) {
	LCD_PIXELINDEX Pixel;
	switch (GUI_Context.DrawMode & (LCD_DRAWMODE_TRANS | LCD_DRAWMODE_XOR)) {
		case 0:
			if (pTrans) {
				for (; xsize > 0; xsize--, x++, p++) {
					Pixel = *p;
					LCD_L0_SetPixelIndex(x, y, *(pTrans + Pixel));
				}
			} else {
				for (; xsize > 0; xsize--, x++, p++) {
					LCD_L0_SetPixelIndex(x, y, *p);
				}
			}
			break;
		case LCD_DRAWMODE_TRANS:
			if (pTrans) {
				for (; xsize > 0; xsize--, x++, p++) {
					Pixel = *p;
					if (Pixel) {
						LCD_L0_SetPixelIndex(x, y, *(pTrans + Pixel));
					}
				}
			} else {
				for (; xsize > 0; xsize--, x++, p++) {
					Pixel = *p;
					if (Pixel) {
						LCD_L0_SetPixelIndex(x, y, Pixel);
					}
				}
			}
			break;
	}
}
#endif

/*********************************************
 *
 *       Draw Bitmap 16 BPP
 *
 **********************************************
 */

#if (LCD_BITSPERPIXEL > 8)
static void  DrawBitLine16BPP(int x, int y, U16 const * p, int xsize, const LCD_PIXELINDEX * pTrans) {
	LCD_PIXELINDEX pixel;
	if ((GUI_Context.DrawMode & LCD_DRAWMODE_TRANS) == 0) {
		if (pTrans) {
			for (; xsize > 0; xsize--, x++, p++) {
				pixel = *p;
				//LCD_L0_SetPixelIndex(x, y, *(pTrans + pixel));
				LCD_OFF_SetPixelIndex(x, y, *(pTrans + pixel));
			}
		} else {
			for (;xsize > 0; xsize--, x++, p++) {
				//LCD_L0_SetPixelIndex(x, y, *p);
				LCD_OFF_SetPixelIndex(x, y, *p);
			}
		}
	} else {
		if (pTrans) {
			for (; xsize > 0; xsize--, x++, p++) {
				pixel = *p;
				if (pixel) {
					//LCD_L0_SetPixelIndex(x, y, *(pTrans + pixel));
					LCD_OFF_SetPixelIndex(x, y, *(pTrans + pixel));
				}
			}
		} else {
			for (; xsize > 0; xsize--, x++, p++) {
				pixel = *p;
				if (pixel) {
					//LCD_L0_SetPixelIndex(x, y, pixel);
					LCD_OFF_SetPixelIndex(x, y, pixel);
				}
			}
		}
	}
}
#endif

/*********************************************************************
 *
 *       Exported functions
 *
 **********************************************************************
 */

/*********************************************
 *
 *       LCD_L0_SetPixelIndex
 *
 **********************************************
Purpose:
Sets the index of the given pixel. The upper layers of emWin
calling this routine make sure that the coordinates are in range, so
that no check on the parameters needs to be performed.
 */

void LCD_L0_SetPixelIndex(int x, int y, int PixelIndex)
{
#ifdef D2DUPDATE
	LCD_OFF_SetPixelIndex(x,y,PixelIndex);
	return;
#else
	unsigned long location = 0;
	if(x<0 || x>799 || y<0 || y>599)
	{
		return;
	}
	location = x * (bits_per_pixel / 8)  + y * xres * (bits_per_pixel / 8);
	*(unsigned long*)(pFrameBuffer + location) = RGB((((unsigned short)PixelIndex)&0xF800)>>8,((unsigned short)PixelIndex&0x07E0)>>3,((unsigned short)PixelIndex&0x001F)<<3);
	return;
#endif
}

static void LCD_OFF_SetPixelIndex(int x, int y, int PixelIndex)
{
	unsigned long location = 0;
	unsigned short h,l;
	if(x<0 || x>799 || y<0 || y>599)
	{
		return;
	}
	location = x + y*xres;
	h = PixelIndex&0x0000F800;
	l = PixelIndex&0x0000001F;
	h >>= 11;
	l <<= 11;
	PixelIndex &= 0x000007E0;
	PixelIndex = PixelIndex|h|l;
	*(off_screen_buffer + location) = PixelIndex;
	return;
}

/*********************************************
 *
 *       LCD_L0_GetPixelIndex
 *
 **********************************************
Purpose:
Returns the index of the given pixel. The upper layers of emWin
calling this routine make sure that the coordinates are in range, so
that no check on the parameters needs to be performed.
 */

unsigned int LCD_L0_GetPixelIndex(int x, int y)
{
#ifdef D2DUPDATE
	return LCD_OFF_GetPixelIndex(x,y);
#else
	LCD_PIXELINDEX PixelIndex;
	unsigned long location = 0;
	unsigned int r,g,b;
	if(x<0 || x>799 || y<0 || y>599)
	{
		return 0;
	}
	location = x * (bits_per_pixel /8)  + y * xres * (bits_per_pixel / 8);
	b = (*(unsigned long*)(pFrameBuffer + location)&0x00FF0000)>>16;
	g = (*(unsigned long*)(pFrameBuffer + location)&0x0000FF00)>>8;
	r = *(unsigned long*)(pFrameBuffer + location)&0x000000FF;
	PixelIndex = RGB565(r,g,b);
	return PixelIndex;
#endif
}

static unsigned int LCD_OFF_GetPixelIndex(int x, int y)
{
	LCD_PIXELINDEX PixelIndex;
	unsigned long location = 0;
	unsigned int r,g,b;
	if(x<0 || x>799 || y<0 || y>599)
	{
		return 0;
	}
	location = x + y*xres;
	PixelIndex = *(off_screen_buffer+location);
	return PixelIndex;
}

/*********************************************
 *
 *       LCD_L0_XorPixel
 *
 **********************************************
 */

void LCD_L0_XorPixel(int x, int y) {
	LCD_PIXELINDEX PixelIndex = LCD_L0_GetPixelIndex(x, y);
	LCD_L0_SetPixelIndex(x, y, LCD_NUM_COLORS - PixelIndex - 1);
}

/*********************************************
 *
 *       LCD_L0_DrawHLine
 *
 **********************************************
 */

void LCD_L0_DrawHLine  (int x0, int y,  int x1) {
	if (GUI_Context.DrawMode & LCD_DRAWMODE_XOR) {
		for (; x0 <= x1; x0++) {
			LCD_L0_XorPixel(x0, y);
		}
	} else {
		for (; x0 <= x1; x0++) {
			LCD_L0_SetPixelIndex(x0, y, LCD_COLORINDEX);
		}
	}
}

/*********************************************
 *
 *       LCD_L0_DrawVLine
 *
 **********************************************
 */

void LCD_L0_DrawVLine  (int x, int y0,  int y1) {
	if (GUI_Context.DrawMode & LCD_DRAWMODE_XOR) {
		for (; y0 <= y1; y0++) {
			LCD_L0_XorPixel(x, y0);
		}
	} else {
		for (; y0 <= y1; y0++) {
			LCD_L0_SetPixelIndex(x, y0, LCD_COLORINDEX);
		}
	}
}

/*********************************************
 *
 *       LCD_L0_FillRect
 *
 **********************************************
 */

static void DrawRect(int x0, int y0, int x1, int y1, int PixelIndex)
{
	unsigned long color;
	unsigned int id;
	*(D2D + (0x08>>2)) =0x3;
	id=*(D2D+(0x08>>2));
	while((id&0x02)!=0)
		id=*(D2D+(0x08>>2));
	color = RGB((((unsigned short)PixelIndex)&0xF800)>>8,((unsigned short)PixelIndex&0x07E0)>>3,((unsigned short)PixelIndex&0x001F)<<3);
	*(D2D+(0x14>>2)) = color;
	*(D2D+(0x18>>2)) = (y0<<16)|x0;
	*(D2D+(0x1C>>2)) = (y1<<16)|x1;
	*(D2D+(0x20>>2)) = (y0<<16)|x0;
	*(D2D+(0x24>>2)) = (y1<<16)|x1;
	*(D2D+(0x40>>2)) = 0x53E2B000;
	*(D2D+(0x44>>2)) = 800*4;
	*(D2D+(0x4c>>2)) = 0x03;

	*(D2D + (0x08>>2)) = 0x1;
	*(D2D + (0x10>>2)) = 0x2;	//draw rect

	id=*(D2D+(0x84>>2));
	while((id&0x1)!=0x1)
		id=*(D2D+(0x84>>2));	//check busy status
}

static void DrawRectOff(int x0, int y0, int x1, int y1, int PixelIndex)
{
	unsigned long color;
	unsigned int id;
	*(D2D + (0x08>>2)) =0x3;
	id=*(D2D+(0x08>>2));
	while((id&0x02)!=0)
		id=*(D2D+(0x08>>2));
	color = RGB((((unsigned short)PixelIndex)&0xF800)>>8,((unsigned short)PixelIndex&0x07E0)>>3,((unsigned short)PixelIndex&0x001F)<<3);
	*(D2D+(0x14>>2)) = color;
	*(D2D+(0x18>>2)) = (y0<<16)|x0;
	*(D2D+(0x1C>>2)) = (y1<<16)|x1;
	*(D2D+(0x20>>2)) = (y0<<16)|x0;
	*(D2D+(0x24>>2)) = (y1<<16)|x1;
	*(D2D+(0x40>>2)) = 0x532F6000;
	*(D2D+(0x44>>2)) = 800*4;
	*(D2D+(0x4C>>2)) = 0x03;

	*(D2D + (0x08>>2)) = 0x1;
	*(D2D + (0x10>>2)) = 0x2;	//draw rect

	id=*(D2D+(0x84>>2));
	while((id&0x1)!=0x1)
		id=*(D2D+(0x84>>2));	//check busy status
}

void LCD_L0_FillRect(int x0, int y0, int x1, int y1)
{
#ifdef D2DUPDATE
	if (GUI_Context.DrawMode & LCD_DRAWMODE_XOR)
	{
		for (; y0 <= y1; y0++)
		{
			LCD_L0_DrawHLine(x0, y0, x1);
		}
	}
	else
	{
		DrawRectOff(x0,y0,x1,y1,LCD_COLORINDEX);
	}
#else
	if (GUI_Context.DrawMode & LCD_DRAWMODE_XOR)
	{
		for (; y0 <= y1; y0++)
		{
			LCD_L0_DrawHLine(x0, y0, x1);
		}
	}
	else
	{
		DrawRect(x0,y0,x1,y1,LCD_COLORINDEX);
	}
#endif
}

/*********************************************
 *
 *       LCD_L0_DrawBitmap
 *
 **********************************************
 */

void D2D_Bitblt(int x0, int y0, int width, int heigh)
{
	int x1,y1;
	x1 = x0+width;
	y1 = y0+heigh;
	unsigned int id;
	*(D2D + (0x08>>2)) =0x3;
	id=*(D2D+(0x08>>2));
	while((id&0x02)!=0)
		id=*(D2D+(0x08>>2));

	*(D2D+(0x14>>2)) = 0xFF000000;
	*(D2D+(0x20>>2)) = (y0<<16)|x0;
	*(D2D+(0x24>>2)) = (y1<<16)|x1;
	*(D2D+(0x30>>2)) = 0x532F6000+(800*y0+x0)*2;
	*(D2D+(0x34>>2)) = 800*2;
	*(D2D+(0x38>>2)) = (heigh<<16)|width;
	*(D2D+(0x3C>>2)) = 0x4;
	*(D2D+(0x40>>2)) = 0x53E2B000+(800*y0+x0)*4;
	*(D2D+(0x44>>2)) = 800*4;
	*(D2D+(0x4C>>2)) = 0x3;
	*(D2D+(0x50>>2)) = 0;
	*(D2D+(0x54>>2)) = 0;
	*(D2D+(0x58>>2)) = 0;
	*(D2D+(0x5C>>2)) = 0;

	*(D2D+(0x08>>2)) = 0x1;

	*(D2D+(0x10>>2)) = (0xAA<<8) | 8;

	id=*(D2D+(0x84>>2));
	while((id&0x01)!=0x1)
		id=*(D2D+(0x84>>2));
}

void LCD_L0_DrawBitmap(int x0, int y0,
		int xsize, int ysize,
		int BitsPerPixel, 
		int BytesPerLine,
		const U8* pData, int Diff,
		const LCD_PIXELINDEX* pTrans)
{
	int i;
	/* Use _DrawBitLineXBPP */
	for (i=0; i<ysize; i++) {
		switch (BitsPerPixel) {
			case 1:
				_DrawBitLine1BPP(x0, i + y0, pData, Diff, xsize, pTrans);
				break;
#if (LCD_MAX_LOG_COLORS > 2)
			case 2:
				_DrawBitLine2BPP(x0, i + y0, pData, Diff, xsize, pTrans);
				break;
#endif
#if (LCD_MAX_LOG_COLORS > 4)
			case 4:
				_DrawBitLine4BPP(x0, i + y0, pData, Diff, xsize, pTrans);
				break;
#endif
#if (LCD_MAX_LOG_COLORS > 16)
			case 8:
				_DrawBitLine8BPP(x0, i + y0, pData, xsize, pTrans);
				break;
#endif
#if (LCD_BITSPERPIXEL > 8)
			case 16:
				DrawBitLine16BPP(x0, i + y0, (const U16 *)pData, xsize, pTrans);
				break;
#endif
		}
		pData += BytesPerLine;
	}
	//D2D_Bitblt(x0,y0,xsize,ysize);
}

/*********************************************
 *
 *       LCD_L0_SetOrg
 *
 **********************************************
 */

void LCD_L0_SetOrg(int x, int y) {
	GUI_USE_PARA(x);
	GUI_USE_PARA(y);
}

/*********************************************
 *
 *       LCD_On / LCD_Off
 *
 **********************************************
 */

void LCD_On (void) {
#ifdef LCD_ON
	LCD_ON();
#endif
}

void LCD_Off (void) {
#ifdef LCD_OFF
	LCD_OFF();
#endif
}

/*********************************************
 *
 *       LCD_L0_Init
 *
 **********************************************
Purpose:
Initialises the LCD-controller.
 */

int LCD_L0_Init(void)
{
	unsigned int id;
	struct fb_var_screeninfo vinfo;
	struct fb_fix_screeninfo finfo;
	DisplayFlip = 0;
	fbfd = 0;
	screensize = 0;

	/*打开framebuffer 设备文件*/
	fbfd = open("/dev/fb0", O_RDWR);
	if(!fbfd)
	{
		std::cout<<"Error: cannot open framebuffer device."<<std::endl<<std::flush;
		exit(1);
	}
	/*读取framebuffer设备的固定信息*/
	if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo))
	{
		close(fbfd);
		std::cout<<"Error reading fixed information."<<std::endl<<std::flush;
		exit (1);
	}
	/*读取framebuffer设备的可变信息*/
	if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo))
	{
		close(fbfd);
		std::cout<<"Error reading variable information."<<std::endl<<std::flush;
		exit(1);
	}
	/*保存信息给画点函数使用*/
	xres = vinfo.xres;
	yres = vinfo.yres;
	bits_per_pixel = vinfo.bits_per_pixel;

	/*mmap到内存中然后直接操作内存后sync到设备文件里*/
	screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;
	pFrameBuffer = (unsigned char*)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
	if ((int)pFrameBuffer == -1)
	{
		close(fbfd);
		std::cout<<"Error: failed to map framebuffer device to memory."<<std::endl<<std::flush;
		exit(4);
	}
	// D2D engine
	D2D = (unsigned long*)mapm(0x08003000, 0x1000);
	id = *D2D;
	*(D2D+ (0x08>>2))=0x3;
	id = *(D2D+(0x08>>2));
	while((id&0x2)!=0)
		id=*(D2D + (0x08>>2));
	OpenMemmap();
	off_screen_buffer = (unsigned short*)getMmapAddr(0x532F6000, 800*2*600);
	return 0;
}

int LCD_L0_Exit(void)
{
	unmapm((void*)D2D, 0x1000);
	unMmapAddr(off_screen_buffer, 800*2*600);
	CloseMemmap();
	munmap(pFrameBuffer, screensize);
	close(fbfd);
}

/*********************************************
 *
 *       LCD_L0_SetLUTEntry
 *
 **********************************************
 */

void LCD_L0_SetLUTEntry(U8 Pos, LCD_COLOR Color) {
	GUI_USE_PARA(Pos);
	GUI_USE_PARA(Color);
}

void * LCD_L0_GetDevFunc(int Index) {
  GUI_USE_PARA(Index);
  return NULL;
}


#else

void LCDDummy(void) { } /* avoid empty object files */

#endif /* (LCD_CONTROLLER undefined) */

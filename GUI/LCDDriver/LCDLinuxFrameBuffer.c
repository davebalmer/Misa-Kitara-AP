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

#include "LCD_Private.h"      /* private modul definitions & config */
#include "GUI_Private.h"
#include "GUIDebug.h"


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

#define USE_FRAMEBUFFER0

static int fbfd = 0;
static unsigned char* pFrameBuffer = 0;
static int xres = 0;
static int yres = 0;
static int bits_per_pixel = 0;
static unsigned long screensize = 0;
static unsigned char DisplayFlip = 0;

#define RGB(r,g,b) ((unsigned long)(((unsigned char)(r)|((unsigned short)((unsigned char)(g)) <<8))|(((unsigned long)(unsigned char)(b)) <<16)))
#define RGB565(r,g,b) (r>>3)<<11 | (g>>2)<<5 | b>>3

void SetLcdFrameBuffer(unsigned long* pFrame)
{
#ifndef USE_FRAMEBUFFER0
	pFrameBuffer = (unsigned char*)pFrame;
#endif // USE_FRAMEBUFFER0
}

void SetLcdFlip(unsigned char flip)
{
#ifdef USE_FRAMEBUFFER0
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
#else // USE_FRAMEBUFFER0
	struct fb_var_screeninfo vinfo;
	fbfd = 0;
	if(DisplayFlip != flip)
	{
		fbfd = open("/dev/fb0", O_RDWR);
		if(!fbfd)
		{
			std::cout<<"Error: cannot open framebuffer device."<<std::endl<<std::flush;
			exit(1);
		}
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
		close(fbfd);
	}
#endif // USE_FRAMEBUFFER0
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
				LCD_L0_SetPixelIndex(x, y, *(pTrans + pixel));
			}
		} else {
			for (;xsize > 0; xsize--, x++, p++) {
				LCD_L0_SetPixelIndex(x, y, *p);
			}
		}
	} else {
		if (pTrans) {
			for (; xsize > 0; xsize--, x++, p++) {
				pixel = *p;
				if (pixel) {
					LCD_L0_SetPixelIndex(x, y, *(pTrans + pixel));
				}
			}
		} else {
			for (; xsize > 0; xsize--, x++, p++) {
				pixel = *p;
				if (pixel) {
					LCD_L0_SetPixelIndex(x, y, pixel);
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
	unsigned long location = 0;
	if(x<0 || x>799 || y<0 || y>599)
	{
		return;
	}
	location = x * (bits_per_pixel / 8)  + y * xres * (bits_per_pixel / 8);
	*(unsigned long*)(pFrameBuffer + location) = RGB((((unsigned short)PixelIndex)&0xF800)>>8,((unsigned short)PixelIndex&0x07E0)>>3,((unsigned short)PixelIndex&0x001F)<<3);
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

void LCD_L0_FillRect(int x0, int y0, int x1, int y1) {
	for (; y0 <= y1; y0++) {
		LCD_L0_DrawHLine(x0, y0, x1);
	}
}

/*********************************************
 *
 *       LCD_L0_DrawBitmap
 *
 **********************************************
 */

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
#ifdef USE_FRAMEBUFFER0
	struct fb_var_screeninfo vinfo;
	struct fb_fix_screeninfo finfo;
	DisplayFlip = 0;
	fbfd = 0;
	screensize = 0;

	std::cout<<"LCD_L0_Init..."<<std::endl<<std::flush;
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
	vinfo.rotate = FB_ROTATE_UD;
	if(ioctl(fbfd, FBIOPUT_VSCREENINFO, &vinfo))
	{
		close(fbfd);
		std::cout<<"Error writing variable information."<<std::endl<<std::flush;
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
	std::cout<<"The framebuffer device was mapped to memory successfully."<<std::endl<<std::flush;
	std::cout<<"xres="<<xres<<",yres="<<yres<<std::endl<<"bit_per_pixel="<<bits_per_pixel<<",screensize="<<screensize<<std::endl<<std::flush;
	/*都正确了返回0*/
#else // USE_FRAMEBUFFER0
	DisplayFlip = 0;
	xres = 800;
	yres = 600;
	bits_per_pixel = 32;

#endif // USE_FRAMEBUFFER0
	return 0;
}

int LCD_L0_Exit(void)
{
#ifdef USE_FRAMEBUFFER0
	munmap(pFrameBuffer, screensize);
	close(fbfd);
#endif // USE_FRAMEBUFFER0
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

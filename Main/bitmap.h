#ifndef _BITMAP_H_
#define _BITMAP_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include "type.h"

/* bitmap file identifier 'BM'*/
#define BITMAP_FORMAT_ID	0x4d42

/* bitmap file header */
#pragma pack(1)
typedef struct _BitmapFileHeader {
    UINT16	bfType;
    UINT32	bfSize;
    UINT16	bfReserved1;
    UINT16	bfReserved2;
    UINT32	bfOffBits;
} BitmapFileHeader, *BitmapFileHeaderPtr;
/*#pragma pack()*/

/* bitmap information header */
typedef struct _BitmapInfoHeader {
    UINT32	biSize;
    UINT32	biWidth;
    UINT32	biHeight;
    UINT16	biPlanes;
    UINT16	biBitCount;
    UINT32	biCompression;
    UINT32	biSizeImage;
    UINT32	biXPelsPerMeter;
    UINT32	biYPelSPerMeter;
    UINT32	biClrUsed;
    UINT32	biClrImportant;
} BitmapInfoHeader, *BitmapInfoHeaderPtr;

/* bitmap RGB color for 1/2/4/8 bpp mode */
typedef struct _RGBQuad {
    UINT8	blue;
    UINT8	green;
    UINT8	red;
    UINT8	reserved;
} RGBQuad, *RGBQuadPtr;
#pragma pack()

/****************************************************************
 * parameter [in]: path, bitmap file name, including path	*
 * parameter [in]: pFileHeader, bitmap file header structure	*
 * parameter [in]: pInforHeader, bitmap information structure	*
 * return value: a file stream pointer if succeeds, or NULL	*
 * this function open a bitmap file and read the header infor-	*
 * mation into the two given structures, this is the first func-*
 * tion to call before decoding.
 * **************************************************************/

FILE *BitmapInit (char *path,
		  BitmapFileHeaderPtr pFileHeader,
		  BitmapInfoHeaderPtr pInfoHeader);

/****************************************************************
 * parameter [in]: f, file stream pointer of a open file	*
 * return value: 0 if succeed, or non-zero			*
 * this function close an open file, this is the last function	*
 * to call after decoding					*
 * **************************************************************/

INT32 BitmapExit (FILE *f);

/****************************************************************
 * parameter [in]: BitmapFileHeader structure pointer		*
 * this function returns the file size of a bitmap file		*
 * **************************************************************/

UINT32 BitmapGetFileSize (BitmapFileHeaderPtr pFileHeader);

/****************************************************************
 * parameter [in]: BitmapInfoHeader structure pointer		*
 * this function returns the image data  size of a bitmap file	*
 * **************************************************************/

UINT32 BitmapGetImageSize (BitmapInfoHeaderPtr pInfoHeader);

/****************************************************************
 * parameter [in]: BitmapInfoHeader structure pointer		*
 * this function returns the image width			*
 * **************************************************************/

UINT32 BitmapGetImageWidth (BitmapInfoHeaderPtr pInfoHeader);

/****************************************************************
 * parameter [in]: BitmapInfoHeader structure pointer		*
 * this function returns the image height			*
 * **************************************************************/

UINT32 BitmapGetImageHeight (BitmapInfoHeaderPtr pInfoHeader);

/****************************************************************
 * parameter [in]: BitmapInfoHeader structure pointer		*
 * this function returns the bits per pixel of the image	*
 * **************************************************************/

UINT16 BitmapGetImageBitCount (BitmapInfoHeaderPtr pInfoHeader);

/****************************************************************
 * parameter [in]: BitmapInfoHeader structure pointer		*
 * this function returns the offset image color data from the 	*
 * beginning of the bitmap file					*
 * **************************************************************/

UINT32 BitmapGetImageOffset (BitmapFileHeaderPtr pFileHeader);

/****************************************************************
 * parameter [in]: f, file stream pointer			*
 * parameter [in]: BitmapFileHeader structure pointer		*
 * parameter [in]: BitmapInfoHeader structure pointer		*
 * parameter [in]: destination address to store the decoded data*
 * parameter [in]: destination color format, bits per pixel	*
 * **************************************************************/

void BitmapDecodeImage (FILE *f,
			BitmapFileHeaderPtr pFileHeader,
			BitmapInfoHeaderPtr pInfoHeader,
			void *dst,
			UINT16 dstStride,
			UINT16 dstBpp);

/****************************************************************
 * parameter [in]: f, file stream pointer			*
 * parameter [in]: BitmapFileHeader structure pointer		*
 * parameter [in]: BitmapInfoHeader structure pointer		*
 * parameter [in]: destination color format, bits per pixel	*
 * **************************************************************/

void BitmapDecodeImageRaw (FILE *f,
			BitmapFileHeaderPtr pFileHeader,
			BitmapInfoHeaderPtr pInfoHeader,
			void *dst,
			UINT16 dstBpp);

/****************************************************************
 * parameter [in]: f, file stream pointer returned by BitmapInit*
 * parameter [in]: rgbQuad[], an RGBQuad structure pointer array*
 * parameter [in]: nRGB, number of colors in an image		*
 * **************************************************************/

void BitmapGetImagePalette (FILE *f,
			    RGBQuadPtr rgbQuad,
			    INT16 nRGB);

#ifdef __cplusplus
}
#endif

#endif


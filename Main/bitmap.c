#include "bitmap.h"
#include <memory.h>
#include <stdlib.h>

FILE *BitmapInit (char *path,
		  BitmapFileHeaderPtr pFileHeader,
		  BitmapInfoHeaderPtr pInfoHeader)
{
    FILE	*f;
    UINT32	fileHeaderSize;
    UINT32	infoHeaderSize;
    
    fileHeaderSize = sizeof (BitmapFileHeader);
    infoHeaderSize = sizeof (BitmapInfoHeader);

    if (!(f = fopen (path, "rb")))
    {
		printf ("could not open %s\n", path);
		return NULL;
    }

    fread(&(pFileHeader->bfType), 2, 1, f);
    fread(&(pFileHeader->bfSize), 4, 1, f);
    fread(&(pFileHeader->bfReserved1), 2, 1, f);
    fread(&(pFileHeader->bfReserved2), 2, 1, f);
    fread(&(pFileHeader->bfOffBits), 4, 1, f);

    fread(&(pInfoHeader->biSize), 4, 1, f);
    fread(&(pInfoHeader->biWidth), 4, 1, f);
    fread(&(pInfoHeader->biHeight), 4, 1, f);
    fread(&(pInfoHeader->biPlanes), 2, 1, f);
    fread(&(pInfoHeader->biBitCount), 2, 1, f);
    fread(&(pInfoHeader->biCompression), 4, 1, f);
    fread(&(pInfoHeader->biSizeImage), 4, 1, f);
    fread(&(pInfoHeader->biXPelsPerMeter), 4, 1, f);
    fread(&(pInfoHeader->biYPelSPerMeter), 4, 1, f);
    fread(&(pInfoHeader->biClrUsed), 4, 1, f);
    fread(&(pInfoHeader->biClrImportant), 4, 1, f);

/*    if (fread (pFileHeader, 1, fileHeaderSize, f) < fileHeaderSize)
    {
		printf ("could not get bitmap file header\n");
		fclose (f);
		return NULL;
    }
    if (fread (pInfoHeader, 1, infoHeaderSize, f) < infoHeaderSize)
    {
		printf ("could not get bitmap info header\n");
		fclose (f);
		return NULL;
    }*/
    if (pFileHeader->bfType != BITMAP_FORMAT_ID)
    {
		printf ("%s is not a bitmap picture\n", path);
		fclose (f);
		return NULL;
    }

    return f;
}

INT32 BitmapExit (FILE *f)
{
    if (f)
    {
		return fclose (f);
    }
    else
    {
		return -1;
    }
}

UINT32 BitmapGetFileSize (BitmapFileHeaderPtr pFileHeader)
{
    return pFileHeader->bfSize;
}

UINT32 BitmapGetImageSize (BitmapInfoHeaderPtr pInfoHeader)
{
    return pInfoHeader->biSizeImage;
}

UINT32 BitmapGetImageWidth (BitmapInfoHeaderPtr pInfoHeader)
{
    return pInfoHeader->biWidth;
}

UINT32 BitmapGetImageHeight (BitmapInfoHeaderPtr pInfoHeader)
{
    return pInfoHeader->biHeight;
}

UINT16 BitmapGetImageBitCount (BitmapInfoHeaderPtr pInfoHeader)
{
    return pInfoHeader->biBitCount;
}

UINT32 BitmapGetImageOffset (BitmapFileHeaderPtr pFileHeader)
{
    return pFileHeader->bfOffBits;
}

#define SWAPWORD(x)	((((x) & 0x0000ffff) << 16) | (((x) & 0xffff0000) >> 16))

void BitmapDecodeImage (FILE *f,
			BitmapFileHeaderPtr pFileHeader,
			BitmapInfoHeaderPtr pInfoHeader,
			void *dst,
			UINT16 dstStride,
			UINT16 dstBpp)
{
    UINT32	width, height;
    UINT16	bpp;
    INT32	byteStride, imageByteWidth;
    INT32	i;
    CARD8	*buffer;
    CARD8	*dstByte, *dstByteLine;
    UINT16	*dstHalf, *dstHalfLine;
    UINT32	*dstWord, *dstWordLine;
    UINT32	pixel;
    UINT32	offset, seekOff;

    width = pInfoHeader->biWidth;
    height = pInfoHeader->biHeight;
    bpp = pInfoHeader->biBitCount;
    offset = pFileHeader->bfOffBits;

    byteStride = ((width * bpp + 31) >> 5) << 2;
    imageByteWidth = (width * bpp) >> 3;
    seekOff = byteStride - imageByteWidth;

    {
		dstByteLine = (CARD8 *) dst;
		/* move to the last line */
		dstByteLine += (dstStride * (height - 1));
		fseek (f, offset, SEEK_SET);
		while (height--)
		{
	    	dstByte = dstByteLine;
	    	dstByteLine -= dstStride;
	    	fread (dstByte, 1, imageByteWidth, f);
			fseek (f, seekOff, SEEK_CUR);
		}
    }
}

void BitmapDecodeImageRaw (FILE *f,
			BitmapFileHeaderPtr pFileHeader,
			BitmapInfoHeaderPtr pInfoHeader,
			void *dst,
			UINT16 dstBpp)
{
    UINT32	width, height;
    UINT16	bpp;
    INT32	byteStride, imageByteWidth;
    INT32	i;
    CARD8	*buffer;
    CARD8	*dstByte, *dstByteLine;
    UINT16	*dstHalf, *dstHalfLine;
    UINT32	*dstWord, *dstWordLine;
    UINT32	pixel;
    UINT32	offset, seekOff;

    width = pInfoHeader->biWidth;
    height = pInfoHeader->biHeight;
    bpp = pInfoHeader->biBitCount;
    offset = pFileHeader->bfOffBits;

	unsigned int dstStride = width * bpp/8;

    byteStride = ((width * bpp + 31) >> 5) << 2;
    imageByteWidth = (width * bpp) >> 3;
    seekOff = byteStride - imageByteWidth;

    {
		dstByteLine = (CARD8 *) dst;
		/* move to the last line */
		dstByteLine += (dstStride * (height - 1));
		fseek (f, offset, SEEK_SET);
		while (height--)
		{
	    	dstByte = dstByteLine;
	    	dstByteLine -= dstStride;
	    	fread (dstByte, 1, imageByteWidth, f);
			fseek (f, seekOff, SEEK_CUR);
		}
    }
}

void BitmapGetImagePalette (FILE *f,
			    RGBQuadPtr rgbQuad,
			    INT16 nRGB)
{
    INT32	i;
    INT32	headerSize;
    INT32	rgbQuadSize;

    headerSize = sizeof (BitmapFileHeader) + sizeof (BitmapInfoHeader);
    rgbQuadSize = sizeof (RGBQuad);

    fseek (f, headerSize, SEEK_SET);

    for (i = 0; i < nRGB; i++)
    {
		fread (&rgbQuad[i], rgbQuadSize, 1, f);
    }
}


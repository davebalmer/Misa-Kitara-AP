#ifndef __DRAW2D_COMMON_H__
#define __DRAW2D_COMMON_H__

#define SDR_CSD1_BASE	0x90000000


#define DRAW2D_IDR            (DRAW2D_BASE + 0x000)
#define DRAW2D_CAPR           (DRAW2D_BASE + 0x004)
#define DRAW2D_CTL            (DRAW2D_BASE + 0x008)

#define DRAW2D_CMD            (DRAW2D_BASE + 0x010)
#define DRAW2D_COLOR          (DRAW2D_BASE + 0x014)
#define DRAW2D_START          (DRAW2D_BASE + 0x018)
#define DRAW2D_END            (DRAW2D_BASE + 0x01C)
#define DRAW2D_CLIP_START	  (DRAW2D_BASE + 0x020)
#define DRAW2D_CLIP_END 	  (DRAW2D_BASE + 0x024)
#define DRAW2D_SRC_ADDR       (DRAW2D_BASE + 0x030)
#define DRAW2D_SRC_STRIDE     (DRAW2D_BASE + 0x034)
#define DRAW2D_SRC_DIM        (DRAW2D_BASE + 0x038)
#define DRAW2D_SRC_PIX        (DRAW2D_BASE + 0x03C)
#define DRAW2D_DEST_ADDR      (DRAW2D_BASE + 0x040)
#define DRAW2D_DEST_STRIDE    (DRAW2D_BASE + 0x044)
#define DRAW2D_DEST_DIM       (DRAW2D_BASE + 0x048)
#define DRAW2D_DEST_PIX       (DRAW2D_BASE + 0x04C)
#define DRAW2D_BRUSH_ADDR     (DRAW2D_BASE + 0x050)
#define DRAW2D_BRUSH_STRIDE   (DRAW2D_BASE + 0x054)
#define DRAW2D_BRUSH_DIM      (DRAW2D_BASE + 0x058)
#define DRAW2D_BRUSH_PIX      (DRAW2D_BASE + 0x05C)

#define DRAW2D_INTRPT_EN      (DRAW2D_BASE + 0x080)
#define DRAW2D_STA            (DRAW2D_BASE + 0x084)
#define DRAW2D_Q_LO_REG		  (DRAW2D_BASE + 0x090)
#define DRAW2D_Q_HI_REG       (DRAW2D_BASE + 0x094)
#define DRAW2D_Q_STAT_LO_REG  (DRAW2D_BASE + 0x098)
#define DRAW2D_Q_STAT_HI_REG  (DRAW2D_BASE + 0x09C)

/* Masks for DRAW2D_CAPR */
#define DRAW2D_CAPR_STR       0x00000001
#define DRAW2D_CAPR_POLY      0x00000002
#define DRAW2D_CAPR_ELLIPSE   0x00000004
#define DRAW2D_CAPR_CLUT      0x00000008
#define DRAW2D_CAPR_BPP24     0x00000010

/* Masks for DRAW2D_CFGR */
#define DRAW2D_CFGR_EN        0x00000001
#define DRAW2D_CFGR_RST       0x00000002
#define DRAW2D_CFGR_INTR      0x00000004
#define DRAW2D_CFGR_INCR      0x00000008

/* Masks for DRAW2D_STA */
#define DRAW2D_STA_DONE       0x00000001

/* Masks for DRAW2D_CMD */
#define DRAW2D_CMD_OP         0x0000000f
#define DRAW2D_CMD_OP_LINE       0x01
#define DRAW2D_CMD_OP_RECT       0x02
#define DRAW2D_CMD_OP_ELLIPSE    0x03
#define DRAW2D_CMD_OP_POLY       0x04
#define DRAW2D_CMD_OP_CLUT       0x07
#define DRAW2D_CMD_OP_BITBLT     0x08
#define DRAW2D_CMD_OP_TRANSBLT   0x09
#define DRAW2D_CMD_OP_ALPHA      0x0a
#define DRAW2D_CMD_OP_STRETCHBLT 0x0b
#define DRAW2D_CMD_ROT        0x00000010
#define DRAW2D_CMD_VFLIP      0x00000020
#define DRAW2D_CMD_HFLIP      0x00000040
#define DRAW2D_CMD_PARAM      0x0000ff00

/* Masks for DRAW2D_COLOR */
#define DRAW2D_COLOR_ALPHA    0xff000000
#define DRAW2D_COLOR_RED      0x00ff0000
#define DRAW2D_COLOR_GREEN    0x0000ff00
#define DRAW2D_COLOR_BLUE     0x000000ff

/* Masks for DRAW2D_START */
/*
#define DRAW2D_START_X        0x0000ffff
#define DRAW2D_START_Y        0xffff0000
*/
/* modified 10/11.  since start stop dimension is not always 16 bits */
unsigned int DRAW2D_START_X = 0x0000ffff;
unsigned int DRAW2D_START_Y = 0xffff0000;

/* Masks for DRAW2D_END */
/*
#define DRAW2D_END_X          0x0000ffff
#define DRAW2D_END_Y          0xffff0000
*/
unsigned int DRAW2D_END_X = 0x0000ffff;
unsigned int DRAW2D_END_Y = 0xffff0000;

/* Masks for DRAW2D_SRC_DIM */
/*
#define DRAW2D_SRC_DIM_WIDTH  0x0000ffff
#define DRAW2D_SRC_DIM_HEIGHT 0xffff0000
*/
unsigned int DRAW2D_SRC_DIM_WIDTH = 0x0000ffff;
unsigned int DRAW2D_SRC_DIM_HEIGHT = 0xffff0000;

/* Masks for DRAW2D_SRC_PIX */
#define DRAW2D_SRC_PIX_1BPP   0x0
#define DRAW2D_SRC_PIX_2BPP   0x1
#define DRAW2D_SRC_PIX_4BPP   0x2
#define DRAW2D_SRC_PIX_8BPP   0x3
#define DRAW2D_SRC_PIX_565    0x4
#define DRAW2D_SRC_PIX_1555   0x5
#define DRAW2D_SRC_PIX_24BPP  0x6
#define DRAW2D_SRC_PIX_32BPP  0x7
#define DRAW2D_SRC_PIX_332    0x8
#define DRAW2D_SRC_PIX_FORMAT 0x0000000f
#define DRAW2D_SRC_PIX_OFFSET 0x00000070

/* Masks for DRAW2D_DEST_DIM */
/*
#define DRAW2D_DEST_DIM_WIDTH  0x0000ffff
#define DRAW2D_DEST_DIM_HEIGHT 0xffff0000
*/
unsigned int DRAW2D_DEST_DIM_WIDTH = 0x0000ffff;
unsigned int DRAW2D_DEST_DIM_HEIGHT = 0xffff0000;

/* Masks for DRAW2D_DEST_PIX */
#define DRAW2D_DEST_PIX_565    0x0
#define DRAW2D_DEST_PIX_1555   0x1
#define DRAW2D_DEST_PIX_24BPP  0x2
#define DRAW2D_DEST_PIX_32BPP  0x3
#define DRAW2D_DEST_PIX_332    0x4
#define DRAW2D_DEST_PIX_FORMAT 0x00000007

/* Masks for DRAW2D_BRUSH_DIM_WIDTH */
/* 
#define DRAW2D_BRUSH_DIM_WIDTH  0x0000ffff
#define DRAW2D_BRUSH_DIM_HEIGHT 0xffff0000
*/
unsigned int DRAW2D_BRUSH_DIM_WIDTH = 0x0000ffff;
unsigned int DRAW2D_BRUSH_DIM_HEIGHT = 0xffff0000;

/* Masks for DRAW2D_BRUSH_PIX */
#define DRAW2D_BRUSH_PIX_565    0x0
#define DRAW2D_BRUSH_PIX_1555   0x1
#define DRAW2D_BRUSH_PIX_24BPP  0x2
#define DRAW2D_BRUSH_PIX_32BPP  0x3
#define DRAW2D_BRUSH_PIX_332    0x4
#define DRAW2D_BRUSH_PiX_FORMAT 0x00000007

#endif

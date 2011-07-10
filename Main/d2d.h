/**
@file			d2d.h - header file for d2d.c
@author		davidli@solomon-systech.com
@version		0.1
@date		01JUN10
@todo
*/

#ifndef _D2D_H
#define _D2D_H

#define D2D_IOC_MAGIC	'v'

#define	D2D_IOCTL_INIT	_IOW(D2D_IOC_MAGIC,1,int)
#define	D2D_IOCTL_WR	_IOW(D2D_IOC_MAGIC,2,int)
#define	D2D_IOCTL_WR8	_IOW(D2D_IOC_MAGIC,3,int)
#define	D2D_IOCTL_WR16	_IOW(D2D_IOC_MAGIC,4,int)
#define	D2D_IOCTL_RD	_IO(D2D_IOC_MAGIC,5)
#define	D2D_IOCTL_RD8	_IO(D2D_IOC_MAGIC,6)
#define	D2D_IOCTL_RD16	_IOW(D2D_IOC_MAGIC,7,int)

#define	D2D_IOCTL_STATUS _IOW(D2D_IOC_MAGIC,8,int)

#define D2D_IOCTL_START	_IOR(D2D_IOC_MAGIC,9,int)
#define	D2D_IOCTL_STOP	_IOW(D2D_IOC_MAGIC,10,int)
#define	D2D_IOCTL_ALPHA	_IOW(D2D_IOC_MAGIC,11,int)
#define	D2D_IOCTL_BLT	_IOW(D2D_IOC_MAGIC,12,int)
#define	D2D_IOCTL_TRANS	_IOW(D2D_IOC_MAGIC,13,int)
#define	D2D_IOCTL_SCALE _IOW(D2D_IOC_MAGIC,14,int)
#define	D2D_IOCTL_FILL_SCREEN _IOW(D2D_IOC_MAGIC,15,int)

#define D2D_IOC_MAXNO	15

typedef enum
{
	D2D_ERR_NONE = 0,		/**< sucessful */
	D2D_ERR_HW = -1,		/**< hardware error */
	D2D_ERR_PARM = -2,		/**< parameter error */
	D2D_ERR_CFG = -3,		/**< configuration error */
	D2D_ERR_MEM = -4,		/**< memory allocation error */
	D2D_ERR_SUPP = -5		/**< not supported err */
}
d2d_err_e;
/*
typedef struct
{
	void		*reg;		
	uint16_t	lmem_sz;	
	uint8_t		no_of_channels;
	uint8_t		features;	

	void		*private[2];
	uint8_t		start_once;
}
d2d_t, *d2d_p;
*/
typedef struct
{
	unsigned int addr;
	unsigned int data;
}
wrd_t, *wrd_p;

typedef struct
{
unsigned int src_addr;
unsigned int src_stride;
unsigned int src_width;
unsigned int src_height;
unsigned int src_bpp_mode;

unsigned int dest_addr;
unsigned int dest_stride;
unsigned int dest_width;
unsigned int dest_height;
unsigned int dest_bpp_mode;

unsigned int hflip;
unsigned int vflip;
unsigned int rotate;
	
/*
unsigned int pat_addr = 0; 
unsigned int pat_stride = 0;
unsigned int pat_height = 0; 
unsigned int pat_width = 0; 
unsigned int pat_bpp_mode = 0; 
unsigned int rop_code = 0xaa;
*/
unsigned int alphamode;
unsigned int color;
}
copyblt_t, *copyblt_p;



#endif

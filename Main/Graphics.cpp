#include <iostream>
#include "Graphics.h"
#include "draw2d_common.h"
#include "mapm.h"
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include "d2d.h"
#include <errno.h>
#include <sys/time.h>
#include <signal.h>
#include "bitmap.h"

#define IMAGE_DIR working_directory + std::string("/images")

extern std::string working_directory;

void *graphics_thread(void *g)
{
	Graphics *graphics = (Graphics *) g;

	std::cout << "Graphics started." << std::endl << std::flush;

	std::vector<struct graphics_operation *> draw_set;

	while(1)
	{
		//retrieve next set of graphics operations
		graphics->mutexLock();

		int set = !graphics->open_operation_set; //completed set

		for(int i = 0; i < graphics->operation_set[set].size(); i++)
		{
			struct graphics_operation *op = new struct graphics_operation;

			op->type = graphics->operation_set[set].at(i)->type;
			op->animation = graphics->operation_set[set].at(i)->animation;

			op->colour = graphics->operation_set[set].at(i)->colour;

			draw_set.push_back(op);

			delete(graphics->operation_set[set].at(i));
		}

		graphics->operation_set[set].clear();

		graphics->mutexUnlock();

		//draw next set of graphics operations
		for(int i = 0; i < draw_set.size(); i++)
		{
			struct graphics_operation *next_op = draw_set.at(i);
			if(!graphics->isPaused())
			{
				switch(next_op->type)
				{
					case OP_CLEARSCREEN:
						graphics->clearScreen(next_op->colour);
						break;

					case OP_BLIT:
						graphics->drawAnimation(next_op->animation);
						break;

					case OP_FLIP:
						graphics->flip();
						break;

					case OP_ALGORITHM:
						graphics->drawAlgorithm();
						break;
				}
			}

			delete(next_op);
		}

		draw_set.clear();
	}

	pthread_exit(NULL);
}

Graphics::Graphics(int argc, char **argv)
{
	pthread_t tid;

	void *g = this;

	frame_table.size = 0;

	open_operation_set = 0;

	paused = false;

	graphics_mode = MODE_NORMAL;

	next_window_buffer = main_window;

	show_ball = false;
	show_strings = false;

	window1_width = 550;
	window2_width = screen_width - window1_width;

	mainball_size = 60000, mainball_size_step = 3000;
	mainball.x = getScreenWidth()/2;
	mainball.y = getScreenHeight()/2;
	for(int i = 0; i < 5; i++)
	{
		touch_life[i] = 0;
		touch_visible[i] = false;
	}

	for(int i = 0; i < 6; i++)
		sustain_indicator[i] = false;

	brightness = 0;

	d2d = (volatile unsigned int*) mapm(0x08003000, 0x1000);
	idx = *d2d;
	*(d2d + (0x08>>2))=0x3;
	idx = *(d2d+(0x08>>2));
	while((idx&0x2)!=0)
		idx=*(d2d + (0x08>>2));
		
	main_window_buffer = getMmapAddr(main_window, screen_stride * screen_height);
	floating_window_1_buffer = getMmapAddr(floating_window_1, screen_stride * screen_height);
	off_screen_buffer = getMmapAddr(floating_window_2, 1280*960*4*2);

	memset(off_screen_buffer, 0, 1280*960*4*2);

	stringed_mode = false;
	tap_mode = false;

//	clearScreen(0);
//	flip();

	lcd = (volatile unsigned int*) mapm(0x08004000, 0x1000);
	hideFloatingWindows();
	setLCDScaled();

	for(int i = 0; i < 6; i++)
	{
		string_pressed[i] = false;
		string_updated[i] = true;
	}

	xy_control_old_x = 0;
	xy_control_old_y = 0;
	setXYControlPosition(400, 300);

	clearScreen(0);
	flip();

	if(pthread_mutex_init(&g_mutex, NULL))
		std::cout << "Unable to initialise graphics mutex." << std::endl << std::flush;

	pthread_create(&tid, NULL, graphics_thread, g);
}

Graphics::~Graphics(void)
{
	unmapm((char *)off_screen_buffer, 1280*960*4*2);

	unMmapAddr(main_window_buffer, screen_stride * screen_height);
	unMmapAddr(off_screen_buffer, screen_stride * screen_height);

	pthread_exit(NULL);
	pthread_mutex_destroy(&g_mutex);
//	free(off_screen_buffer);

//	close(d2d_fd);
}

void Graphics::clearOperationSets(void)
{
	for(int set = 0; set < 2; set++)
	{
		for(int i = 0; i < operation_set[set].size(); i++)
		{
			delete(operation_set[set].at(i));
		}

		operation_set[set].clear();
	}
}

void Graphics::createAnimation(struct animation_t *animation, int calls, int align, bool vis, bool rev, bool repeat)
{
	animation->x = 0;
	animation->y = 0;
	animation->calls_per_frame = calls;
	animation->next_frame = 0;
	animation->alignment = align;
	animation->visible = vis;
	animation->reverse = rev;
	animation->current_direction_forward = true;
	animation->repeat = repeat;
}

void Graphics::addImageToAnimation(struct animation_t *animation, std::string filepath, int x, int y, int w, int h, int bpp)
{
	if(loadBMP((char *)filepath.c_str(), (unsigned int *) off_screen_buffer, x, y, w, h, bpp) == -1)
	{
		std::cerr << "Could not open file " << filepath << "." << std::endl;
		exit(1);
		return;
	}

	struct image_dma_frame new_frame;
	new_frame.wx = x;
	new_frame.wy = y;
	new_frame.width = w;
	new_frame.height = h;
	new_frame.bpp = bpp;
	//new_frame.size = new_frame.width * new_frame.height * (new_frame.bpp / 8);
	animation->frames.push_back(new_frame);
	animation->visible = true;
}

int Graphics::loadBMP(char* filename, unsigned int *addr, unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned int bpp)
{
	int i,w,h,x0,y0,ll;
	int bmpfile = 0;
	char *buff;
	unsigned int ipos=0;
	int count,tmp,pixelcount=0;
	char *pt;
	int buffsize;
	unsigned int *fb=addr;
	unsigned int *addx;
	x0 = x;
	y0 = y;
	bmpfile = open(filename,O_RDONLY);
	if (bmpfile<0)
	{
		printf("can not open %s \n",filename);
    	return -1;
	}
	if (bpp==24)
		buffsize = 32700;
	else if (bpp==32)
		buffsize = 32768;
	else {
		printf("only 24 and 32 bpp supported! \n");
    	return -1;
	}
	buff = (char*)malloc(buffsize);	
	// get file information
	//struct stat st;
	//stat(filename, &st);
	//u32 totalbytes = st.st_size;
	lseek(bmpfile,0x36L,0);
	if (bpp==24)
		count = width*height*3;	//total bytes
	else //32bpp
	{
		count = width*height*4;	//total bytes
		//printf("count = %ld\n",count);
	}
	x=x0;
	y=y0;
	while(count>0)
	{
		if (count>buffsize)
		{
			read(bmpfile,buff,buffsize);
			count-=buffsize;
			tmp = buffsize;
		}
		else
		{
			read(bmpfile,buff,count);
			tmp = count;
			count=0;
		}
		pt = buff;
		ll = 0;
		//if (bpp==32) printf("tmp=%ld\n", tmp);
		while(tmp>0)
		{
			addx = fb+y*1280+x;
			
			if (bpp==24)
			{
				*addx = pt[0]+pt[1]*0x100+pt[2]*0x10000;	//buffer = 32bpp
				pt+=3;
				tmp -= 3;
				//pixelcount++;
			} 
			else //32bpp
			{
				//printf("addr=%08x fb=%08x y=%d x=%d\n",addr,fb,y,x);
				*addx = pt[0]+pt[1]*0x100+pt[2]*0x10000+pt[3]*0x1000000;
				pt+=4;
				tmp -= 4;
				//pixelcount++;
			}
			x++;
			if(x>=x0+width) 
			{
				x=x0;
				y++;
			}
		}
	}
	//printf("pixelcount=%ld \n",pixelcount);
	close(bmpfile);
	free(buff);
	return 1;
}

int Graphics::convertRGB(struct colour *col)
{
	int c = col->r;

	c <<= 8;
	c += col->g;
	c <<= 8;
	c += col->b;

	return c;
}

void Graphics::draw(void)
{
	possiblyDrawAlgorithm();
	xy_control_old_x = mainball.x;
	xy_control_old_y = mainball.y;
	possiblyFlip();
}

void Graphics::flip(void)
{
/*	unsigned int id;
	*(d2d + (0x08>>2)) =0x3;
	id=*(d2d+(0x08>>2));
	while((id&0x02)!=0)
		id=*(d2d+(0x08>>2));

	*(d2d + (0x30>>2)) = floating_window_2;
	*(d2d + (0x34>>2)) = 1280*4;//getScreenStride();
//	*(d2d + (0x38>>2)) = (((getScreenHeight())<<16)|(getScreenWidth()));
	*(d2d + (0x38>>2)) = (((600/8)<<16)|(800/8));
	*(d2d + (0x3c>>2)) = 0x7; //should be 0x3?
	*(d2d + (0x40>>2)) = next_window_buffer;//main_window;
	*(d2d + (0x44>>2)) = getScreenStride();
//	*(d2d + (0x48>>2)) = (((getScreenHeight())<<16)|(getScreenWidth()));
	*(d2d + (0x4c>>2)) = 0x3;
	*(d2d + (0x50>>2)) = 0;
	*(d2d + (0x54>>2)) = 0;
	*(d2d + (0x58>>2)) = 0;
	*(d2d + (0x5c>>2)) = 0;
	*(d2d + (0x14>>2)) = 0x00ffffff;

	*(d2d + (0x08>>2)) = 0x1;

	*(d2d + (0x10>>2)) = (0xAA<<8) | 8;

	id=*(d2d+(0x84>>2));
	while((id&0x01)!=0x1)
		id=*(d2d+(0x84>>2));
*/

	if(next_window_buffer == main_window)
	{
		next_window_buffer = floating_window_1;
		*(lcd + (0x54>>2)) = 0x3600; //hide fw1
	}
	else
	{
		next_window_buffer = main_window;
		*(lcd + (0x54>>2)) = 0x3604; //show fw1
	}

	usleep(1000*20); //use this to stop tearing
}

int Graphics::getScreenWidth(void)
{
	return screen_width;
}

int Graphics::getWindow1Width(void)
{
	return window1_width;
}

int Graphics::getWindow2Width(void)
{
	return window2_width;
}

int Graphics::setWindow1Width(int w)
{
	window1_width = w;
}

int Graphics::setWindow2Width(int w)
{
	window2_width = w;
}

int Graphics::getScreenHeight(void)
{
	return screen_height;
}

int Graphics::getScreenBpp(void)
{
	return screen_bpp;
}

int Graphics::getScreenStride(void)
{
	return screen_stride;
}

void Graphics::possiblyClearScreen(unsigned int colour)
{
	addToOpSet(OP_CLEARSCREEN, NULL, colour);
}

void Graphics::possiblyDrawAlgorithm(void)
{
	addToOpSet(OP_ALGORITHM, NULL, 0);
}

void Graphics::possiblyDrawAnimation(struct animation_t *a)
{
	if(a->visible)
		addToOpSet(OP_BLIT, a, 0);
}

void Graphics::possiblyFlip(void)
{
	addToOpSet(OP_FLIP, NULL, 0);
}

void Graphics::addToOpSet(char operation, struct animation_t *a, unsigned int colour)
{
	pthread_mutex_lock(&g_mutex);

	int complete_operation_set = !open_operation_set; //0 or 1 only

	struct graphics_operation *op = new struct graphics_operation;
	op->type = operation;

	op->animation = a;

	op->colour = colour;

	operation_set[open_operation_set].push_back(op);

	if(operation == OP_FLIP)
	{
//		if(operation_set[complete_operation_set].size() > 0) std::cout << "frame skipped." << std::flush;else std::cout << "DRAWN" << std::endl << std::flush;
		for(unsigned int i = 0; i < operation_set[complete_operation_set].size(); i++)
			delete(operation_set[complete_operation_set].at(i));

		operation_set[complete_operation_set].clear(); //don't draw that frame
		open_operation_set = complete_operation_set;
	}

	pthread_mutex_unlock(&g_mutex);
}

void Graphics::clearScreen(unsigned int colour)
{
	unsigned int id;
	*(d2d + (0x08>>2)) = 0x3;
	id=*(d2d+(0x08>>2));
	while((id&0x02)!=0)
		id=*(d2d+(0x08>>2));

	*(d2d+(0x14>>2)) = colour;
	*(d2d+(0x18>>2)) = 0;
//	*(d2d+(0x1c>>2)) = (((getScreenHeight()-1)<<16)|(getScreenWidth()-1));
	*(d2d+(0x1c>>2)) = (((600-1)<<16)|(800-1));
	*(d2d+(0x20>>2)) = 0;
	*(d2d+(0x24>>2)) = (((600-1)<<16)|(800-1));
//	*(d2d+(0x24>>2)) = (((getScreenHeight()-1)<<16)|(getScreenWidth()-1));
	*(d2d+(0x40>>2)) = floating_window_2;
	*(d2d+(0x44>>2)) = screen_stride;
	*(d2d+(0x4c>>2)) = 0x03;

	*(d2d + (0x08>>2)) = 0x1;
	*(d2d + (0x10>>2)) = 0x2;	//draw rect

	id=*(d2d+(0x84>>2));
	while((id&0x1)!=0x1)
		id=*(d2d+(0x84>>2));	//check busy status
}

void Graphics::clearMainWindow(void)
{
	unsigned int id;
	*(d2d + (0x08>>2)) = 0x3;
	id=*(d2d+(0x08>>2));
	while((id&0x02)!=0)
		id=*(d2d+(0x08>>2));

	*(d2d+(0x14>>2)) = 0;
	*(d2d+(0x18>>2)) = 0;
//	*(d2d+(0x1c>>2)) = (((getScreenHeight()-1)<<16)|(getScreenWidth()-1));
	*(d2d+(0x1c>>2)) = (((600-1)<<16)|(800-1));
	*(d2d+(0x20>>2)) = 0;
	*(d2d+(0x24>>2)) = (((600-1)<<16)|(800-1));
//	*(d2d+(0x24>>2)) = (((getScreenHeight()-1)<<16)|(getScreenWidth()-1));
	*(d2d+(0x40>>2)) = main_window;
	*(d2d+(0x44>>2)) = screen_stride;
	*(d2d+(0x4c>>2)) = 0x03;

	*(d2d + (0x08>>2)) = 0x1;
	*(d2d + (0x10>>2)) = 0x2;	//draw rect

	id=*(d2d+(0x84>>2));
	while((id&0x1)!=0x1)
		id=*(d2d+(0x84>>2));	//check busy status
}

void Graphics::drawRectangle(unsigned int colour, unsigned int x, unsigned int y)
{
	unsigned int id;
	*(d2d + (0x08>>2)) =0x3;
	id=*(d2d+(0x08>>2));
	while((id&0x02)!=0)
		id=*(d2d+(0x08>>2));

	*(d2d+(0x14>>2)) = colour;
	*(d2d+(0x18>>2)) = (((y-1)<<16)|(x-1)); //start
	*(d2d+(0x1c>>2)) = (((y+10-1)<<16)|(x+10-1)); //end
	*(d2d+(0x20>>2)) = (((y-1)<<16)|(x-1)); //start
	*(d2d+(0x24>>2)) = (((y+10-1)<<16)|(x+10-1)); //end
	*(d2d+(0x40>>2)) = floating_window_2;
	*(d2d+(0x44>>2)) = 1280*4;//screen_stride;
	*(d2d+(0x4c>>2)) = 0x03;

	*(d2d + (0x08>>2)) = 0x1;
	*(d2d + (0x10>>2)) = 0x2;	//draw rect

	id=*(d2d+(0x84>>2));
	while((id&0x1)!=0x1)
		id=*(d2d+(0x84>>2));	//check busy status
}

unsigned int Graphics::copyToPhysicalMemory(std::string filepath, void *dst, unsigned int image_size)
{
	//make sure this file does not already exist in the frame table
	for(int i = 0; i < frame_table.size; i++)
		if(frame_table.entries[i].filepath == filepath)
			return frame_table.entries[i].phys_addr;

	void *p_addr = getMmapAddr(floating_window_1, screen_stride * screen_height);

	if(frame_table.size == 0)
	{
		memcpy((char*)p_addr, dst, image_size);
		frame_table.entries[frame_table.size].phys_addr = floating_window_1;
	}
	else
	{
		memcpy((char*)p_addr+frame_table.entries[frame_table.size-1].start_next_offset, dst, image_size);
		frame_table.entries[frame_table.size].phys_addr = floating_window_1 + frame_table.entries[frame_table.size-1].start_next_offset;
	}

	unMmapAddr(p_addr, screen_stride * screen_height);

	frame_table.entries[frame_table.size].filepath = filepath;
	frame_table.entries[frame_table.size].start_next_offset = frame_table.entries[frame_table.size-1].start_next_offset + image_size;

	return frame_table.entries[frame_table.size++].phys_addr;
}

unsigned int Graphics::copyToFloatingWindow2(void *dst, unsigned int image_size)
{
	void *p_addr = getMmapAddr(floating_window_2, screen_stride * screen_height);
	memcpy((char*)p_addr, dst, image_size);
	unMmapAddr(p_addr, screen_stride * screen_height);

	return floating_window_2;
}

void *Graphics::getMmapAddr(unsigned int physAddr, size_t size)
{
	void *addr;

	phys_mem_fd = open("/dev/mem", O_RDWR | O_SYNC);
	if(phys_mem_fd < 0)
	{
		std::cerr << "Cannot open memory device." << std::endl;
		return NULL;
	}

	addr = mmap((void *) 0, size, PROT_READ | PROT_WRITE, MAP_SHARED, phys_mem_fd, physAddr);
	if((long) addr == (long) -1)
	{
		std::cerr << "Unable to map memory: " << std::hex << physAddr << std::endl << std::flush;
		close(phys_mem_fd);
		return NULL;
	}

	std::cerr << "Mapped memory: " << std::hex << physAddr << std::endl << std::flush;

	return addr;
}

void Graphics::unMmapAddr(void *start, size_t length)
{
	munmap(start, length);
	close(phys_mem_fd);
}

void Graphics::drawImageDirect(struct image_raw_t *image)
{
	//assume src bpp is the same as dst bpp	
	//code changes significantly if 16bpp instead of 32bpp

	unsigned int line_length = image->width * (screen_bpp >> 3);

	unsigned int *dst_line, *dst, *src_line, *src;
	int dst_stride, src_stride;

	dst_line = (unsigned int *) ((unsigned char *) main_window_buffer + (image->y * screen_stride) + (image->x * (screen_bpp >> 3)));
	src_line = image->buffer;

	dst_stride = screen_stride / sizeof(unsigned int);
	src_stride = image->width;

	unsigned int image_height = image->height;

	while(image_height--)
	{
		dst = dst_line;
		src = src_line;
		memcpy(dst, src, line_length);
		dst_line += dst_stride;
		src_line += src_stride;
	}
}

void Graphics::drawDMAImage(int x, int y, struct image_dma_frame *img)
{
//	if(!animation->visible) return;

	unsigned int id;
	*(d2d + (0x08>>2)) = 0x3;
	id=*(d2d+(0x08>>2));
	while((id&0x02)!=0)
		id=*(d2d+(0x08>>2));

	int new_height = img->height;
	int new_width = img->width;
	int x_start_draw = x, y_start_draw = y;
	int x_offset = img->wx, y_offset = img->wy;

	if(x_start_draw < 0)
	{
		x_offset -= x;
		x_start_draw = 0;
		new_width += x;
	}
	else
	if(x_start_draw + img->width >= screen_width)
		new_width = screen_width - x_start_draw;

	if(y_start_draw < 0)
	{
		y_offset -= y;
		y_start_draw = 0;
		new_height += y;
	}
	else
	if(y_start_draw + img->height >= screen_height)
		new_height = screen_height - y_start_draw;

	*(d2d + (0x30>>2)) = floating_window_2 + (x_offset * (img->bpp/8)) + (y_offset * 1280 * 4);
	*(d2d + (0x34>>2)) = 1280*4;
	*(d2d + (0x38>>2)) = ((new_height << 16) | new_width);
	*(d2d + (0x3c>>2)) = 0x7;
	*(d2d + (0x40>>2)) = floating_window_1 + (y_start_draw * getScreenStride()) + (x_start_draw * 4);
	*(d2d + (0x44>>2)) = getScreenStride();
	*(d2d + (0x4c>>2)) = 0x3;
	*(d2d + (0x50>>2)) = 0;
	*(d2d + (0x54>>2)) = 0;
	*(d2d + (0x58>>2)) = 0;
	*(d2d + (0x5c>>2)) = 0;
	*(d2d + (0x14>>2)) = 0;//0x00ffffff;

	*(d2d + (0x08>>2)) = 0x1;
	*(d2d + (0x10>>2)) = (0xAA<<8) | 9; //transblt

	id=*(d2d+(0x84>>2));
	while((id&0x01)!=0x1)
		id=*(d2d+(0x84>>2));
}

void Graphics::flashScreen(void)
{
	for(int i = 0; i < 5; i++)
		hideTouchIndicator(i);

	for(int s = 0; s < 6; s++)
		setStringPressed(s, false);

	brightness = 255;	
}

void Graphics::drawAlgorithm(void)
{
	unsigned int vx[800/8];
	unsigned int vy[600/8];
	unsigned int sx[6][800/8];
	unsigned int sy[6][600/8];

	unsigned int vxt[5][800/8];
	unsigned int vyt[5][600/8];

	unsigned int *fb;

	//page flipping mechanism
	if(next_window_buffer == main_window)
		fb = (unsigned int *) main_window_buffer;
	else
		fb = (unsigned int *) floating_window_1_buffer;

	unsigned int *addx;

	int mx = mainball.x / 8;
	int my = mainball.y / 8;
	int ms = mainball_size;

	if(show_ball)
	{
		for(int x = 0; x < 800/8; x++)
			vx[x] = (mx - x) * (mx - x);

		for(int y = 0; y < 600/8; y++)
			vy[y] = (my - y) * (my - y);
	}
	else
	{
		for(int x = 0; x < 800/8; x++)
			vx[x] = (mx) * (mx);

		for(int y = 0; y < 600/8; y++)
			vy[y] = (my) * (my);
	}

	if(show_strings)
	{
		for(int s = 0; s < 6; s++)
		{
			for(int x = 0; x < 800/8; x++)
				sx[s][x] = (96 - x) * (96 - x);

			for(int y = 0; y < 600/8; y++)
				sy[s][y] = ((600/6/8/2+1)+(600/6/8)*s-y) * ((600/6/8/2+1)+(600/6/8)*s-y) * (string_pressed[5-s]?5:100);
		}
	}

	for(int i = 0; i < 5; i++)
	{
		for(int x = 0; x < 800/8; x++)
			vxt[i][x] = (touch[i].x/8 - x) * (touch[i].x/8 - x);

		for(int y = 0; y < 600/8; y++)
			vyt[i][y] = (touch[i].y/8 - y) * (touch[i].y/8 - y);
	}

	for(int x = 0; x < 800/8; x++)
		for(int y = 0; y < 600/8; y++)
		{
			int m = 0;

//			if(show_ball)
				m = (ms / (vx[x] + vy[y] + 1)) + 1;

//			if(!show_ball) m+=25;

			for(int i = 0; i < 5; i++)
				if(touch_life[i] > 0)
				{
//					if(touch_life[i] < 70000) touch_life[i]++;
					m += touch_life[i] / (vxt[i][x] + vyt[i][y] + 1);
				}

			if(show_strings)
				for(int s = 0; s < 6; s++)
					m += (10000 / (sx[s][x] + sy[s][y] + 1));
			
			for(int s = 0; s < 6; s++)
				if(sustain_indicator[s] && (y >= ((5-s) * (75/6+1))) && (y < ((5-s) * (75/6+1)) + (75/6+1)))
					m+=50;

			int r = 0, g = 0, b = 0;
			if(!tap_mode)
			{
				r = (m >> 1) + brightness;
				g = m + brightness;
				b = (m << 1) + brightness;
			}
			else
			{
				b = (m >> 2) + brightness;
				g = (m >> 2) + brightness;
				r = (m) + brightness;
			}
			if(r > 255) r = 255; if(g > 255) g = 255; if(b > 255) b = 255;

			int pixel = b + (g<<8) + (r<<16);
			addx = fb+y*800+x;//fb+y*1280+x;
			*addx = pixel;
		}

	mainball_size += mainball_size_step;
	if((mainball_size > 120000) || (mainball_size < 25000))
		mainball_size_step = -mainball_size_step;

	if(brightness > 0) brightness-=20;
}

void Graphics::setStringPressed(unsigned char s, bool state)
{
	string_updated[s] = true;
	string_pressed[s] = state;
}

void Graphics::setStringPosition(unsigned char str, unsigned int y)
{
/*	guitar_string_unpressed[str].y = y;
	guitar_string_pressed[str].y = y;
	guitar_string_unpressed_tap[str].y = y;
	guitar_string_pressed_tap[str].y = y;
*/}

void Graphics::setXYControlPosition(unsigned int x, unsigned int y)
{
//	mutexLock();
	mainball.x = x;
	mainball.y = y;
//	mutexUnlock();
}

void Graphics::setDragOriginIndicator(unsigned int x, unsigned int y)
{
/*	drag_origin_indicator.x = x;
	drag_origin_indicator.y = y;
	drag_origin_indicator.visible = true;
	drag_origin_indicator_tap.x = x;
	drag_origin_indicator_tap.y = y;
	drag_origin_indicator_tap.visible = true;
*/
}

void Graphics::hideDragOriginIndicator(void)
{
//	drag_origin_indicator.visible = false;
//	drag_origin_indicator_tap.visible = false;
}

void Graphics::setTouchIndicator(unsigned int touch_id, unsigned int x, unsigned int y)
{
	touch[touch_id].x = x;
	touch[touch_id].y = y;
	if(!touch_visible[touch_id])
	{
		touch_life[touch_id] = 10000;
		brightness = 100;
		touch_visible[touch_id] = true;
	}
}

void Graphics::hideTouchIndicator(unsigned int touch_id)
{
	touch_life[touch_id] = 0;
	touch_visible[touch_id] = false;
}

void Graphics::setSustainIndicator(unsigned char str, bool state)
{
	sustain_indicator[str] = state;
}

void Graphics::setGraphicsModeNormal(bool tap_state)
{
	xy_control_old_x = 0;
	xy_control_old_y = 0;
	graphics_mode = MODE_NORMAL;
	stringed_mode = false;
	tap_mode = tap_state;
}

void Graphics::drawAnimation(struct animation_t *animation)
{
//	if(!animation->visible) return;

	struct image_dma_frame *img = &animation->frames.at(animation->next_frame);

	switch(animation->alignment)
	{
		case ALIGN_LEFT:
			drawDMAImage(animation->x, animation->y-img->height/2, img);
			break;

		case ALIGN_CENTERED:
			drawDMAImage(animation->x-img->width/2, animation->y-img->height/2, img);
			break;
	}

	if(animation->reverse)
	{
		if(animation->current_direction_forward)
		{
			animation->next_frame++;
			if(animation->next_frame >= animation->frames.size())
			{
				animation->next_frame--;
				animation->current_direction_forward = false;
			}
		}
		else
		{
			animation->next_frame--;
			if(animation->next_frame < 0)
			{
				animation->next_frame = 0;
				animation->current_direction_forward = true;
			}
		}
	}
	else
	{
		animation->next_frame++;
		if(animation->next_frame >= animation->frames.size())
		{
			animation->next_frame = 0;
			if(!animation->repeat) animation->visible = false;
		}
	}
}

void Graphics::mutexLock(void)
{
	pthread_mutex_lock(&g_mutex);
}

void Graphics::mutexUnlock(void)
{
	pthread_mutex_unlock(&g_mutex);
}

unsigned long* Graphics::GetOffScreenBuffer(void)
{
	return (unsigned long*)off_screen_buffer;
}

void Graphics::setPaused(bool state)
{
	paused = state;
}

bool Graphics::isPaused(void)
{
	return paused;
}

void Graphics::setLCDScaled(void)
{
	*(lcd + (0x58>>2)) = 0x300; //scale 8x
}

void Graphics::setLCDUnScaled(void)
{
	*(lcd + (0x58>>2)) = 0x0; //scale 1x
}

void Graphics::hideFloatingWindows(void)
{
	clearMainWindow();
	*(lcd + (0x54>>2)) = 0x3600; //hide fw1 and fw2
}

void Graphics::showBall(bool state)
{
	show_ball = state;
}

void Graphics::showStrings(bool state)
{
	show_strings = state;
}

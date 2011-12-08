
#ifndef __GRAPHICSWRAPPER_H__
#define __GRAPHICSWRAPPER_H__

#include <string>
#include "ebmlib.h"
#include "mapm.h"
#include <vector>
#include <pthread.h>

#define DRAW2D_BASE 0x08003000

#define ALIGN_LEFT		1
#define ALIGN_CENTERED 	2

#define OP_CLEARSCREEN	0
#define OP_BLIT			1
#define OP_FLIP			2
#define OP_ALGORITHM	3

struct frame_table_entry_t {
	std::string filepath;
	unsigned int phys_addr;
	unsigned int start_next_offset;
};

struct frame_table_t {
	frame_table_entry_t entries[10];
	unsigned int size;
};

struct image_t {
	int x;
	int y;
	unsigned int frame_mem;
	int frame_size, frame_width, frame_height, frame_bpp;
	bool visible;
	char alignment;
};

struct image_raw_t {
	int x, y;
	unsigned int *buffer;
	int size, width, height, bpp;
};

struct image_dma_frame {
	int wx, wy;
	int width, height, bpp;
};

struct animation_t {
	int x, y;
	bool visible;
	std::vector<struct image_dma_frame> frames;
	int calls_per_frame;
	int next_frame;
	int alignment;
	bool reverse;
	bool current_direction_forward;
	bool repeat;
};

struct graphics_operation
{
	char type;	//OP_CLEARSCREEN, OP_BLIT, OP_FLIP
	struct animation_t *animation;
	unsigned int colour;
};

struct colour
{
	int r, g, b;
};

struct coordinate
{
	int x, y;
};

//to get start address of main window: mm r 4 0x0800405c (see LCDC reference)

class Graphics
{
	private:
		static const int screen_width = 800, screen_height = 600, screen_bpp = 32;
		int window1_width, window2_width;
		static const int screen_stride = screen_width * (screen_bpp >> 3);
		static const int main_window = 0x53E2B000;
		static const int floating_window_1 = 0x53C56000;
		static const int floating_window_2 = 0x532F6000;//0x53A81000;
		int next_window_buffer;

		volatile unsigned int *d2d, *lcd;
		volatile unsigned int idx;

		pthread_mutex_t g_mutex;

		int phys_mem_fd;
		frame_table_t frame_table;
		int d2d_fd;
		bool string_pressed[6], string_updated[6];
		struct animation_t xy_ball, xy_ball_tap, touch_indicator[5], touch_indicator_tap[5], drag_origin_indicator, drag_origin_indicator_tap;
		struct animation_t guitar_string_unpressed[6], guitar_string_pressed[6];
		struct animation_t guitar_string_unpressed_tap[6], guitar_string_pressed_tap[6];
		int xy_control_old_x, xy_control_old_y;
		int mainball_size, mainball_size_step;
		struct coordinate mainball, touch[5];
		int touch_life[5];
		bool touch_visible[5];
		bool sustain_indicator[6];

		int brightness;

		static const int MODE_NORMAL = 1;
		unsigned char graphics_mode;

		void *main_window_buffer, *floating_window_1_buffer, *off_screen_buffer;

		struct colour screen_colour, background;
		bool stringed_mode, tap_mode, paused;
		bool show_ball, show_strings;

	public:
		Graphics(int argc, char **argv);
		~Graphics(void);
		void clearOperationSets(void);
		void draw(void);
		void flip(void);
		int getScreenWidth(void);
		int getWindow1Width(void);
		int getWindow2Width(void);
		int setWindow1Width(int w);
		int setWindow2Width(int w);
		int getScreenHeight(void);
		int getScreenBpp(void);
		int getScreenStride(void);
		void clearScreen(unsigned int colour);
		void clearMainWindow(void);
		unsigned int copyToPhysicalMemory(std::string filepath, void *dst, unsigned int image_size);
		unsigned int copyToFloatingWindow2(void *dst, unsigned int image_size);
		void *getMmapAddr(unsigned int physAddr, size_t size);
		void unMmapAddr(void *start, size_t length);
		void drawImageDirect(struct image_raw_t *image);
		void drawDMAImage(int x, int y, struct image_dma_frame *img);
		void setStringPressed(unsigned char s, bool state);
		void setStringPosition(unsigned char s, unsigned int y);
		void setSustainIndicator(unsigned char s, bool state);
		void setXYControlPosition(unsigned int x, unsigned int y);
		void setDragOriginIndicator(unsigned int x, unsigned int y);
		void hideDragOriginIndicator(void);
		void setTouchIndicator(unsigned int touch_id, unsigned int x, unsigned int y);
		void hideTouchIndicator(unsigned int touch_id);
		void setGraphicsModeNormal(bool tap_mode);
		int loadBMP(char* filename, unsigned int *addr, unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned int bpp);
		void createAnimation(struct animation_t *animation, int calls, int align, bool vis, bool rev, bool repeat);
		void addImageToAnimation(struct animation_t *animation, std::string filepath, int x, int y, int w, int h, int bpp);
		void drawAnimation(struct animation_t *animation);
		void drawAlgorithm(void);
		void drawRectangle(unsigned int colour, unsigned int x, unsigned int y);
		void possiblyClearScreen(unsigned int colour);
		void possiblyDrawAnimation(struct animation_t *a);
		void possiblyDrawAlgorithm(void);
		void possiblyFlip(void);
		void addToOpSet(char operation, struct animation_t *a, unsigned int colour);
		void mutexLock(void);
		void mutexUnlock(void);
		int convertRGB(struct colour *col);

		std::vector<struct graphics_operation *> operation_set[2];
		char open_operation_set;
		unsigned long* GetOffScreenBuffer(void);

		void setPaused(bool state);
		bool isPaused(void);
		void setLCDScaled(void);
		void setLCDUnScaled(void);
		void hideFloatingWindows(void);

		void showBall(bool state);
		void showStrings(bool state);

		void flashScreen(void);
};
#endif

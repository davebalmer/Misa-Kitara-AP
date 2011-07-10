#include "ebmlib.h"
#include "mapm.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

//#include "platform.h"

// artemis2
#define ABB1_BASE	 0x08000000//	0xD0000000
#define ABB2_BASE	0x08100000//	0xD8000000
#define SDR_CSD1_BASE	0x90000000
#define ABB_RANGE		0x11000


void *apb1 = 0;
void *apb2 = 0;
/* void *expio = 0; */
/* void *nandf = 0; */
void *sdram_cs1 = 0;

void ebm_break(int a){
	/*
	fprintf(stderr, "caught signal %x\n", a);
	*/
	ebmlib_exit();
	exit(0);
}

int ebmlib_init(){
	/*
	fprintf(stderr,"Debug: init\n");
	*/
	apb1 = mapm(ABB1_BASE,ABB_RANGE);
	if(!apb1){
		fprintf(stderr, "Err: mapping of apb1 space failed\n");
		exit(1);
	}
	apb2 = mapm(ABB2_BASE,ABB_RANGE);
	if(!apb2){
		unmapm(apb1,ABB_RANGE);
		fprintf(stderr, "Err: mapping of apb2 space failed\n");
		exit(1);
	}
/*	expio = mapm(0x20000000,0x10000000); */
/*	nandf = mapm(0x40000000,0x10000000); */
	sdram_cs1 = mapm(SDR_CSD1_BASE, 0x500000);
	if(!sdram_cs1){
		unmapm(apb1,ABB_RANGE);
		unmapm(apb2,ABB_RANGE);
		fprintf(stderr, "Err: mapping of sdram cs1 space failed");
		exit(1);
	}
	signal(SIGKILL, ebm_break);
	return 0;
	/*
	fprintf(stderr,"debug: init good\n");
*/
}

void ebmlib_exit(){
	unmapm(apb1,ABB_RANGE);
	unmapm(apb2,ABB_RANGE);
	/*
	unmapm(expio,0x10000000);
	unmapm(nandf,0x10000000);
	*/
	unmapm(sdram_cs1,0x500000);
}

int ebmlib_rd(int addr){
	int result;
	/*
	fprintf(stderr,"debug:ebmlib_rd\n");
	*/
	if((addr >= ABB1_BASE) && (addr < (ABB1_BASE+ABB_RANGE))){ /* hacked for d2d module */
		addr -= ABB1_BASE;
		result = *((int *)((char*)apb1 + addr));
	} else if((addr >= ABB2_BASE) && (addr < (ABB2_BASE+ABB_RANGE))) {
		addr -= ABB2_BASE;
		result = *((int *)((char*)apb2 + addr));
	} else if((addr >= SDR_CSD1_BASE) && (addr < (SDR_CSD1_BASE+0x500000))) {
		addr -= SDR_CSD1_BASE;
		result = *((int *)((char*)sdram_cs1 + addr));
	} else {
		fprintf(stderr, "Error: currently ebmlib port on magus only supports apb bus, expio, nandf and sdram:cs1.  Please contact writer if you think you should have ownership on other range of address\n");
		return -1;
	}
	/*
	fprintf(stderr,"debug:ebmlib_rd good\n");
	*/
	return result;
}

signed char ebmlib_rd8(int addr){
	char result;
	/*
	fprintf(stderr,"debug:ebmlib_rd8\n");
	*/
	if((addr >= ABB1_BASE) && (addr < (ABB1_BASE+ABB_RANGE))){ /* hacked for d2d module */
		addr -= ABB1_BASE;
		result = *((char *)((char*)apb1 + addr));
	} else if((addr >= ABB2_BASE) && (addr < (ABB2_BASE+ABB_RANGE))) {
		addr -= ABB2_BASE;
		result = *((char *)((char*)apb2 + addr));
	} else if((addr >= SDR_CSD1_BASE) && (addr < (SDR_CSD1_BASE+0x500000))) {
		addr -= SDR_CSD1_BASE;
		result = *((char *)((char*)sdram_cs1 + addr));
	} else {
		fprintf(stderr, "Error: currently ebmlib port on magus only supports apb bus, expio, nandf and sdram:cs1.  Please contact writer if you think you should have ownership on other range of address\n");
		return -1;
	}
	/*
	fprintf(stderr,"debug:ebmlib_rd8 good\n");
	*/
	return result;
}

short ebmlib_rd16(int addr){
	short result;
	/*
	fprintf(stderr,"debug:ebmlib_rd16\n");
	*/
	if((addr >= ABB1_BASE) && (addr < (ABB1_BASE+ABB_RANGE))){ /* hacked for d2d module */
		addr -= ABB1_BASE;
		result = *((short *)((char*)apb1 + addr));
	} else if((addr >= ABB2_BASE) && (addr < (ABB2_BASE+ABB_RANGE))) {
		addr -= ABB2_BASE;
		result = *((short *)((char*)apb2 + addr));
	} else if((addr >= SDR_CSD1_BASE) && (addr < (SDR_CSD1_BASE+0x500000))) {
		addr -= SDR_CSD1_BASE;
		result = *((short *)((char*)sdram_cs1 + addr));
	} else {
		fprintf(stderr, "Error: currently ebmlib port on magus only supports apb bus, expio, nandf and sdram:cs1.  Please contact writer if you think you should have ownership on other range of address\n");
		return -1;
	}
	/*
	fprintf(stderr,"debug:ebmlib_rd16 good\n");
	*/
	return result;
}

void ebmlib_wr(int addr, int data){
	/*
	fprintf(stderr,"debug:ebmlib_wr\n");
	*/
	if((addr >= ABB1_BASE) && (addr < (ABB1_BASE+ABB_RANGE))){ /* hacked for d2d module */
		addr -= ABB1_BASE;
		*((int *)((char*)apb1 + addr)) = data;
	} else if((addr >= ABB2_BASE) && (addr < (ABB2_BASE+ABB_RANGE))) {
		addr -= ABB2_BASE;
		*((int *)((char*)apb2 + addr)) = data;
	} else  if((addr >= SDR_CSD1_BASE) && (addr < (SDR_CSD1_BASE+0x500000))) {
		addr -= SDR_CSD1_BASE;
		*((int *)((char*)sdram_cs1 + addr)) = data;
	} else {
		fprintf(stderr, "Error: currently ebmlib port on magus only supports apb bus, expio, nandf and sdram:cs1.  Please contact writer if you think you should have ownership on other range of address\n");
	}
	/*
	fprintf(stderr,"debug:ebmlib_wr good\n");
	*/
}

void ebmlib_wr8(int addr, char data){
	/*
	fprintf(stderr,"debug:ebmlib_wr8\n");
	*/
	if((addr >= ABB1_BASE) && (addr < (ABB1_BASE+ABB_RANGE))){ /* hacked for d2d module */
		addr -= ABB1_BASE;
		*((char *)((char*)apb1 + addr)) = data;
	} else if((addr >= ABB2_BASE) && (addr < (ABB2_BASE+ABB_RANGE))) {
		addr -= ABB2_BASE;
		*((char *)((char*)apb2 + addr)) = data;
	} else  if((addr >= SDR_CSD1_BASE) && (addr < (SDR_CSD1_BASE+0x500000))) {
		addr -= SDR_CSD1_BASE;
		*((char *)((char*)sdram_cs1 + addr)) = data;
	} else {
		fprintf(stderr, "Error: currently ebmlib port on magus only supports apb bus, expio, nandf and sdram:cs1.  Please contact writer if you think you should have ownership on other range of address\n");
	}
	/*
	fprintf(stderr,"debug:ebmlib_wr8 good\n");
	*/
}

void ebmlib_wr16(int addr, short data){
	/*
	fprintf(stderr,"debug:ebmlib_wr16\n");
	*/
	if((addr >= ABB1_BASE) && (addr < (ABB1_BASE+ABB_RANGE))){ /* hacked for d2d module */
		addr -= ABB1_BASE;
		*((short *)((char*)apb1 + addr)) = data;
	} else if((addr >= ABB2_BASE) && (addr < (ABB2_BASE+ABB_RANGE))) {
		addr -= ABB2_BASE;
		*((short *)((char*)apb2 + addr)) = data;
	} else  if((addr >= SDR_CSD1_BASE) && (addr < (SDR_CSD1_BASE+0x500000))) {
		addr -= SDR_CSD1_BASE;
		*((short *)((char*)sdram_cs1 + addr)) = data;
	} else {
		fprintf(stderr, "Error: currently ebmlib port on magus only supports apb bus, expio, nandf and sdram:cs1.  Please contact writer if you think you should have ownership on other range of address\n");
	}
	/*
	fprintf(stderr,"debug:ebmlib_wr16 good\n");
	*/
}

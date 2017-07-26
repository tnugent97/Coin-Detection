////////////////////////////////////////////////////////////////////////////////
//  File:           greyscale.h
//  Description:    
//  By:             MTM
////////////////////////////////////////////////////////////////////////////////



#ifndef _BLUR
#define _BLUR

#include <ac_int.h>
#include <iostream>

// total number of pixels from screen frame/image read in testbench
#define NUM_PIXELS (640*480)

#define KERNEL_WIDTH      1
#define KERNEL_NUMEL      (KERNEL_WIDTH * KERNEL_WIDTH)
#define COLOUR_WL         10
#define PIXEL_WL          (3 * COLOUR_WL)

#define  COORD_WL          10

void mean_vga(ac_int<PIXEL_WL*KERNEL_WIDTH,false> vin[NUM_PIXELS], ac_int<10, false> threshold, ac_int<PIXEL_WL,false> vout[NUM_PIXELS]);

#endif

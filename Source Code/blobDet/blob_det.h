////////////////////////////////////////////////////////////////////////////////
//  File:           blob_det.h
//  Description:    vga blur - real-time processing
//  By:             rad09
////////////////////////////////////////////////////////////////////////////////


#ifndef _BLUR
#define _BLUR

#include <ac_int.h>
#include <iostream>

// total number of pixels from screen frame/image read in testbench
#define NUM_PIXELS (640*480)

#define KERNEL_WIDTH      64
#define KERNEL_NUMEL      (KERNEL_WIDTH * KERNEL_WIDTH)
#define COLOUR_WL         10
#define PIXEL_WL          (3 * COLOUR_WL)

#define  COORD_WL          10


void blob_det(ac_int<1,false>& vin, ac_int<1,false>& enable_copy, ac_int<1,false>& startFrame, ac_int<1,false> *flag,static ac_int<14,false> *area0,static ac_int<14,false> *area1,static ac_int<14,false> *area2,static ac_int<14,false> *area3,static ac_int<14,false> *area4,static ac_int<14,false> *area5,static ac_int<14,false> *area6,static ac_int<14,false> *area7,static ac_int<14,false> *area8,static ac_int<14,false> *area9);

#endif

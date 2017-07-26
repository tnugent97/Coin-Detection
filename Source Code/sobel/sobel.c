////////////////////////////////////////////////////////////////////////////////
//  _____                           _       _    _____      _ _
// |_   _|                         (_)     | |  / ____|    | | |
//   | |  _ __ ___  _ __   ___ _ __ _  __ _| | | |     ___ | | | ___  __ _  ___
//   | | | '_ ` _ \| '_ \ / _ \ '__| |/ _` | | | |    / _ \| | |/ _ \/ _` |/ _ \
//  _| |_| | | | | | |_) |  __/ |  | | (_| | | | |___| (_) | | |  __/ (_| |  __/
// |_____|_| |_| |_| .__/ \___|_|  |_|\__,_|_|  \_____\___/|_|_|\___|\__, |\___|
//                 | |                                                __/ |
//                 |_|                                               |___/
//  _                     _
// | |                   | |
// | |     ___  _ __   __| | ___  _ __
// | |    / _ \| '_ \ / _` |/ _ \| '_ \
// | |___| (_) | | | | (_| | (_) | | | |
// |______\___/|_| |_|\__,_|\___/|_| |_|
//
////////////////////////////////////////////////////////////////////////////////
//  File:           sobel.cpp
//  Description:    video to vga blur filter - real-time processing
//  By:             MTM
////////////////////////////////////////////////////////////////////////////////
// this hardware block receives the VGA stream and then produces a blured output
// based on the FIR design - page 230 of HLS Blue Book
////////////////////////////////////////////////////////////////////////////////
// Catapult Project options
// Constraint Editor:
//  Frequency: 27 MHz
//  Top design: vga_blur
//  clk>reset sync: disable; reset async: enable; enable: enable
// Architecture Constraints:
//  interface>vin: wordlength = 150, streaming = 150
//  interface>vout: wordlength = 30, streaming = 30
//  core>main: pipeline + distributed + merged
//  core>main>frame: merged
//  core>main>frame>shift, mac1, mac2: unroll + merged
////////////////////////////////////////////////////////////////////////////////


#include <ac_fixed.h>
#include "blur.h"
#include <iostream>

// shift_class: page 119 HLS Blue Book
#include "shift_class.h" 


//absolute value function

ac_int<16> abs(ac_int<16> din){
    ac_int<16> tmp0=0,tmp1 = 0;
    tmp0 = din;
    for(int i=0;i<16;i++)
        tmp1[i] = tmp0[i]^tmp0[15];
    return tmp1+tmp0[15];
}

#pragma hls_design top
void mean_vga(ac_int<PIXEL_WL*KERNEL_WIDTH,false> vin[NUM_PIXELS], ac_int<PIXEL_WL,false> vout[NUM_PIXELS])
{
    ac_int<16, true> r[3*KERNEL_WIDTH], g[3*KERNEL_WIDTH], b[3*KERNEL_WIDTH], red=0, green=0, blue=0;
	ac_int<16, true> red_x=0, green_x=0, blue_x=0, red_y=0, green_y=0, blue_y=0;
    

// #if 1: use filter
// #if 0: copy input to output bypassing filter
#if 1

    // shifts pixels from KERNEL_WIDTH rows and keeps KERNEL_WIDTH columns (KERNEL_WIDTHxKERNEL_WIDTH pixels stored)
    static shift_class<ac_int<PIXEL_WL*KERNEL_WIDTH,false>, KERNEL_WIDTH> regs;
    int i;

    FRAME: for(int p = 0; p < NUM_PIXELS; p++) {
		// init
		red = 0; 
		green = 0; 
		blue = 0;
		RESET: for(i = 0; i < 9; i++) {
			r[i] = 0;
			g[i] = 0;
			b[i] = 0;
		}
	    
		// shift input data in the filter fifo
		regs << vin[p]; // advance the pointer address by the pixel number (testbench/simulation only)
		// accumulate
		ACC1: for(i = 0; i < KERNEL_WIDTH; i++) {
			// current line
			r[i] = (regs[i].slc<COLOUR_WL>(2*COLOUR_WL));
			g[i] = (regs[i].slc<COLOUR_WL>(COLOUR_WL));
			b[i] = (regs[i].slc<COLOUR_WL>(0));
			// the line before ...
			r[i+3] = (regs[i].slc<COLOUR_WL>(2*COLOUR_WL + PIXEL_WL));
			g[i+3] = (regs[i].slc<COLOUR_WL>(COLOUR_WL + PIXEL_WL));
			b[i+3] = (regs[i].slc<COLOUR_WL>(0 + PIXEL_WL));
			// the line before ...
			r[i+6] = (regs[i].slc<COLOUR_WL>(2*COLOUR_WL + 2*PIXEL_WL));
			g[i+6] = (regs[i].slc<COLOUR_WL>(COLOUR_WL + 2*PIXEL_WL)) ;
			b[i+6] = (regs[i].slc<COLOUR_WL>(0 + 2*PIXEL_WL)) ;
		}
		// add the accumualted value for all processed lines
		ACC2:
		
		red_x += r[0]*-1 + r[2] + r[3] * -2 + r[5] * 2 + r[6] * -1 + r[8];
		green_x += g[0]*-1 + g[2] + g[3] * -2 + g[5] * 2 + g[6] * -1 + g[8];
		blue_x += b[0]*-1 + b[2] + b[3] * -2 + b[5] * 2 + b[6] * -1 + b[8];
		
		red_y += r[0] * -1 + r[1] * -2 + r[2] * -1 + r[6] + 2 * r[7] + r[8];
		green_y += g[0] * -1 + g[1] * -2 + g[2] * -1 + g[6] + 2 * g[7] + g[8];
		blue_y += b[0] * -1 + b[1] * -2 + b[2] * -1 + b[6] + 2 * b[7] + b[8];
		
		red = abs(red_x) + abs(red_y);
		green = abs(green_x) + abs(green_y);
		blue = abs(blue_x) + abs(blue_y);
	    
		red /= 3;
		green /= 3;
		blue /= 3;
		
		// group the RGB components into a single signal
		vout[p] = ((((ac_int<PIXEL_WL, false>)red) << (2*COLOUR_WL)) | (((ac_int<PIXEL_WL, false>)green) << COLOUR_WL) | (ac_int<PIXEL_WL, false>)blue);
	    
    }
}
     
     
     
     
     
     
#else    
// display input  (test only)
    FRAME: for(p = 0; p < NUM_PIXELS; p++) {
        // copy the value of each colour component from the input stream
        red = vin[p].slc<COLOUR_WL>(2*COLOUR_WL);
        green = vin[p].slc<COLOUR_WL>(COLOUR_WL);
        blue = vin[p].slc<COLOUR_WL>(0);

		// combine the 3 color components into 1 signal only
        vout[p] = ((((ac_int<PIXEL_WL, false>)red) << (2*COLOUR_WL)) | (((ac_int<PIXEL_WL, false>)green) << COLOUR_WL) | (ac_int<PIXEL_WL, false>)blue);   
    }
}
#endif


// end of file
////////////////////////////////////////////////////////////////////////////////
//  File:           greyscale.cpp
//  Description:    
//  By:             MTM
////////////////////////////////////////////////////////////////////////////////

#include <ac_fixed.h>
#include "greyscale.h"
#include <iostream>

// shift_class: page 119 HLS Blue Book
#include "shift_class.h" 

#pragma hls_design top
void greyscale(ac_int<PIXEL_WL,false> vin[NUM_PIXELS], ac_int<PIXEL_WL,false> vout[NUM_PIXELS])
{
    ac_int<16, false> fin;
    

// #if 1: use filter
// #if 0: copy input to output bypassing filter
#if 1

    // shifts pixels from KERNEL_WIDTH rows and keeps KERNEL_WIDTH columns (KERNEL_WIDTHxKERNEL_WIDTH pixels stored)
    static shift_class<ac_int<PIXEL_WL*KERNEL_WIDTH,false>, KERNEL_WIDTH> regs;
    int i = 0;

    FRAME: for(int p = 0; p < NUM_PIXELS; p++) {
		// init
		fin = 0; 
			    
		// shift input data in the filter fifo
		regs << vin[p]; // advance the pointer address by the pixel number (testbench/simulation only)
		// accumulate
		ACC1:
			// current pixel
			fin += (regs[i].slc<COLOUR_WL>(2*COLOUR_WL));
			fin += (regs[i].slc<COLOUR_WL>(COLOUR_WL));
			fin += (regs[i].slc<COLOUR_WL>(0));
		
		// normalize result
		fin /= 3;
	    
		// group the RGB components into a single signal
		vout[p] = ((((ac_int<PIXEL_WL, false>)fin) << (2*COLOUR_WL)) | (((ac_int<PIXEL_WL, false>)fin) << COLOUR_WL) | (ac_int<PIXEL_WL, false>)fin);
	    
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

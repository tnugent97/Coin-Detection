////////////////////////////////////////////////////////////////////////////////
// Catapult Project options
// Constraint Editor:
//  Frequency: 27 MHz
//  Top design: vga_mouse
//  clk>reset sync: disable; reset async: enable; enable: enable
// Architecture Constraint:
//  core>main: enable pipeline + loop can be merged
////////////////////////////////////////////////////////////////////////////////



#include "stdio.h"
#include "ac_int.h"

#define COLOR_WL          10
#define PIXEL_WL          (3*COLOR_WL)

#define  COORD_WL          10

#pragma hls_design top
void mouse_new(ac_int<(COORD_WL+COORD_WL), false> * vga_xy, ac_int<(COORD_WL+COORD_WL), false> * mouse_xy, ac_int<(8), false> cursor_size, ac_int<1, false> * clk_en, ac_int<PIXEL_WL, false> * video_in, ac_int<PIXEL_WL, false> * video_out)
{
    ac_int<10, false> i_red, i_green, i_blue; // current pixel
    ac_int<10, false> o_red, o_green, o_blue; // output pixel
    ac_int<10, false> mouse_x, mouse_y, vga_x, vga_y; // mouse and screen coordinates

    
/*  --extract the 3 color components from the 30 bit signal--
    the 2 blocks are identical - you can shift and mask the desired bits or "slice" the signal <length>(location)
    
    i_red = *video_in >> 20;
    i_green = (*video_in >> 10) & (ac_int<10>)1023;
    i_blue = *video_in & ((ac_int<10>)1023); 
*/    
    i_red = (*video_in).slc<COLOR_WL>(20);
    i_green = (*video_in).slc<COLOR_WL>(10);
    i_blue = (*video_in).slc<COLOR_WL>(0);

    // extract mouse X-Y coordinates
    mouse_x = (*mouse_xy).slc<COORD_WL>(0);
    mouse_y = -(*mouse_xy).slc<COORD_WL>(10);
    // extract VGA pixel X-Y coordinates
    vga_x = (*vga_xy).slc<COORD_WL>(0);
    vga_y = (*vga_xy).slc<COORD_WL>(10);
   
      
    if ((vga_x > mouse_x - cursor_size) && (vga_x <= mouse_x + cursor_size) && (vga_y > mouse_y - cursor_size) && (vga_y <= mouse_y + cursor_size)){
        // if it is inside the mouse square
        o_red = 400;
        o_green = 800;
        o_blue = i_blue;
        *clk_en = 1;
    }
    else {
        // if it is outside the mouse square
        o_red = i_red;
        o_green = i_green;
        o_blue = i_blue;
        *clk_en = 0;
    }
   
    *video_out = ((((ac_int<PIXEL_WL, false>)o_red) << 20) | (((ac_int<PIXEL_WL, false>)o_green) << 10) | (ac_int<PIXEL_WL, false>)o_blue);
}


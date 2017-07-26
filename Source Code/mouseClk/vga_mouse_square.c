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
void mouseClk(ac_int<(COORD_WL+COORD_WL), false> * vga_xy, ac_int<(COORD_WL+COORD_WL), false> * mouse_xy, ac_int<(8), false> cursor_size, ac_int<1, false> * clk_en)
{
    
    ac_int<10, false> mouse_x, mouse_y, vga_x, vga_y; // mouse and screen coordinates

    // extract mouse X-Y coordinates
    mouse_x = (*mouse_xy).slc<COORD_WL>(0);
    mouse_y = -(*mouse_xy).slc<COORD_WL>(10);
    // extract VGA pixel X-Y coordinates
    vga_x = (*vga_xy).slc<COORD_WL>(0);
    vga_y = (*vga_xy).slc<COORD_WL>(10);
   
    if ((vga_x >= mouse_x - cursor_size) && (vga_x <= mouse_x + cursor_size) && (vga_y >= mouse_y - cursor_size) && (vga_y <= mouse_y + cursor_size)){
        // if it is inside the mouse square
        *clk_en = 1;
    }
    else {
        // if it is outside the mouse square
        *clk_en = 0;
    }
}


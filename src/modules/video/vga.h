/****************************************************************************
FILE          : vga.h
LAST REVISION : 2008-04-16
SUBJECT       : Phoenix VGA
PROGRAMMER    : (C) Copyright 2008  The Phoenix Team.

This header file describes the Phoenix VGA system.

Please send comments or bug reports to

    Phoenix Team
    c/o Peter C. Chapin
    Vermont Technical College
    Randolph Center, VT 05061
    Peter.Chapin@vtc.vsc.edu
****************************************************************************/

#ifndef VGA_H
#define VGA_H

#include <stddef.h>

#include <Phoenix.h>
#include "images.h"

#define VGA_MODE            0x13      /* The mode used for VGA with 256 colors. */

#define VIDEO_INT           0x10      /* The BIOS video interrupt. */
#define SET_CURSOR_POS      0x02      /* BIOS func to set the cursor position. */  
#define WRITE_STRING        0x13      /* BIOS func to write a string on the screen. */

#define SCREEN_WIDTH        320       /* width in pixels of mode 0x13 */
#define SCREEN_HEIGHT       200       /* height in pixels of mode 0x13 */
#define NUM_COLORS          256       /* number of colors in mode 0x13 */

// Prints a character string to a specific location with color information (used in mode 0x13);
void vga_print_at( byte row, byte column, const char *message, byte color );

// Plot a pixel by directly writing to video memory. 
void put_pixel( int x, int y, byte color );

// Get the color of the pixel at this location.
byte get_pixel( int x, int y );

// Draws a line using Bresenham's line-drawing algorithm, which uses no multiplication or division.
void line( int x1, int y1, int x2, int y2, byte color );

// Draws a polygon using the line function.
void polygon( int num_vertices, int *vertices, byte color );

// Draws a rectangle by drawing all lines by itself.
void rect( int left,int top, int right, int bottom, byte color );

// Draws and fills a rectangle.
void rect_fill( int left,int top, int right, int bottom, byte color );

// Draws a circle
void circle( int xCenter, int yCenter, int radius, byte color );

// Calles a recursive fill function using color of target pixel as the empty color.
void fill( int x, int y, byte color );

// Recursive fill using the boundary-fill algorithm.
void boundary_fill( int x, int y, byte color, byte empty_color );

// Take in x and y coordinates and an image name.
// Display a bmp image array (use the tool bmp2hex.c).
void put_image( int x, int y, const char *name );

// Take in x and y coordinates, a transparent color and an image name.
// Display a bmp image array without displaying the transparent color.
void put_trans_image( int x, int y, byte trans_color, const char *name );

#endif


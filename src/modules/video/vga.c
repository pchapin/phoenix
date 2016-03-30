/****************************************************************************
FILE          : vga.c
LAST REVISION : 2008-04-16
SUBJECT       : Phoenix VGA
PROGRAMMER    : (C) Copyright 2008  The Phoenix Team.

This is the implementation of the Phoenix VGA system.

Please send comments or bug reports to

    Phoenix Team
    c/o Peter C. Chapin
    Vermont Technical College
    Randolph Center, VT 05061
    Peter.Chapin@vtc.vsc.edu
****************************************************************************/

#include <string.h>

#include <Phoenix.h>
#include "vga.h"

/*! \file vga.c Provides VGA graphics support functions.
 *
 * This file contains a collection of functions that can be used to assist in displaying VGA
 * graphics. The mode must be set to VGA_MODE in order for the graphics to be displayed.
 */


//! Print a string in VGA_MODE at a specific location
/*!
 * The given string must be null terminated.
 *
 * \param row The row position.
 * \param column The column position.
 * \param message The message string.
 * \param color The color used for the text.
 */
void vga_print_at( byte row, byte column, const char *message, byte color )
{
    word length;

    length = strlen( message );
    _asm {
        push bp
        mov al, 1
        mov bh, 0
        mov bl, color
        mov cx, length
        mov dl, column
        mov dh, row
        push ds
        pop es
        mov bp, message
        mov ah, 13h
        int 10h
        pop bp
    };
}


//! Plot a pixel by directly writing to video memory.
/*!
 * Based on code by David Brackeen.
 *
 * \param x The x position (0 to 319).
 * \param y The y position (0 to 199).
 * \param color The color of the pixel.
 */
void put_pixel( int x, int y, byte color )
{
    byte far *VGA = MK_FP( 0xA000, 0x0000 );
    // VGA[y*SCREEN_WIDTH + x] = color;
    VGA[(y << 8) + (y << 6) + x] = color;
}


//! Get the color of the pixel at a specific location.
/*!
 * \param x The x position of the pixel (0 to 319).
 * \param y The y position of the pixel (0 to 199).
 */
byte get_pixel( int x, int y )
{
    byte far *VGA = MK_FP( 0xA000, 0x0000 );
    return VGA[(y << 8) + (y << 6) + x];
}


//! Draws a line between two points
/*!
 * Uses Bresenham's line-drawing algorithm, which uses no multiplication or division. Based on
 * code by David Brackeen.
 *
 * \param x1 The x position of the first point (0 to 319).
 * \param y1 The y position of the first point (0 to 199).
 * \param x2 The x position of the second point (0 to 319).
 * \param y2 The y position of the second point (0 to 199).
 * \param color The color of the line.
 */
void line( int x1, int y1, int x2, int y2, byte color )
{
    int i, dx, dy, sdx, sdy, dxabs, dyabs, x, y, px, py;

    dx    = x2 - x1;      /* the horizontal distance of the line */
    dy    = y2 - y1;      /* the vertical distance of the line */
    dxabs = abs( dx );
    dyabs = abs( dy );
    sdx   = sgn( dx );
    sdy   = sgn( dy );
    x     = dyabs >> 1;
    y     = dxabs >> 1;
    px    = x1;
    py    = y1;

    put_pixel( px, py, color );

    if( dxabs >= dyabs ) { /* the line is more horizontal than vertical */
        for( i = 0; i < dxabs; i++ ) {
            y += dyabs;
            if( y >= dxabs ) {
                y -= dxabs;
                py += sdy;
            }
            px += sdx;
            put_pixel( px, py, color );
        }
    }
    else  { /* the line is more vertical than horizontal */
        for( i = 0; i < dyabs; i++ ) {
            x += dxabs;
            if( x >= dyabs ) {
                x -= dyabs;
                px += sdx;
            }
            py += sdy;
            put_pixel( px, py, color );
        }
    }
}


//! Draws a polygon outline.
/*!
 * Based on code by David Brackeen.
 *
 * \param num_vertices The number of vertices of the polygon.
 * \param vertices A pointer to an array of vertices. (e.g. {x1, y1, x2, y2, x3, y3})
 * \param color The color of the lines.
 */
void polygon( int num_vertices, int *vertices, byte color )
{
    int i;

    for( i = 0; i < num_vertices - 1; i++ ) {
        line( vertices[(i << 1) + 0],
              vertices[(i << 1) + 1],
              vertices[(i << 1) + 2],
              vertices[(i << 1) + 3],
              color );
    }

    line( vertices[0],
          vertices[1],
          vertices[(num_vertices << 1) - 2],
          vertices[(num_vertices << 1) - 1],
          color );
}


//! Draws the outline of a rectangle
/*!
 * Based on code by David Brackeen.
 *
 * \param left The left side of the rectangle (0 to 319).
 * \param top The top of the rectangle (0 to 199).
 * \param right The right side of the rectangle (0 to 319).
 * \param bottom The bottom of the rectangle (0 to 199).
 * \param color The color of the rectangle.
 */
void rect( int left, int top, int right, int bottom, byte color )
{
    byte far *VGA = MK_FP( 0xA000, 0x0000 );

    word top_offset, bottom_offset, i, temp;

    if( top > bottom ) {
        temp = top;
        top = bottom;
        bottom = temp;
    }
    if( left > right ) {
        temp = left;
        left = right;
        right = temp;
    }

    top_offset = (top << 8) + (top << 6);
    bottom_offset = (bottom << 8) + (bottom << 6);

    for( i = left; i <= right; i++ ) {
        VGA[top_offset + i] = color;
        VGA[bottom_offset + i] = color;
    }
    for( i = top_offset; i <= bottom_offset; i += SCREEN_WIDTH ) {
        VGA[left + i] = color;
        VGA[right + i] = color;
    }
}


//! Draws filled rectangle
/*!
 * Based on code by David Brackeen.
 *
 * \param left The left side of the rectangle (0 to 319).
 * \param top The top of the rectangle (0 to 199).
 * \param right The right side of the rectangle (0 to 319).
 * \param bottom The bottom of the rectangle (0 to 199).
 * \param color The color of the rectangle.
 */
void rect_fill( int left, int top, int right, int bottom, byte color )
{
    byte far *VGA = MK_FP( 0xA000, 0x0000 );
    word top_offset, bottom_offset, i, j, temp;

    if( top > bottom ) {
        temp = top;
        top = bottom;
        bottom = temp;
    }
    if( left > right ) {
        temp = left;
        left = right;
        right = temp;
    }

    top_offset = (top << 8) + (top << 6);
    bottom_offset = (bottom << 8) + (bottom << 6);

    for( i = top_offset; i <= bottom_offset; i += SCREEN_WIDTH ) {
        for( j = left; j <= right; j++ ) {
            VGA[i + j] = color;
        }
    }
}


//! Draws points on a circle
/*!
 * This should not be called by the user. Based on code by Leonard McMillan.
 *
 * \param cx The x position of the circle's center (0 to 319).
 * \param cy The y position of the circle's center (0 to 199).
 * \param x The x position (0 to 319).
 * \param y The y position (0 to 199).
 * \param color The color of the circle.
 */
void circlePoints( int cx, int cy, int x, int y, byte color )
{
    if( x == 0 ) {
        put_pixel( cx, cy + y, color );
        put_pixel( cx, cy - y, color );
        put_pixel( cx + y, cy, color );
        put_pixel( cx - y, cy, color );
    }
    else if( x == y ) {
        put_pixel( cx + x, cy + y, color );
        put_pixel( cx - x, cy + y, color );
        put_pixel( cx + x, cy - y, color );
        put_pixel( cx - x, cy - y, color );
    }
    else if( x < y ) {
        put_pixel( cx + x, cy + y, color );
        put_pixel( cx - x, cy + y, color );
        put_pixel( cx + x, cy - y, color );
        put_pixel( cx - x, cy - y, color );
        put_pixel( cx + y, cy + x, color );
        put_pixel( cx - y, cy + x, color );
        put_pixel( cx + y, cy - x, color );
        put_pixel( cx - y, cy - x, color );
    }
}


//! Draws the outline of a circle
/*!
 * Based on code by Leonard McMillan.
 *
 * \param xCenter The x position of the circle's center (0 to 319).
 * \param yCenter The y position of the circle's center (0 to 199).
 * \param radius The radius of the circle.
 * \param color The color of the circle.
 */
void circle( int xCenter, int yCenter, int radius, byte color )
{
    int x = 0;
    int y = radius;
    int p = (5 - radius * 4) / 4;

    circlePoints( xCenter, yCenter, x, y, color );
    while( x < y ) {
        x++;
        if( p < 0 ) {
            p += 2*x+1;
        }
        else {
            y--;
            p += 2*(x - y) + 1;
        }
        circlePoints( xCenter, yCenter, x, y, color );
    }
}


//! Fills an area
/*!
 * Calls a recursive fill function using color of target pixel as the empty color.
 *
 * \param x The x position of the circle's center (0 to 319).
 * \param y The y position of the circle's center (0 to 199).
 * \param color The color of the circle.
 */
void fill(int x, int y, byte color)
{
    byte far *VGA = MK_FP( 0xA000, 0x0000 );
    boundary_fill( x, y, color, VGA[y * SCREEN_WIDTH + x] );
}


//! Fills an area using an empty color
/*!
 * Recursive fill using the boundary-fill algorithm. This function should not be called by the
 * user.
 *
 * \param x The x position of the fill (0 to 319).
 * \param y The y position of the fill (0 to 199).
 * \param color The fill color.
 * \param empty_color The color of the area to be filled.
 */
void boundary_fill( int x, int y, byte color, byte empty_color )
{
    if( x > SCREEN_HEIGHT || x < 0 ) { return; }
    if( y > SCREEN_WIDTH  || y < 0 ) { return; }
    if( get_pixel(x, y) == empty_color ) {
        put_pixel(x, y, color);
        boundary_fill( x + 1, y    , color, empty_color );
        boundary_fill( x    , y - 1, color, empty_color );
        boundary_fill( x - 1, y    , color, empty_color );
        boundary_fill( x    , y + 1, color, empty_color );
    }
}


//! Sets the color palette for an image to use.
/*!
 * Based on code by David Brackeen.
 *
 * \param A pointer to a palette.
 */
void set_palette( byte *palette )
{
    int i;

    outp( 0x03C8, 0 ); // Tell the VGA that palette data is coming.
    for( i = 0; i < 256 * 3; i++ ) {
        outp( 0x03C9, palette[i] );    // Write the data.
    }
}


//! Display an image
/*!
 * Display a bmp image array. Use the tool bmp2hex and add the image to image.c.
 *
 * \param x The x position of the upper left corner (0 to 319).
 * \param y The y position of the upper left corner (0 to 199).
 * \param name A string representing the name of the image.
 */
void put_image( int x, int y, const char *name )
{
    int x_offset, y_offset;
    struct image my_image = get_image( name );

    if( my_image.data == NULL ) {
        vga_print_at( 0, 0, "Error: Invalid image name ", 0x04 );
        vga_print_at( 0, 26, name, 0x04 );
    }
    else {
        for( y_offset = 0; y_offset < my_image.height; y_offset++ ) {
            for( x_offset = 0; x_offset < my_image.width; x_offset++ ) {
                put_pixel( x + x_offset,
                           y + y_offset,
                           my_image.data[(y_offset * my_image.width) + x_offset] );
            }
        }
        if( my_image.palette != NULL ) set_palette( my_image.palette );
    }
}



//! Display an image with a transparent background
/*!
 * The color chosen as the transparent color will not be printed to the screen. The background
 * will then show through.
 *
 * \param x The x position of the upper left corner (0 to 319).
 * \param y The y position of the upper left corner (0 to 199).
 * \param trans_color The transparent color.
 * \param name A string representing the name of the image.
 */
void put_trans_image( int x, int y, byte trans_color, const char *name )
{
    int x_offset, y_offset;
    byte current_pixel;
    struct image my_image = get_image( name );

    if( my_image.data == NULL ) {
        vga_print_at( 0, 0, "Error: Invalid image name ", 0x04 );
        vga_print_at( 0, 26, name, 0x04 );
    }
    else {
        for( y_offset = 0; y_offset < my_image.height; y_offset++ ) {
            for( x_offset = 0; x_offset < my_image.width; x_offset++ ) {
                current_pixel = my_image.data[(y_offset * my_image.width) + x_offset];
                if( current_pixel != trans_color ) {
                    put_pixel( x + x_offset, y + y_offset, current_pixel );
                }
            }
        }
        if( my_image.palette != NULL ) set_palette( my_image.palette );
    }
}

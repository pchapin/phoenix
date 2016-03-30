/****************************************************************************
FILE          : video.c
LAST REVISION : 2008-01-25
SUBJECT       : Interface for Phoenix video.
PROGRAMMER    : (C) Copyright 2008  The Phoenix Team.

This file implements the Phoenix video system.

Please send comments or bug reports to

    Phoenix Team
    c/o Peter C. Chapin
    Vermont Technical College
    Randolph Center, VT 05061
    Peter.Chapin@vtc.vsc.edu
****************************************************************************/

#include <stddef.h>

#include "internal.h"
#include "video.h"
#include "vga.h"

/*! \file video.c Provides graphics support functions.
 *
 * This file contains a collection of functions that can be used to assist in screen display.
 */

//! Print a string at a specific location
/*!
 * Mode must be set to default (mode 0x03). The given string must be null terminated.
 *
 * \param row The row position.
 * \param column The column position.
 * \param message The message string.
 * \param color The color to use for the text.
 */
void print_at( int row, int column, const char *message, byte color )
{
    char far *video_buffer = MK_FP( 0xB800, 0x0000 );
    int offset;

    row = row % 25;
    column = column % 80;
  
    offset = 2 * (80 * row + column);
    while( *message ) {
        video_buffer[offset] = *message++;
        video_buffer[offset+1] = color;
        offset += 2;
    }
}


//! Sets the video mode
/*!
 * 0x03 is the default. Pass in VGA_MODE to switch to vga mode.
 *
 * \param mode The mode to switch to.
 */
void set_mode( byte mode )
{
    /*
    union REGS regs;

    regs.h.ah = SET_MODE;
    regs.h.al = mode;
    int86( VIDEO_INT, &regs, &regs );
    */
    _asm {
        mov ah, SET_MODE
        int VIDEO_INT
    };
}


//! Clears the screen
/*!
 * This will find the current mode being used and switch to it. Since a BIOS call to change the
 * mode also clears the screen by default, this is an easy way to have this accomplished.
 */
void clear_screen( )
{
    /*
    union REGS regs;
    byte mode;

    regs.h.ah = 0x0F;
    mode = int86( VIDEO_INT, &regs, &regs );
    regs.h.ah = SET_MODE;
    regs.h.al = mode;
    int86( VIDEO_INT, &regs, &regs );
    */
    _asm {
        mov ah, GET_MODE
        int VIDEO_INT
        mov ah, SET_MODE
        int VIDEO_INT
    };
}


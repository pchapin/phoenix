/****************************************************************************
FILE          : debug.c
LAST REVISION : 2008-04-19
SUBJECT       : Implemenation Phoenix debug functions.
PROGRAMMER    : (C) Copyright 2008  The Phoenix Team.

This file implements Phoenix debugging functions.

Please send comments or bug reports to

    Phoenix Team
    c/o Peter C. Chapin
    Vermont Technical College
    Randolph Center, VT 05061
    Peter.Chapin@vtc.vsc.edu
****************************************************************************/

#include "video.h"
#include "xvideo.h"

unsigned int debug_counter = 0;

void debug_print( int row, int column, const char *message, byte color )
{
    #ifdef DEBUG
        print_at( row, column, message, color );
    #endif
}


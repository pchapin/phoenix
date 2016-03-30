/****************************************************************************
FILE          : debug.h
LAST REVISION : 2008-04-19
SUBJECT       : Declarations of Phoenix debug functions.
PROGRAMMER    : (C) Copyright 2008  The Phoenix Team.

This file declares Phoenix debugging functions.

Please send comments or bug reports to

    Phoenix Team
    c/o Peter C. Chapin
    Vermont Technical College
    Randolph Center, VT 05061
    Peter.Chapin@vtc.vsc.edu
****************************************************************************/

#ifndef DEBUG_H
#define DEBUG_H

extern int debug_counter;

// Prints a character string to a specific location with color information, when in debug mode
void debug_print( int row, int column, const char *message, byte color );

#endif


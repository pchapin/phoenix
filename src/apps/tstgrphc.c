/****************************************************************************
FILE      : tstgraphc.c
SUBJECT   : Tests parts of graphics system
PROGRAMMER: (C) Copyright 2008  The Phoenix Team.

Tests parts of graphics system.

Please send comments or bug reports to

    Phoenix Team
    c/o Peter C. Chapin
    Vermont Technical College
    Williston, VT 05495
    PChapin@vtc.vsc.edu
****************************************************************************/

#include <stddef.h>

#include <Phoenix.h>
#include "debug.h"
#include "vga.h"
#include "video.h"
#include "xvideo.h"

#define TRACE_COLUMN 60
#define COLOR 0x04

int trace_counter = 0;

void *test( void );

int main( void )
{
    processID vidid;
    processID testid;

    vidid.pid  = VIDEO_PID;
    testid.pid = MAIN_PID;
	
    xthread_create( vidid, run_video, NORMAL_PRIORITY );
    xthread_create( testid, test, NORMAL_PRIORITY );
    start_phoenix( );
    return 0;
}


void *test( void )
{
    int row    = 0;
    int column = 0;
    byte color = 0x01;
  
    set_mode( 0x13 );
  
    xvideo_line( 40, 40, 100, 60, 6 );
    xvideo_rect_fill( 30, 160, 50, 178, 2 );
  
    while( 1 ) {
        if( row < 0 ) {
            row = 0;
        }
  
        xvideo_put_pixel( column, row, color );
        //put_pixel( column, row, color );
    
        row++;
        if( row > 199 ) {
            row = 0;
            column++;
            if( column > 319 ) {
                column = 0;
                color--;
            }
        }
        color++;
    }
    return NULL;
}

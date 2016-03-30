/****************************************************************************
FILE      : anima.c
SUBJECT   : A running animation demo.
PROGRAMMER: (C) Copyright 2008  The Phoenix Team.

This is to demo animation in VGA using put image.

Please send comments or bug reports to

    Phoenix Team
    c/o Peter C. Chapin
    Vermont Technical College
    Williston, VT 05495
    PChapin@vtc.vsc.edu
****************************************************************************/

#include <Phoenix.h>
#include "beep.h"
#include "keyboard.h"  // Needed for run_keyboard( )
#include "xkeyboard.h"
#include "vga.h"
#include "video.h"     // Needed for run_video( )
#include "xvideo.h"

#define TRACE_COLUMN 60
#define COLOR 0x04

int trace_counter = 0;

void *main_thread( void );

int main( void )
{
    processID vidid;
    processID keyid;
    processID mainid;

    vidid.pid  = VIDEO_PID;
    keyid.pid  = KEYBOARD_PID;
    mainid.pid = MAIN_PID;
	
    xthread_create( vidid, run_video, NORMAL_PRIORITY );
    xthread_create( keyid, run_keyboard, NORMAL_PRIORITY );
    xthread_create( mainid, main_thread, NORMAL_PRIORITY );
    start_phoenix( );
    return 0;
}


// This is a startup intro for the console
void intro( )
{
    // The palette will change when pu_image is called, so the color values have to be based on
    // that palette.
    xvideo_rect_fill( 0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1, 0xCD );
    xvideo_put_image( 120, 60, "phoenix" );
    xvideo_circle( 160, 95, 60, 0xAC );
    beep(  C5 );
    beep( A4S );
    beep(  F4 );
    beep(  C5 );
    beep( A4S );
    beep(  F4 );
    beep(  C5 );
    beep( A4S );
    beep(  F4 );
    beep(  C5 );
    beep( A4S );
    beep(  F4 );
    beep(  C5 );
    beep(  C5 );
    beep(  C5 );
    
    while( xkeyboard_getKey( ) == 0 ) { }
    xvideo_clear_screen( );
}


void *main_thread( void )
{
    unsigned int i, j, frame = 0, bgColor = 230, counter = 0, cycleOn = 0;
  
    set_mode( VGA_MODE );
  
    intro( );
    xvideo_rect_fill( 0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1, bgColor );
  
    while( 1 ) {
        if( xkeyboard_getKey( ) != 0 ) cycleOn = !cycleOn;
        xvideo_rect_fill( 144, 84, 176, 116, bgColor );
        switch( frame ) {
        case 0:
            xvideo_put_trans_image( 144, 84, 0x6B, "run1" );
            break;
        case 1:
            xvideo_put_trans_image( 144, 84, 0x6B, "run2" );
            break;
        case 2:
            xvideo_put_trans_image( 144, 84, 0x6B, "run3" );
            break;
        case 3:
            xvideo_put_trans_image( 144, 84, 0x6B, "run4" );
            break;
        case 4:
            xvideo_put_trans_image( 144, 84, 0x6B, "run5" );
            break;
        case 5:
            xvideo_put_trans_image( 144, 84, 0x6B, "run6" );
            break;
        case 6:
            xvideo_put_trans_image( 144, 84, 0x6B, "run7" );
            break;
        case 7:
            xvideo_put_trans_image( 144, 84, 0x6B, "run8" );
            break;
        }
        frame++;
        if( frame > 7 ) frame = 0;
    
        // Delay.
        for( i = 0; i < 4000; i++ ) {
            for( j = 0; j < 5000; j++ ) { }
        }
    
        if( cycleOn ) {
            counter++;
            if( counter == 16 ) {
                counter = 0;
                bgColor++;
                if( bgColor > 255 ) {
                    bgColor = 230;
                }
                xvideo_rect_fill( 0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1, bgColor );
            }
        }
    }
    return NULL;
}

/****************************************************************************
FILE      : tstkybrd.c
SUBJECT   : Tests message passing to the keyboard module.
PROGRAMMER: (C) Copyright 2008  The Phoenix Team.

This basic test will test message passing functionality by remotely using the video module.

Please send comments or bug reports to

    Phoenix Team
    c/o Peter C. Chapin
    Vermont Technical College
    Williston, VT 05495
    PChapin@vtc.vsc.edu
****************************************************************************/

#include <stddef.h>

#include <Phoenix.h>
#include "keyboard.h"
#include "video.h"
#include "xvideo.h"

#define TRACE_COLUMN   60
#define COLOR        0x04

int trace_counter = 0;
unsigned int row = 0;

void *test( void );

int main( void )
{
    processID keyboardid;
    processID testid;

    keyboardid.pid = KEYBOARD_PID;
    testid.pid     = MAIN_PID;
	
    xthread_create( keyboardid, run_keyboard, NORMAL_PRIORITY );
    xthread_create( testid, test, NORMAL_PRIORITY );
    start_phoenix( );
    return 0;
}


void keyboard_test_message( )
{  
    message testKeyboard;
    message result;
    char    testBuff[128];
    char   *temp = testBuff;
    int     size = 0;
    keyboard_func_t function;
   
    function = GETNEWKEYS;
   
    // load function name
    // cast as int to use a fixed width
    temp = add_int_to_message( function, temp );
    size += 2;
    
    testKeyboard.src.pid = MAIN_PID;
    testKeyboard.dest.pid = KEYBOARD_PID;
    testKeyboard.size = size;
    testKeyboard.data = &testBuff;
   
    result.src.pid = KEYBOARD_PID;
    result.dest.pid = MAIN_PID;
   
    message_send( &testKeyboard, &result );
   
    if( result.data ) {
        xvideo_print_at( row++, 1, result.data, 0x04 );
    }
    else {
        xvideo_print_at( row++, 2, "NULL", 0x02 );
    }
}


void *test( void )
{
    xvideo_clear_screen( );  
    while( 1 ) {
        keyboard_test_message( );
    }
    return NULL;
}

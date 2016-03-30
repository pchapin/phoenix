/****************************************************************************
FILE      : tstmutex.c
SUBJECT   : Tests basic mutex usage.
PROGRAMMER: (C) Copyright 2008  The Phoenix Team.

This test will exercise the mutexes and condition variables. This functionality is basic to the
thread support and must work correctly before the message passing system will work.

Please send comments or bug reports to

    Phoenix Team
    c/o Peter C. Chapin
    Vermont Technical College
    Williston, VT 05495
    PChapin@vtc.vsc.edu
****************************************************************************/

#include <stddef.h>
#include <string.h>

#include <Phoenix.h>
#include "video.h"   // Needed for run_video( )
#include "xvideo.h"

#define TRACE_COLUMN 20
#define OUTPUT_COLUMN 10
#define COLOR 0x04

int trace_counter = 0;

xbuffer message_buffer;

// Thread functions.
void *thread1( void );
void *thread2( void );
processID id1;
processID id2;

int main( void )
{  
    id1.pid = MAIN_PID;
    id2.pid = MAIN_PID - 1;
	
    xbuffer_init( &message_buffer );

    xthread_create( id1, thread1, NORMAL_PRIORITY );
    xthread_create( id2, thread2, NORMAL_PRIORITY );
    start_phoenix( );
    return 0;
}


// Thread 1
void *thread1( void )
{
    message  send;
    message *receive;
    char     buffer[32];

    xvideo_clear_screen( );

    send.src  = id1;
    send.dest = id2;
  
    strcpy( &buffer, "XBuffer push and pop" );
    send.data = &buffer;
    send.size = 21;
  
    xbuffer_push( &message_buffer, &send, id1 );
  
    xvideo_print_at( trace_counter++, TRACE_COLUMN, "Buffer pushed", COLOR );
  
    xthread_suspend( id1 );
    xthread_switch_thread( );
  
    receive = xbuffer_pop( &message_buffer, &id2, &id1 );
    xvideo_print_at( trace_counter++, TRACE_COLUMN, "Buffer popped", COLOR );
    xvideo_print_at( trace_counter++, TRACE_COLUMN, receive->data, COLOR );
    xvideo_print_at( trace_counter++, TRACE_COLUMN, "Eternal Loopiness", COLOR );
  
    while( 1 ) { }
    return NULL;
}


// Thread 2
void *thread2( void )
{
    message  send;
    message *receive;
    char     buffer[32];

    send.src  = id2;
    send.dest = id1;
  
    receive = xbuffer_pop( &message_buffer, &id1, &id2 );
    xvideo_print_at( trace_counter++, TRACE_COLUMN, "Buffer popped", COLOR );
    xvideo_print_at( trace_counter++, TRACE_COLUMN, receive->data, COLOR );
  
    strcpy( &buffer, "appears to be working" );
    send.data = &buffer;
    send.size = 22;
  
    xbuffer_push( &message_buffer, &send, id2 );
    xvideo_print_at( trace_counter++, TRACE_COLUMN, "Buffer pushed", COLOR );
    xthread_suspend( id2 );
    xthread_resume( id1 );
    xthread_switch_thread( );
    return NULL;
}

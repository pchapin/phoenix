/****************************************************************************
FILE      : sprmutex.c
SUBJECT   : Tests thread creation and context switching.
PROGRAMMER: (C) Copyright 2008  The Phoenix Team.

This test will exercise the mutexes by quickly locking and unlocking in two threads. This
functionality is basic to the thread support and must work correctly before the message passing
system will work.

Please send comments or bug reports to

    Phoenix Team
    c/o Peter C. Chapin
    Vermont Technical College
    Williston, VT 05495
    PChapin@vtc.vsc.edu
****************************************************************************/

#include <stddef.h>

#include <Phoenix.h>
#include "video.h"   // Needed for run_video( )
#include "xvideo.h"

#define TRACE_COLUMN    60
#define OUTPUT_COLUMN   10
#define COLOR         0x04

int trace_counter  = 0;
int output_counter = 0;

int mutex_state = 0;

xthread_mutex_t mutex;

// Thread functions.
void *thread1( void );
void *thread2( void );
processID id1;
processID id2;

int main( void )
{  
    id1.pid = MAIN_PID;
    id2.pid = MAIN_PID - 1;
	
    xthread_mutex_init( &mutex );
    xthread_create( id1, thread1, NORMAL_PRIORITY );
    xthread_create( id2, thread2, NORMAL_PRIORITY );
    start_phoenix( );
    return 0;
}


// Thread 1
void *thread1( void )
{
    xvideo_clear_screen( );

    while( 1 ) {
        xthread_mutex_lock( &mutex, id1 );
        if( mutex_state != 0 ) {
            xvideo_print_at( 0, TRACE_COLUMN, "BAD THINGS!!!", COLOR );
        }
	
        mutex_state = 1;
        xvideo_print_at( trace_counter++, 0, "Thread1 got lock", COLOR );
        mutex_state = 0;
	
        xthread_mutex_unlock( &mutex );
    }
    return NULL;
}


// Thread 2
void *thread2( void )
{
    while( 1 ) {
        xthread_mutex_lock( &mutex, id2 );
        if( mutex_state != 0 ) {
            xvideo_print_at( 0, TRACE_COLUMN, "BAD THINGS!!!", COLOR );
        }
	
        mutex_state = 2;
        xvideo_print_at( trace_counter++, 18, "Thread2 got lock", COLOR );
        mutex_state = 0;
	
        xthread_mutex_unlock( &mutex );
    }
    return NULL;
}


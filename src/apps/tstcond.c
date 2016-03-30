/****************************************************************************
FILE      : tstcond.c
SUBJECT   : Tests thread creation and context switching.
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

#include <Phoenix.h>
#include "video.h"
#include "xvideo.h"

#define TRACE_COLUMN    60
#define OUTPUT_COLUMN   10
#define COLOR         0x04

int trace_counter = 0;
int output_counter = 0;

int mutex_state;

xthread_cond_t cond;
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
    xthread_cond_init( &cond );
    xthread_create( id1, thread1, NORMAL_PRIORITY );
    xthread_create( id2, thread2, NORMAL_PRIORITY ); 
    mutex_state = 0;
    start_phoenix( );
    return 0;
}


// Thread 1
void *thread1( void )
{
    while( 1 ) {
        xthread_mutex_lock( &mutex, id1 );
    
        xvideo_print_at( trace_counter++, 0, "Waiting", COLOR );
        xthread_cond_wait( &cond, &mutex, id1 );
  
        if( mutex_state == 0 ) {
            xvideo_print_at( trace_counter++, 40, "BAD THINGS!!!", COLOR );
        }

        mutex_state = 0;
        xvideo_print_at( output_counter++, 0, "Setting state to 0", COLOR );
    
        xthread_mutex_unlock( &mutex );
    }
    return NULL;
}


// Thread 2
void *thread2( void )
{
    int result;

    while( 1 ) {
        xthread_mutex_lock( &mutex, id2 );
        if( mutex_state == 0 ) {
            mutex_state = 1;
            xvideo_print_at( output_counter++, 20, "Setting state to 1", COLOR );
        }
        xthread_mutex_unlock( &mutex );
        result = xthread_cond_signal( &cond );
    }
    return NULL;
}


/*!
 * \file  Phoenix.c
 * \brief Start up function and other useful tidbits.
 *
 */

#include "kernel.h"

//! Start Phoenix scheduling.
/*!
 * This function creates the special idle thread (at PID 0 with IDLE_PRIORITY) and then starts
 * Phoenix scheduling. This function should only be called from load context. It never returns.
 * Once Phoenix scheduling begins, all further activity is in Phoenix threads. It is permitted
 * (even necessary) to use xthread_create( ) to create one or more Phoenix threads before
 * calling this function. However, those threads will not execute until this function is called.
 */
void start_phoenix( void )
{
    // TODO: This function should really return an error code.

    message_init( );
    create_idle_thread( );  // Be sure the thread exists before we activate context switching.
    disable_interrupts( );
    initialize_timer_frequency( );
    initialize_timerISR( );
    enable_interrupts( );

    // Wait for a timer tick. This thread should never resume.
    while( 1 ) { }
}

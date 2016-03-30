/*!
 * \file  xtimer.c
 * \brief Timer handling functions.
 *
 */

#include "kernel.h"

// The following functions are written in assembly language.
extern void create_process_asm( void *(*start_routine)( void ), word *stack );
extern void __interrupt timer_ISR_asm( void );

static void *idle_thread( void );
static void (__interrupt * __far * __far IVT)( void );

// static unsigned long deadbeef_seed = 0;
// static unsigned long deadbeef_beef = 0xdeadbeef;
static bool first_time = true;

// Timer interrupt function.
word far *schedule( word far *p )
{
    process   *current_process;
    process   *next_process;
    priority_t priority_level;
    bool       found;

    // When the scheduler is called for the first time a non-Phoenix thread will be running.
    // Thus there is no "current process." To work around this just run the idle process for one
    // timer tick. Some other thread will take over at the next interrupt. Using the idle
    // process here is a bit of a hack but it's the only process we know we have.
    //
    if( first_time ) {
        processID idle_process;
        idle_process.pid = IDLE_PID;
        current_process = get_process( idle_process );
        set_current( current_process );
        first_time = false;
        return current_process->stack;
    }

    // Normal operation...
    // Get the current Phoenix process and search for a new one to schedule.
    //
    current_process = get_current( );
    current_process->stack = p;

    // Search for next runnable process.
    found = false;
    for( priority_level = HIGH_PRIORITY; priority_level >= 0; --priority_level ) {
        next_process = get_next( current_process );
        while( !found ) {
            if( next_process->runnable && next_process->priority == priority_level ) {
                found = true;
                break;
            }
            if( next_process == current_process ) break;
            next_process = get_next( next_process );
        }

        if( found ) break;
    }

    // We should find a runnable process because the idle process is always runnable and the
    // loop above will stumble into it eventually. Note that the idle process should be the
    // only process at the IDLE_PRIORITY level. 

    set_current( next_process );
    return next_process->stack;
}


// Initialize the timer interrupt.
void initialize_timerISR( void )
{
    IVT = MK_FP( 0, 0 );
    IVT[TimerIRQ] = timer_ISR_asm;
}


// Initialize_timer_frequency to ~2.8ms.
void initialize_timer_frequency( void )
{
    outp( 0x42, 0x3c );
    outp( 0x40, 0x00 );
    outp( 0x40, 0x10 );
}


//reset the interrupt vector back to normal
/*
void reset_timer_interrupt( void )
{
  _dos_setvect( TimerIRQ, Old_TimerISR );
  outp( 0x43, 0x3c );
  outp( 0x40, LOW_BYTE(0xFFFF) );
  outp( 0x40, HIGH_BYTE(0xFFFF) );
}
*/

// based on Robert Haarman's deadbeef prng
// http://inglorion.net/software/deadbeef_rand/
unsigned int get_random( void )
{
/*
    if( deadbeef_seed == 0 ) {
        deadbeef_seed  = random_seed( );
        deadbeef_seed *= random_seed( );
        deadbeef_seed *= random_seed( );
    }

    deadbeef_seed = (deadbeef_seed << 7) ^ ((deadbeef_seed >> 25) + deadbeef_beef);
    deadbeef_beef = (deadbeef_beef << 7) ^ ((deadbeef_beef >> 25) + 0xdeadbeef);

    return (unsigned int)deadbeef_seed;
*/
    return 0;
}


int random_seed( )
{
    return (int)inp( 0x40 );
}

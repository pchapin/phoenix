/*!
 * \file  kernel.h
 * \brief Internal kernel header
 *
 * This file contains declarations used by the Phoenix kernel internally. Applications should
 * never include this header (directly). Kernel files should only include this header. In
 * particular kernel files should *not* include standard C library headers or Open Watcom
 * specific headers.
 */

#ifndef KERNEL_H
#define KERNEL_H

// The kernel knows and can use the public API.
#include "Phoenix.h"

#define NULL ((void *)0)

int  message_init( void );
void disable_interrupts( void );
void enable_interrupts( void );
void create_idle_thread( void );

// XRndbuff
// --------

typedef struct {
    word  far *stack;
    bool       runnable;
    processID  pid;
    priority_t priority;
} process;

int      add_process( process * );
process *get_current( );
process *get_next( process * );
void     set_current( process * );
process *get_process( processID );

// Timer
// -----

#define LOW_BYTE(n) (n & 0x00FF)
#define HIGH_BYTE(n) ((n >> 8) & 0x00FF)
#define TimerIRQ 0x08

void initialize_timerISR( );
void initialize_timer_frequency( );
void reset_timer_interrupt( );
unsigned int get_random( );

#endif

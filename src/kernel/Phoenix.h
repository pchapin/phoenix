/*!
 * \file  Phoenix.h
 * \brief Application interface to the Phoenix kernel.
 *
 * This file contains the public facing API of the Phoenix kernel. Phoenix applications should
 * only include this header. No other kernel headers should be (directly) included.
 */

#ifndef PHOENIX_H
#define PHOENIX_H

// Types
// -----

typedef unsigned char  byte;
typedef unsigned short word;

#ifndef __cplusplus
typedef enum { false = 0, true = 1 } bool;
#endif

// Start Up
// --------

void start_phoenix( void );

// XThreads
// --------

#define MAX_THREADS 16                   //!< Maximum number of threads in the OS.
#define IDLE_PID     0                   //!< Process ID of the main thread.
#define MAIN_PID     (MAX_THREADS - 1)   //!< Process ID of the main thread.

// Thread priority levels. Applications should not use IDLE_PRIORITY.
typedef enum { IDLE_PRIORITY, LOW_PRIORITY, NORMAL_PRIORITY, HIGH_PRIORITY } priority_t;

// Make sure all members of the following structures are byte aligned. These structures are
// accessed from assembly language in xt_asm.asm (in the kernel) and that code depends on
// these structures being layed out a certain way.
//
// NOTE: It does not appear that any of these structures are used in xt_asm.asm so perhaps the
// comment above is obsolete and specific packing is no longer required here. --pchapin.
//
#pragma pack(push, 1);
typedef struct {
    int pid;
} processID;

typedef struct {
    bool locked;  // False if available, true if locked.
    bool waiting[MAX_THREADS];
    int  waiting_index;
} xthread_mutex_t;

typedef struct {
    bool waiting[MAX_THREADS];
    int  waiting_index;
} xthread_cond_t;

#pragma pack(pop);
// End of structures with specific alignment requirements.

int  xthread_create( processID id, void *(*start_routine)( void ), priority_t priority );
processID xthread_get_processid( );
int  xthread_suspend( processID id );
int  xthread_resume( processID id );
void xthread_switch_thread( void );
int  xthread_mutex_init( xthread_mutex_t * );
int  xthread_mutex_lock( xthread_mutex_t *, processID );
int  xthread_mutex_unlock( xthread_mutex_t * );
int  xthread_cond_init( xthread_cond_t * );
int  xthread_cond_wait( xthread_cond_t *, xthread_mutex_t *, processID );
int  xthread_cond_signal( xthread_cond_t * );


// Message Passing
// ---------------

typedef struct {
    processID src;    //!< Thread ID where this message was created
    processID dest;   //!< Thread ID where this message is being sent
    int size;         //!< Number of bytes in message data
    char *data;       //!< Pointer to the message itself
} message;

void message_send( message *request, message *response );
void message_throw( message *request );
void message_receive( message *request );
void message_reply( message *response );
int  get_int( char *current);
char *add_int_to_message( int value, char *buff );

// XBuffer
// -------

#define XBUFFER_SIZE 8

// This is our semaphore type.
typedef struct {
    xthread_mutex_t lock;
    xthread_cond_t  non_zero;
    int             raw_count;
} semaphore;

void semaphore_init( semaphore *, int );
void semaphore_destroy( semaphore * );
void semaphore_up( semaphore *, processID );
void semaphore_down( semaphore *, processID );

// This is our producer/consumer buffer type.
typedef struct {
    message *buffer[XBUFFER_SIZE];
    xthread_mutex_t lock;
    semaphore       used;      // Use our semaphores here.
    semaphore       free;      // ...
} xbuffer;

void  xbuffer_init( xbuffer * );
void  xbuffer_destroy( xbuffer *, processID );
void  xbuffer_push( xbuffer *, message *, processID );
message *xbuffer_pop( xbuffer *, processID *, processID * );

// Low Level
// ---------

// These functions are defined in src/apps/startup.asm
unsigned int outp( int port, unsigned int value );
unsigned int inp( int port );

/* Macros to break 'far' pointers into segment and offset components. */
#define FP_OFF(__p) ((unsigned)(__p))
#define FP_SEG(__p) ((unsigned)((unsigned long)(void __far*)(__p) >> 16))

/* Make a far pointer from segment and offset. */
#define MK_FP(__s,__o) (((unsigned short)(__s)):>((void __near *)(__o)))

#endif

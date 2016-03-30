/*!
 * \file  xbuffer.c
 * \brief Producer/Consumer buffers
 *
 * Phoenix uses an xbuffer for managing message mailboxes.
 */

#include "kernel.h"

void semaphore_init( semaphore *sem, int count )
{
    xthread_mutex_init( &sem->lock );
    xthread_cond_init( &sem->non_zero );
    sem->raw_count = count;
}


void semaphore_up( semaphore *sem, processID proc )
{
    xthread_mutex_lock( &sem->lock, proc );
    ++sem->raw_count;
    xthread_cond_signal( &sem->non_zero );
    xthread_mutex_unlock( &sem->lock );
}


void semaphore_down( semaphore *sem, processID proc )
{
    xthread_mutex_lock( &sem->lock, proc );

    if( sem->raw_count == 0 ) {
        xthread_cond_wait( &sem->non_zero, &sem->lock, proc );
    }

    --sem->raw_count;
    xthread_mutex_unlock( &sem->lock );
}


void  xbuffer_init( xbuffer *buff )
{
    int i = 0;

    xthread_mutex_init( &buff->lock );
    semaphore_init( &buff->used, 0 );
    semaphore_init( &buff->free, XBUFFER_SIZE );
  
    for( i = 0; i < XBUFFER_SIZE; i++ ) {
        buff->buffer[i] = NULL;
    }
}


void xbuffer_push( xbuffer *buff, message *stuff, processID proc )
{
    int i = 0;

    semaphore_down( &buff->free, proc );
    xthread_mutex_lock( &buff->lock, proc );
  
    for( i = 0; i < XBUFFER_SIZE; i++ ) {
        if( buff->buffer[i] == NULL ) break;
    }
  
    buff->buffer[i] = stuff;
  
    xthread_mutex_unlock( &buff->lock );
    semaphore_up( &buff->used, proc );
}


// Using NULL as the source will get a message for the destination.
message *xbuffer_pop( xbuffer *buff, processID *src, processID *dest )
{
    int i = 0;
    message *result = NULL;
  
    semaphore_down( &buff->used, *dest );
    xthread_mutex_lock( &buff->lock, *dest );
  
    for( i = 0; i < XBUFFER_SIZE; i++ ) {
        if( buff->buffer[i] == NULL ) continue;
        
        if( src == NULL ) {
            if( buff->buffer[i]->dest.pid == dest->pid ) {
                result = buff->buffer[i];
                buff->buffer[i] = NULL;
                break;
            }
        }
        else if( buff->buffer[i]->src.pid  == src->pid &&
                 buff->buffer[i]->dest.pid == dest->pid ) {
            result = buff->buffer[i];
            buff->buffer[i] = NULL;
            break;
        }
    }

    xthread_mutex_unlock( &buff->lock );
    semaphore_up( &buff->free, *dest );

    return result;
}

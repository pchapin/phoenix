/*!
 * \file  xmessage.c
 * \brief Phoenix message passing
 *
 */

#include "kernel.h"

static xbuffer messages;

//! Send a message.
/*!
 * Takes a filled request message and an empty response message which has the src and dest
 * filled (opposite that of the request message).
 */
void message_send( message *request, message *response )
{
    response->src = request->dest;
    response->dest = request->src;
  
    xbuffer_push( &messages, request, request->src );
    xbuffer_push( &messages, response, request->src );
  
    disable_interrupts( );
    xthread_resume( request->dest );
    xthread_suspend( request->src );  
    enable_interrupts( );
  
    xthread_switch_thread( );
}


//! Send a message without blocking.
/*!
 * Takes a filled request message and does not wait for the request to be handled.
 */
void message_throw( message *request )
{
    processID source;
    
    source = request->src;
    request->src.pid = -1;  
  
    xbuffer_push( &messages, request, source );
    xthread_resume( request->dest );
}


//! Receive a message.
/*!
 * Takes a message with only the dest filled in
 */
void message_receive( message *request )
{
    message *temp;
    int i = 0;
    char *req_current;
    char *temp_current;

    temp = xbuffer_pop( &messages, NULL, &request->dest );
  
    if( temp == NULL ) {
        // Block the calling thread.
        xthread_suspend( request->dest );
        xthread_switch_thread( );
	
        // Try receiving again.
        message_receive( request );
        return;  // Recursively return once it really works
    }
  
    req_current = request->data;
    temp_current = temp->data;
  
    for( i = 0; i < temp->size; i++ ) {
        *req_current = *temp_current;
        req_current++;
        temp_current++;
    }
    request->size = temp->size;
    request->src = temp->src;
    request->dest = temp->dest;
}


void message_reply( message *response )
{
    message *temp;
    int i = 0;
    char *resp_current;
    char *temp_current;
  
    // Get address of the expected response.
    temp = xbuffer_pop( &messages, &response->src, &response->dest );
  
    // Fill the response with data.
    resp_current = response->data;
    temp_current = temp->data;
  
    for( i = 0; i < response->size; i++ ) {
        *temp_current = *resp_current;
        resp_current++;
        temp_current++;
    }
  
    temp->size = response->size;
  
    // Unblock the waiting thread.
    xthread_resume( temp->dest );
}


int message_init( )
{
    xbuffer_init( &messages );
    return 0;
}


int get_int( char *current )
{
    char int_str[2];
    int *result;
  
    int_str[1] = *current;
    current++;
    int_str[0] = *current;
  
    result = (int *)&int_str;
    return *result;
}


char *add_int_to_message( int value, char *buff )
{
    buff[0] = ((value >> 8) & 0x00FF);
    buff[1] = (value & 0x00FF);
    return buff + 2;
}

/****************************************************************************
FILE          : kparser.c
LAST REVISION : 2008-03-25
SUBJECT       : Message parser for the keyboard system
PROGRAMMER    : (C) Copyright 2008  The Phoenix Team.

Message parser for the keyboard system

Please send comments or bug reports to

    Phoenix Team
    c/o Peter C. Chapin
    Vermont Technical College
    Randolph Center, VT 05061
    Peter.Chapin@vtc.vsc.edu
****************************************************************************/

#include <stddef.h>
#include <string.h>

#include <Phoenix.h>
#include "internal.h"

void *run_keyboard( void )
{
    message      received;
    char         rec_buff[5120];
    message      response;
    keyboard_func_t func = K_DEFAULT;
    processID    self;
    char         *current;
    char         c;

    // Install the keyboard ISR.
    initialize_keyboardISR( );
  
    // set own pid to globally defined KEYBOARD pid
    self.pid = KEYBOARD_PID;
  
    // setup the structure for received messages
    received.data = &rec_buff;
    received.dest = self;
  
    // set the response source to us
    response.src = self;
    response.size = 0;
  
    while( 1 ) {
        //try to get a message, waits until one's available 
        message_receive( &received );
	
        if( received.src.pid != -1 ) {
    
            // point to the beginning of incoming data 
            current = received.data;

            // extract the function name
            func = get_int( current );
	
            current += 2;
    
            // attempt to run the named function
            switch( func ) {
            case GETKEY:
                c = getKey( );
                response.data = &c;
                break;
                
            case GETNEWKEYS:
                response.data = getNewkeys( );
                response.size = strlen( getNewkeys( ) );
                break;
                
            case GETKEYSTRING:
                response.data = getNewkeys( );
                break;
                
            default:
                break;
            }
    
            response.size = 2;
    
            response.dest = received.src;
            message_reply( &response );
        }
    }
    return NULL;
}


/****************************************************************************
FILE          : xkeyboard.c
LAST REVISION : 2008-01-22
SUBJECT       : Interface to Phoenix message passing.
PROGRAMMER    : (C) Copyright 2008  The Phoenix Team.

This file defines the interface to the Phoenix message passing system.

Please send comments or bug reports to

    Phoenix Team
    c/o Peter C. Chapin
    Vermont Technical College
    Randolph Center, VT 05061
    Peter.Chapin@vtc.vsc.edu
****************************************************************************/

#include <Phoenix.h>
#include "keyboard.h"
#include "xkeyboard.h"

char xkeyboard_getKey( )
{
    message request;
    message result;
    char request_buff[8];
    char result_buff;
    char *temp = &request_buff;
    int size = 0;
    keyboard_func_t function;
   
    function = GETKEY;
   
    temp = add_int_to_message( function, temp );
    size += 2;
 
    request.src = xthread_get_processid( );
    request.dest.pid = KEYBOARD_PID;
    request.size = size;
    request.data = &request_buff;
   
    result.data = &result_buff;
    result.src.pid = KEYBOARD_PID;
    result.dest = xthread_get_processid( );
   
    message_send( &request, &result ); // throw( &testVideo );
   
    return *result.data;
}


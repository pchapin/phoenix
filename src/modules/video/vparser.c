/****************************************************************************
FILE      : vparser.c
SUBJECT   : Message parser for the video system
PROGRAMMER: (C) Copyright 2008  The Phoenix Team.

Message parser for the video system

Please send comments or bug reports to

    Phoenix Team
    c/o Peter C. Chapin
    Vermont Technical College
    Williston, VT 05495
    PChapin@vtc.vsc.edu
****************************************************************************/

#include <Phoenix.h>
#include "internal.h"
#include "video.h"
#include "vga.h"
#include "debug.h"

int try_print_at( char *current );
int try_vga_print_at( char *current );
int try_put_pixel( char *current );
int try_rect_fill( char *current );
int try_line( char *current );
int try_circle( char *current );
byte try_get_pixel( char *current );
int try_put_image( char *current );
int try_put_trans_image( char *current );

unsigned int parse_counter = 0;

void *run_video( void )
{
    message      received;
    char         rec_buff[128];
    message      response;
    char         res_byte;
    video_func_t func = V_DEFAULT;
    processID    self;
    char         *current;
    int          result;
  
    // set own pid to globally defined VIDEO pid
    self.pid = VIDEO_PID;
  
    // setup the structure for received messages
    received.data = &rec_buff;
    received.dest = self;
  
    // set the response source to us
    response.src = self;
    response.size = 0;
  
    // do forever...
    while( 1 ) {
        // try to get a message, waits until one's available
        message_receive( &received );
	
        debug_print( parse_counter++, 24, "RCV", 0x05 );
    
        // point to the beginning of incoming data 
        current = received.data;
    
        // extract the function name
        func = get_int( current );
	
        current += 2;
    
        // attempt to run the named function
        switch( func ) {
        case PRINT_AT:
            result = try_print_at( current );
            if( result ) {
                debug_print( parse_counter++, 24, "BAD", 0x05 );
                res_byte = BAD_ARGS;
            }
            else {
                res_byte = OK;
            }
            break;
            
        case VGA_PRINT_AT:
            result = try_vga_print_at( current );
            if( result ) {
                debug_print( parse_counter++, 24, "BAD", 0x05 );
                res_byte = BAD_ARGS;
            }
            else {
                res_byte = OK;
            }
            break;
            
        case CLEAR_SCREEN:
            clear_screen( );
            res_byte = OK;
            break;
            
        case SET_MODE:
            set_mode( 0x13 );
            res_byte = OK;
            break;
            
        case PUT_IMAGE:
            result = try_put_image( current );
            if( result ) {
                debug_print( parse_counter++, 24, "BAD", 0x05 );
                res_byte = BAD_ARGS;
            }
            else {
                res_byte = OK;
            }
            break;
            
        case PUT_TRANS_IMAGE:
            result = try_put_trans_image( current );
            if( result ) {
                debug_print( parse_counter++, 24, "BAD", 0x05 );
                res_byte = BAD_ARGS;
            }
            else {
                res_byte = OK;
            }
            break;
            
        case PUT_PIXEL:
            result = try_put_pixel( current );
            if( result ) {
                debug_print( parse_counter++, 24, "BAD", 0x05 );
                res_byte = BAD_ARGS;
            }
            else {
                res_byte = OK;
            }
            break;
            
        case RECT_FILL:
            result = try_rect_fill( current );
            if( result ) {
                debug_print( parse_counter++, 24, "BAD", 0x05 );
                res_byte = BAD_ARGS;
            }
            else {
                res_byte = OK;
            }
            break;
            
        case LINE:
            result = try_line( current );
            if( result ) {
                debug_print( parse_counter++, 24, "BAD", 0x05 );
                res_byte = BAD_ARGS;
            }
            else {
                res_byte = OK;
            }
            break;
            
        case CIRCLE:
            result = try_circle( current );
            if( result ) {
                debug_print( parse_counter++, 24, "BAD", 0x05 );
                res_byte = BAD_ARGS;
            }
            else {
                res_byte = OK;
            }
            break;
            
        case GET_PIXEL:
            res_byte = try_get_pixel( current );
            response.data = &res_byte;
            response.size = 1;
            response.dest = received.src;
            debug_print( parse_counter++, 24, "RPY", 0x05 );
            message_throw( &response );
            break;
            
        default:
            debug_print( debug_counter++, 24, "DFT", 0x05 );
            res_byte = NOT_IMPL;
            response.size = 1;
            break;
        }
    
        response.data = &res_byte;
        response.size = 1;
    
        if( received.src.pid != -1 ) {
            response.dest = received.src;
            debug_print( parse_counter++, 24, "RPY", 0x05 );
            message_reply( &response );
        }
    }
    return NULL;
}


int try_print_at( char *current )
{
    int row;
    int column;
    int size = 2;
    int arg_size;
    char *message;
    byte color;
  
    // get the row
    arg_size = get_int( current );
    current += 2;
    size += 2;

    if( arg_size != 2 ) {
        debug_print( debug_counter++, 21, "BS1", 0x05 );
        return 1;
    }
  
    row = get_int( current );
    current += 2;
    size += 2;
  
    // get the column
    arg_size = get_int( current );
    current += 2;
    size += 2;
  
    if( arg_size != 2 ) {
        debug_print( debug_counter++, 21, "BS2", 0x05 );
        return 1;
    }
  
    column = get_int( current );
    current += 2;
  
    // get the message
    arg_size = get_int( current );
    current += 2;
  
    message = current;
    current += arg_size;
  
    // get the color
    arg_size = get_int( current );
    current += 2;
  
    if( arg_size != 1 ) {
        debug_print( debug_counter++, 21, "BS3", 0x05 );
        return 1;
    }
  
    color = (byte)*current;
    current++;
  
    debug_print( parse_counter++, 24, "PRT", 0x05 );
    print_at( row, column, message, color );
    return 0;
}


int try_vga_print_at( char *current )
{
    int row;
    int column;
    int size = 2;
    int arg_size;
    char *message;
    byte color;
  
    // get the row
    arg_size = get_int( current );
    current += 2;
    size += 2;

    if( arg_size != 2 ) {
        debug_print( debug_counter++, 21, "BS1", 0x05 );
        return 1;
    }
  
    row = get_int( current );
    current += 2;
    size += 2;
  
    // get the column
    arg_size = get_int( current );
    current += 2;
    size += 2;
  
    if( arg_size != 2 ) {
        debug_print( debug_counter++, 21, "BS2", 0x05 );
        return 1;
    }
  
    column = get_int( current );
    current += 2;
  
    // get the message
    arg_size = get_int( current );
    current += 2;
  
    message = current;
    current += arg_size;
  
    // get the color
    arg_size = get_int( current );
    current += 2;
  
    if( arg_size != 1 ) {
        debug_print( debug_counter++, 21, "BS3", 0x05 );
        return 1;
    }
  
    color = (byte)*current;
    current++;
  
    debug_print( parse_counter++, 24, "PRT", 0x05 );
    vga_print_at( row, column, message, color );
    return 0;
}


int try_put_pixel( char *current )
{
    int x;
    int y;
    int size = 2;
    int arg_size;
    byte color;
  
    // get x
    arg_size = get_int( current );
    current += 2;
    size += 2;

    if( arg_size != 2 ) {
        debug_print( debug_counter++, 21, "BS1", 0x05 );
        return 1;
    }
  
    x = get_int( current );
    current += 2;
    size += 2;
  
    // get y
    arg_size = get_int( current );
    current += 2;
    size += 2;
  
    if( arg_size != 2 ) {
        debug_print( debug_counter++, 21, "BS2", 0x05 );
        return 1;
    }
  
    y = get_int( current );
    current += 2;
  
    // get the color
    arg_size = get_int( current );
    current += 2;
  
    if( arg_size != 1 ) {
        debug_print( debug_counter++, 21, "BS3", 0x05 );
        return 1;
    }
  
    color = (byte)*current;
    current++;
  
    debug_print( parse_counter++, 24, "PIX", 0x05 );

    put_pixel( x, y, color );
    return 0;
}



int try_rect_fill( char *current )
{
    int left;
    int top;
    int right;
    int bottom;
    int size = 2;
    int arg_size;
    byte color;
  
    // get left
    arg_size = get_int( current );
    current += 2;
    size += 2;

    if( arg_size != 2 ) {
        debug_print( debug_counter++, 21, "BS1", 0x05 );
        return 1;
    }
  
    left = get_int( current );
    current += 2;
    size += 2;
  
    // get top
    arg_size = get_int( current );
    current += 2;
    size += 2;
  
    if( arg_size != 2 ) {
        debug_print( debug_counter++, 21, "BS2", 0x05 );
        return 1;
    }
  
    top = get_int( current );
    current += 2;
    size += 2;
  
    // get right
    arg_size = get_int( current );
    current += 2;
    size += 2;

    if( arg_size != 2 ) {
        debug_print( debug_counter++, 21, "BS1", 0x05 );
        return 1;
    }
  
    right = get_int( current );
    current += 2;
    size += 2;
  
    // get bottom
    arg_size = get_int( current );
    current += 2;
    size += 2;
  
    if( arg_size != 2 ) {
        debug_print( debug_counter++, 21, "BS2", 0x05 );
        return 1;
    }
  
    bottom = get_int( current );
    current += 2;
    size += 2;
  
    // get the color
    arg_size = get_int( current );
    current += 2;
  
    if( arg_size != 1 ) {
        debug_print( debug_counter++, 21, "BS3", 0x05 );
        return 1;
    }
  
    color = (byte)*current;
    current++;
  
    debug_print( parse_counter++, 24, "REC", 0x05 );
    rect_fill( left, top, right, bottom, color );
    return 0;
}


int try_line( char *current )
{
    int x1;
    int y1;
    int x2;
    int y2;
    int size = 2;
    int arg_size;
    byte color;
  
    // get left
    arg_size = get_int( current );
    current += 2;
    size += 2;

    if( arg_size != 2 ) {
        debug_print( debug_counter++, 21, "BS1", 0x05 );
        return 1;
    }
  
    x1 = get_int( current );
    current += 2;
    size += 2;
  
    // get top
    arg_size = get_int( current );
    current += 2;
    size += 2;
  
    if( arg_size != 2 ) {
        debug_print( debug_counter++, 21, "BS2", 0x05 );
        return 1;
    }
  
    y1 = get_int( current );
    current += 2;
    size += 2;
  
    // get right
    arg_size = get_int( current );
    current += 2;
    size += 2;

    if( arg_size != 2 ) {
        debug_print( debug_counter++, 21, "BS1", 0x05 );
        return 1;
    }
  
    x2 = get_int( current );
    current += 2;
    size += 2;
  
    // get bottom
    arg_size = get_int( current );
    current += 2;
    size += 2;
  
    if( arg_size != 2 ) {
        debug_print( debug_counter++, 21, "BS2", 0x05 );
        return 1;
    }
  
    y2 = get_int( current );
    current += 2;
    size += 2;
  
    // get the color
    arg_size = get_int( current );
    current += 2;
  
    if( arg_size != 1 ) {
        debug_print( debug_counter++, 21, "BS3", 0x05 );
        return 1;
    }
  
    color = (byte)*current;
    current++;
  
    debug_print( parse_counter++, 24, "LIN", 0x05 );
    line( x1, y1, x2, y2, color );
    return 0;
}


int try_circle( char *current )
{
    int x;
    int y;
    int radius;
    int size = 2;
    int arg_size;
    byte color;
  
    // get x
    arg_size = get_int( current );
    current += 2;
    size += 2;

    if( arg_size != 2 ) {
        debug_print( debug_counter++, 21, "BS1", 0x05 );
        return 1;
    }
  
    x = get_int( current );
    current += 2;
    size += 2;
  
    // get y
    arg_size = get_int( current );
    current += 2;
    size += 2;
  
    if( arg_size != 2 ) {
        debug_print( debug_counter++, 21, "BS2", 0x05 );
        return 1;
    }
  
    y = get_int( current );
    current += 2;
    size += 2;
  
    // get radius
    arg_size = get_int( current );
    current += 2;
    size += 2;
  
    if( arg_size != 2 ) {
        debug_print( debug_counter++, 21, "BS2", 0x05 );
        return 1;
    }
  
    radius = get_int( current );
    current += 2;
    size += 2;
  
    // get the color
    arg_size = get_int( current );
    current += 2;
  
    if( arg_size != 1 ) {
        debug_print( debug_counter++, 21, "BS3", 0x05 );
        return 1;
    }
  
    color = (byte)*current;
    current++;
  
    debug_print( parse_counter++, 24, "LIN", 0x05 );
    circle( x, y, radius, color );
    return 0;
}


byte try_get_pixel( char *current )
{
    int x;
    int y;
    int size = 2;
    int arg_size;
  
    // get x
    arg_size = get_int( current );
    current += 2;
    size += 2;

    if( arg_size != 2 ) {
        debug_print( debug_counter++, 21, "BS1", 0x05 );
        return 0x00;
    }
  
    x = get_int( current );
    current += 2;
    size += 2;
  
    // get y
    arg_size = get_int( current );
    current += 2;
    size += 2;
  
    if( arg_size != 2 ) {
        debug_print( debug_counter++, 21, "BS2", 0x05 );
        return 0x00;
    }
  
    y = get_int( current );
    current += 2;
    size += 2;
  
    debug_print( parse_counter++, 24, "GPX", 0x05 );
    return get_pixel( x, y );
}


int try_put_image( char *current )
{
    int x;
    int y;
    int size = 2;
    int arg_size;
  
    // get x
    arg_size = get_int( current );
    current += 2;
    size += 2;

    if( arg_size != 2 ) {
        debug_print( debug_counter++, 21, "BS1", 0x05 );
        return 1;
    }
  
    x = get_int( current );
    current += 2;
    size += 2;
  
    // get y
    arg_size = get_int( current );
    current += 2;
    size += 2;
  
    if( arg_size != 2 ) {
        debug_print( debug_counter++, 21, "BS2", 0x05 );
        return 1;
    }
  
    y = get_int( current );
    current += 2;
  
    // get the message
    arg_size = get_int( current );
    current += 2;
  
    debug_print( parse_counter++, 24, "PRT", 0x05 );
    put_image( x, y, current );
    return 0;
}


int try_put_trans_image( char *current )
{
    int x;
    int y;
    int size = 2;
    int arg_size;
    byte color;
  
    // get x
    arg_size = get_int( current );
    current += 2;
    size += 2;

    if( arg_size != 2 ) {
        debug_print( debug_counter++, 21, "BS1", 0x05 );
        return 1;
    }
  
    x = get_int( current );
    current += 2;
    size += 2;
  
    // get y
    arg_size = get_int( current );
    current += 2;
    size += 2;
  
    if( arg_size != 2 ) {
        debug_print( debug_counter++, 21, "BS2", 0x05 );
        return 1;
    }
  
    y = get_int( current );
    current += 2;
  
    // get the color
    arg_size = get_int( current );
    current += 2;
  
    if( arg_size != 1 ) {
        debug_print( debug_counter++, 21, "BS3", 0x05 );
        return 1;
    }
  
    color = (byte)*current;
    current++;  
  
    // get the message
    arg_size = get_int( current );
    current += 2;
  
    debug_print( parse_counter++, 24, "PRT", 0x05 );
    put_trans_image( x, y, color, current );
    return 0;
}

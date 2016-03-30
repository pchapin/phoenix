/****************************************************************************
FILE          : gamepad.c
LAST REVISION : 2008-04-18
SUBJECT       : Phoenix gamepad handling.
PROGRAMMER    : (C) Copyright 2008  The Phoenix Team.

This program is specifically made to be used with a Gravis gamepad.

Please send comments or bug reports to

    Phoenix Team
    c/o Peter C. Chapin
    Vermont Technical College
    Randolph Center, VT 05061
    Peter.Chapin@vtc.vsc.edu
****************************************************************************/

#include <Phoenix.h>
#include "gamepad.h"

static word data    = 0x378;
static word status  = 0x379;
static word control = 0x37A;
static word register4;
static word register5;
static word register6;
static word register7;
static word register8;

void initialize_gamepad( )
{
    word LPT1_base_address;
    byte far *buffer = MK_FP( 0x0000, 0x0000 );
  
    LPT1_base_address = ( buffer[0x0409] << 8) + buffer[0x0408];
    data      = LPT1_base_address;
    status    = LPT1_base_address + 1;
    control   = LPT1_base_address + 2;
    register4 = LPT1_base_address + 3;
    register5 = LPT1_base_address + 4;
    register6 = LPT1_base_address + 5;
    register7 = LPT1_base_address + 6;
    register8 = LPT1_base_address + 7;
    
    // Turn on IRQ.
    outp( control, inp( control ) | 0x10 );
    // outp( data, 0x00 );
    // outp( data, 0x00 );
}


byte get_data_register( )
{
    return inp( data ); // Usually 0x378.
}


byte get_status_register( )
{
    return inp( status ); // Usually 0x379.
}


byte get_control_register( )
{
    return inp( control ); // Usually 0x37A.
}


byte get_register4( )
{
    return inp( register4 );
}


byte get_register5( )
{
    return inp( register5 );
}


byte get_register6( )
{
    return inp( register6 );
}


byte get_register7( )
{
    return inp( register7 );
}


byte get_register8( )
{
    return inp( register8 );
}


/****************************************************************************
FILE          : beep.c
LAST REVISION : 2008-04-18
SUBJECT       : Phoenix BEEP
PROGRAMMER    : (C) Copyright 2008  The Phoenix Team.

This program uses to internal speaker to produce beeps of different 
frequencies.

Please send comments or bug reports to

    Phoenix Team
    c/o Peter C. Chapin
    Vermont Technical College
    Randolph Center, VT 05061
    Peter.Chapin@vtc.vsc.edu
****************************************************************************/

#include <Phoenix.h>
#include "beep.h"

void beep( word frequency )
{
    byte temp_61;
    unsigned int i, j;

    // Initialize speaker.
    outp( 0x43, 182 );
    
    // Set frequency.
    outp( 0x42, (byte)frequency );
    outp( 0x42, (byte)(frequency >> 8) );
    
    // Turn it on.
    temp_61 = inp( 0x61 );
    outp( 0x61, temp_61 | 0x03 );
    
    // Delay.
    for( i = 0; i < 50000; i++ ) {
        for( j = 0; j < 1000; j++ )  {}
    }
    
    // Turn it off.
    temp_61 = inp( 0x61 );
    outp( 0x61, temp_61 & 0xFC );
}

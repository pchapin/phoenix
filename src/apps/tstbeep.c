/****************************************************************************
FILE      : tstbeep.c
SUBJECT   : Test the beep function
PROGRAMMER: (C) Copyright 2008  The Phoenix Team.

This is program to test the audio system

Please send comments or bug reports to

    Phoenix Team
    c/o Peter C. Chapin
    Vermont Technical College
    Williston, VT 05495
    PChapin@vtc.vsc.edu
****************************************************************************/

#include "beep.h"

int main( void )
{ 
    while( 1 ) {
        beep(  C5 );
        beep( A4S );
        beep(  F4 );
        beep(  C5 );
        beep( A4S );
        beep(  F4 );
        beep(  C5 );
        beep( A4S );
        beep(  F4 );
        beep(  C5 );
        beep( A4S );
        beep(  F4 );
        beep(  C5 );
        beep(  C5 );
        beep(  C5 );
    }
    return 0;
}


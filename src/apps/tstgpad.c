/****************************************************************************
FILE      : tstgpad.c
SUBJECT   : Test the gamepad functions function
PROGRAMMER: (C) Copyright 2008  The Phoenix Team.

This is program to test the gamepad system

Please send comments or bug reports to

    Phoenix Team
    c/o Peter C. Chapin
    Vermont Technical College
    Williston, VT 05495
    PChapin@vtc.vsc.edu
****************************************************************************/

#include <Phoenix.h>
#include "gamepad.h"
#include "video.h"

//return hex value
unsigned char *get_hex_string( byte number )
{
    unsigned char hex[16] = "0123456789ABCDEF";
    unsigned char my_output[3];
  
    my_output[0] = hex[(number >> 4) & 0x0F];
    my_output[1] = hex[number & 0x0F];
    my_output[2] = '\0';
  
    return my_output;
}


int main( void )
{
    unsigned char *my_output;
  
    initialize_gamepad( );
    while( 1 ) {
        my_output = get_hex_string( get_data_register( ) );
        print_at( 1, 0, my_output, 0x04 );
        my_output = get_hex_string( get_status_register( ) );
        print_at( 2, 0, my_output, 0x04 );
        my_output = get_hex_string( get_control_register( ) );
        print_at( 3, 0, my_output, 0x04 );
        my_output = get_hex_string( get_register4( ) );
        print_at( 4, 0, my_output, 0x04 );
        my_output = get_hex_string( get_register5( ) );
        print_at( 5, 0, my_output, 0x04 );
        my_output = get_hex_string( get_register6( ) );
        print_at( 6, 0, my_output, 0x04 );
        my_output = get_hex_string( get_register7( ) );
        print_at( 7, 0, my_output, 0x04 );
        my_output = get_hex_string( get_register8( ) );
        print_at( 8, 0, my_output, 0x04 );
    }
    return 0;
}

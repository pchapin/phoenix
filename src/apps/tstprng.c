/****************************************************************************
FILE      : tstprng.c
SUBJECT   : Tests the prng.
PROGRAMMER: (C) Copyright 2008  The Phoenix Team.

This basic test will test the prng.

Please send comments or bug reports to

    Phoenix Team
    c/o Peter C. Chapin
    Vermont Technical College
    Williston, VT 05495
    PChapin@vtc.vsc.edu
****************************************************************************/

#include "xtimer.h"
#include "stdio.h"

int main( void )
{
    double bits[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int i;
    int j;
    int random = 0;
  
    for( i = 0; i < 30000; i++ ) {
        random = get_random( );

        for( j = 0; i < 16; ++j ) {
            if( ( ( random >> j ) & 0x01 ) == 1 ) {
                bits[j]++;
            }
        }
    }
  
    for( i = 0; i < 16; i++ ) {
        printf( "%f\n", bits[i] / 30000 );
    } 
    return 0;
}


/****************************************************************************
FILE      : copyboot.c
SUBJECT   : Copies a raw boot sector into a disk image.
PROGRAMMER: (C) Copyright 2012 The Phoenix Team.

    Phoenix Team
    c/o Peter C. Chapin
    Vermont Technical College
    Randolph Center, VT 05061
    PChapin@vtc.vsc.edu
****************************************************************************/

#include <stdio.h>
#include <stdlib.h>

int main( int argc, char *argv[] ) 
{
    FILE *input_fp, *output_fp;
    int data;
	
    if( argc != 3 ) {
        printf("Incorrect number of arguments!\nMust use: %s input_file output_file.\n", argv[0] );
        return 1;
    }
    
    /* Open the input file */
    if( ( input_fp = fopen( argv[1], "rb" ) ) == NULL ) {
        printf( "Error opening file %s for reading.\n", argv[1] );
        exit( 1 );
    }
  
    /* Open the output file */
    if( ( output_fp = fopen( argv[2], "rb+" ) ) == NULL ) {
        printf( "Error opening file %s for writing.\n", argv[2] );
        exit( 1 );
    }
  
    /* Copy the entire input file on top of the first part of the output file. */
    while( ( data = getc( input_fp ) ) != EOF ) {
        putc( data, output_fp );
    }
  
    fclose( input_fp );
    fclose( output_fp );
    return 0;
}


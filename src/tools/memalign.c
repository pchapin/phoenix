/****************************************************************************
FILE      : memalign.c
SUBJECT   : Memory alignment
PROGRAMMER: (C) Copyright 2008 The Phoenix Team.

Replaces the first 256 bytes of a COM file with NOP opcodes. The resulting file can be loaded
into memory and executed from the begining (which is what boot.asm attempts to do).

Please send comments or bug reports to

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
    int i, data;
	
    if( argc != 3 ) {
        printf("Incorrect number of arguments!\n"
               "Must use: %s input_file output_file.\n", argv[0] );
        return 1;
    }
    
    /* Open the input file */
    if( ( input_fp = fopen( argv[1], "rb" ) ) == NULL ) {
        printf( "Error opening file %s for reading.\n", argv[1] );
        exit( 1 );
    }
  
    /* Open the output file */
    if( ( output_fp = fopen( argv[2], "wb" ) ) == NULL ) {
        printf( "Error opening file %s for writing.\n", argv[2] );
        exit( 1 );
    }
  
    /* Fill up the first 256 bytes of the output file. */
    for( i = 0; i < 256; i++ ) {
        putc( 0x90, output_fp );  /* 0x90 is the opcode of the NOP instruction. */
    }
  
    /* Append the input file starting after the first 256 bytes of the input file. */
    fseek( input_fp, 256, SEEK_SET );
    while( ( data = getc( input_fp ) ) != EOF ) {
        putc( data, output_fp );
    }
  
    fclose( input_fp );
    fclose( output_fp );
    return 0;
}


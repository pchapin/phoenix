/****************************************************************************
FILE          : bmp2hex.c
LAST REVISION : 2008-01-24
SUBJECT       : Bitmap conversion.
PROGRAMMER    : (C) Copyright 2008  The Phoenix Team.

Converts .bmp files to an array of hex values.

Please send comments or bug reports to

    Phoenix Team
    c/o Peter C. Chapin
    Vermont Technical College
    Randolph Center, VT 05061
    Peter.Chapin@vtc.vsc.edu
****************************************************************************/
     
#include <stdio.h>
#include <stdlib.h>

typedef unsigned char  byte;
typedef unsigned short word;
typedef unsigned long  dword;

typedef struct tagBITMAP {             /* the structure for a bitmap. */
    word width;
    word height;
    byte palette[256*3];
    byte *data;
} BITMAP;


// Skips bytes in a file.
// Written by David Brackeen (http://www.brackeen.com/home/vga/) 
void fskip( FILE *fp, int num_bytes )
{
    int i;
    for( i = 0; i < num_bytes; i++ )
        fgetc( fp );
}


// Loads a bitmap file into memory.
// Written by David Brackeen (http://www.brackeen.com/home/vga/) 
void load_bmp( char *file, BITMAP *b )
{
    FILE *fp;
    long index;
    word num_colors;
    int x;

    /* open the file */
    if( ( fp = fopen( file,"rb" ) ) == NULL ) {
        printf( "Error opening file %s.\n",file );
        exit( 1 );
    }

    /* check to see if it is a valid bitmap file */
    if( fgetc( fp )!='B' || fgetc( fp )!='M' ) {
        fclose( fp );
        printf( "%s is not a bitmap file.\n",file );
        exit( 1 );
    }

    /* read in the width and height of the image, and the
       number of colors used; ignore the rest */
    fskip( fp, 16 );
    fread( &b->width, sizeof(word), 1, fp );
    fskip( fp, 2 );
    fread( &b->height,sizeof(word), 1, fp );
    fskip( fp, 22 );
    fread( &num_colors,sizeof(word), 1, fp );
    fskip( fp, 6 );

    /* assume we are working with an 8-bit file */
    if( num_colors == 0 ) num_colors = 256;

    /* try to allocate memory */
    if( ( b->data = (byte *) malloc( (word)(b->width * b->height) ) ) == NULL ) {
        fclose( fp );
        printf( "Error allocating memory for file %s.\n", file );
        exit( 1 );
    }

    /* read the palette information */
    for( index = 0; index < num_colors; index++ ) {
        b->palette[(int)(index*3 + 2)] = fgetc( fp ) >> 2;
        b->palette[(int)(index*3 + 1)] = fgetc( fp ) >> 2;
        b->palette[(int)(index*3 + 0)] = fgetc( fp ) >> 2;
        x = fgetc( fp );
    }

    /* read the bitmap */
    for( index = ( b->height - 1 ) * b->width; index >= 0; index -= b->width ) {
        for( x = 0; x < b->width; x++ ) {
            b->data[(word)index + x] = (byte)fgetc( fp );
        }
    }

    fclose( fp );
}


// Create a file that can be viewed with a normal text editor.
void create_file( char *file, BITMAP *b )
{
    FILE *fp;
    int i, j;

    /* open the file */
    if( (fp = fopen( file,"w" )) == NULL ) {
        printf( "Error opening file %s.\n", file );
        exit( 1 );
    }
  
    fprintf( fp, "Width: %d\n", b->width );
    fprintf( fp, "Height: %d\n", b->height );
  
    fprintf( fp, "char image[]={\n" );
    for( j = 0; j < b->height; j++ ) {
        for( i = 0; i < b->width; i++ ) {
            fprintf( fp, "0x%02X", b->data[( j * b->width ) + i] );
            if( !(i == b->width - 1 && j == b->height - 1) ) {
                fprintf( fp, ", " );
            }
        }
        fprintf( fp, "\n" );
    }
    fprintf( fp, "};\n" );
  
    fprintf( fp, "char palette[]={\n" );
    for( j = 0; j < 16; j++ ) {
        for( i = 0; i < 48; i++ ) {
            fprintf( fp, "0x%02X", b->palette[(j * 48) + i] );
            if( !(i == 48 - 1 && j == 16 - 1) ) {
                fprintf( fp, ", " );
            }
        }
        fprintf( fp, "\n" );
    }
    fprintf( fp, "};\n" );
    fclose( fp );
}


int main( int argc, char *argv[] )
{
    BITMAP bmp;
	
    if( argc != 3 ) {
        printf( "Incorrect number of arguments!\n"
                "Must use: %s [bitmap_file] [output_file].\n", argv[0] );
        return 1;
    }
    
    load_bmp( argv[1], &bmp );
    create_file( argv[2], &bmp );
    return 0;
}


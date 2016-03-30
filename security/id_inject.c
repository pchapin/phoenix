/****************************************************************************
FILE          : id_inject.c
LAST REVISION : 2008-04-16
SUBJECT       : Injects id's in files and the boot sector.
                Also encrypts files.
PROGRAMMER    : Curtis Aube, Nick Guertin, Wei Yao Lin

(C) Copyright 2008  The Phoenix Team.

This is used to create copy protection on phoenix executables.

Please send comments or bug reports to

     cjaube@yahoo.com
****************************************************************************/

#include <i86.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

static unsigned long deadbeef_seed = 0; 
static unsigned long deadbeef_beef = 0xdeadbeef;

// Read the boot sector and place the information at segment:offset.
void read_boot_sector( int segment, unsigned short offset )
{
    union REGS r;
    struct SREGS s;
    int tries = 0;
  
    do {
        r.h.ah = 0x02;   // Read sector function.
        r.h.al = 0x01;   // Number of sectors.
        r.h.ch = 0x00;   // cylinder.
        r.h.cl = 0x01;   // sector (0x01: logical sector 0).
        r.h.dh = 0x00;   // head.
        r.h.dl = 0x00;   // drive (0x00 - 0x7F in floppy).
        s.es = segment;  // FP_SEG((char far *)buffer); //segment.
        r.x.bx = offset; // (unsigned short)buffer;     //offset of buffer.
        int86x( 0x13, &r, &r, &s );
        tries++;
    } while( r.h.ah != 0x00 && tries < 3 );
  
    if( r.h.ah != 0x00 ) {
        fprintf( stderr, "Floppy read was unsuccessful after %d tries: %d\n", tries, r.h.ah );
        exit( 1 );
    } 
}

// Write the bootsector back using the information from segment:offset.
void write_boot_sector( int segment, unsigned short offset )
{
    union REGS r;
    struct SREGS s;
    int tries = 0;
  
    do {
        r.h.ah = 0x03;   // Write sector function.
        r.h.al = 0x01;   // Number of sectors.
        r.h.ch = 0x00;   // cylinder.
        r.h.cl = 0x01;   // sector (0x01: logical sector 0).
        r.h.dh = 0x00;   // head.
        r.h.dl = 0x00;   // drive (0x00 - 0x7F in floppy).
        s.es = segment;  // FP_SEG((char far *)buffer); // segment.
        r.x.bx = offset; // (unsigned short)buffer;     // offset of buffer.
        int86x( 0x13, &r, &r, &s );
        tries++;
    } while( r.h.ah != 0x00 && tries < 3 );
  
    if( r.h.ah != 0x00 ) {
        fprintf( stderr, "Floppy write was unsuccessful after %d tries: %d\n", tries, r.h.ah );
        exit( 1 );
    } 
}

// Inject the id in the boot sector.
void inject_boot( unsigned int rnd1, unsigned int rnd2 )
{
    char buffer[512];
  
    read_boot_sector( FP_SEG( (char far *)buffer ), (unsigned short)buffer );
    buffer[508] = (char)(rnd1 >> 8);
    buffer[509] = (char)(rnd1     );
    buffer[510] = (char)(rnd2 >> 8);
    buffer[511] = (char)(rnd2     );
    write_boot_sector( FP_SEG( (char far *)buffer ), (unsigned short)buffer );
}

// Inject the id in the exe file.
void inject_exe( char *filename, unsigned int rnd1, unsigned int rnd2 )
{
    FILE *fp;
  
    if( ( fp = fopen(filename, "ab") ) == NULL ) {
        fprintf( stderr, "An error occured while opening the file: %s.\n", filename );
        exit( 1 );
    }
    fputc( (char)(rnd1 >> 8), fp );
    fputc( (char)(rnd1     ), fp );
    fputc( (char)(rnd2 >> 8), fp );
    fputc( (char)(rnd2     ), fp );
    fclose( fp );
}

// Inject the id in the exe file.
void encrypt_exe( char *filename, unsigned int rnd1, unsigned int rnd2 )
{
    FILE *fp;
    unsigned int i = 0;
    unsigned int length = 0;
    // unsigned int temp;
    char file;
  
    if( ( fp = fopen(filename, "rb+") ) == NULL ) {
        fprintf( stderr, "An error occured while opening the file: %s.\n", filename );
        exit( 1 );
    }
  
    while( fread( &file, sizeof(char), 1, fp ) ) {
        // file = (char)temp;
        i++;
    
        file ^= (char)rnd1>>8;
        file ^= (char)rnd1;
        file ^= (char)rnd2>>8;
        file ^= (char)rnd2;
    
        // fputc( file, fp );
    
        fwrite( &file, sizeof(char), 1, fp );
        length++;
    }
    fclose( fp );
    printf( "Size of file: %d\n", length );
}

// Return a random number.
unsigned int get_random( )
{

    if( deadbeef_seed == 0 ) {
        deadbeef_seed  = random_seed( );
        deadbeef_seed *= random_seed( );
        deadbeef_seed *= random_seed( );
    }

    deadbeef_seed = (deadbeef_seed << 7) ^ ( (deadbeef_seed >> 25) + deadbeef_beef ); 
    deadbeef_beef = (deadbeef_beef << 7) ^ ( (deadbeef_beef >> 25) + 0xdeadbeef    ); 
  
    return (unsigned int)deadbeef_seed; 
}

// Get a seed for the prng from the internal clock.
int random_seed( )
{
    return (int)inp( 0x40 );
}

int main( int argc, char *argv[] )
{
    unsigned int bits[2] = {0, 0};
    int i = 0;
    int end = 0;
    int random = 0;
  
    // Parse args, read in a file list.
    if( argc < 2 ) {
        fprintf( stderr, "Not enough arguments! Needs a list of files.\n" );
        exit( 1 );
    }
  
    // Create a random 4-byte number.
    get_random( );
    end = random_seed( );
  
    for( i = 0; i < end; i++ ) {
        random = get_random( );
    }
  
    bits[0] = random;
  
    end = random_seed( );
  
    for( i = 0; i < end; i++ ) {
        random = get_random( );
    }
    bits[1] = random;
  
    printf( "Key: %04X%04X\n", bits[0], bits[1] );
  
    // Call inject_boot.
    inject_boot( bits[0], bits[1] );
  
    // Call inject_exe on each file.
    for( i = 1; i < argc; i++ ) {
        inject_exe( argv[i], bits[0], bits[1] );
    }
  
    // Encrypt each file.
    for( i = 1; i < argc; i++ ) { 
        encrypt_exe( argv[i], bits[0], bits[1] );
    }
  
    return 0;
}

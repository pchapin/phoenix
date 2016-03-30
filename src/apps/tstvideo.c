/****************************************************************************
FILE      : tstvideo.c
SUBJECT   : Extremely basic test for Phoenix.
PROGRAMMER: (C) Copyright 2008  The Phoenix Team.

This basic test will test message passing functionality by remotely using the video module.

Please send comments or bug reports to

    Phoenix Team
    c/o Peter C. Chapin
    Vermont Technical College
    Williston, VT 05495
    PChapin@vtc.vsc.edu
****************************************************************************/

//#include "message.h"
#include "vga.h"
#include "video.h"

void video_test( )
{
    int vertices[12]={30,20, 35,30, 62,18, 300,55, 100,60, 40,150};
  
    set_mode( VGA_MODE );
    put_pixel( 100, 101, 4 );
    line( 40, 40, 100, 60, 6 );
    rect( 140, 50, 230, 80, 4 );
    rect_fill( 30, 160, 50, 178, 2 );
    circle( 100, 100, 30, 4 );
    polygon( 6, vertices, 15 );
    fill( 80, 80, 35 );
    vga_print_at( 10, 10, "Hello World", 32 );
    put_trans_image( 20, 20, 0, "rocket" );
}


int main( void )
{
    //process video;  
  
    //video.pid = 0x01;
    //xthread_create( video, do_stuff );
  
    video_test( );
  
    while( 1 ) { }
    return 0;
}


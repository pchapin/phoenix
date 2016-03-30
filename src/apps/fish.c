/*!
 * \file  fish.c
 * \brief Demonstrates how to display an image.
 *
 */

#include <stddef.h>
#include <string.h>

#include <Phoenix.h>
#include "video.h"    // Needed for run_video( )
#include "xvideo.h"

#define TRACE_COLUMN 60
#define COLOR 0x04

int trace_counter = 0;

void *fish( void );

int main( void )
{
    processID vidid;
    processID fishid;

    vidid.pid = VIDEO_PID;
    fishid.pid = MAIN_PID;
  
    xthread_create( vidid, run_video, NORMAL_PRIORITY );
    xthread_create( fishid, fish, NORMAL_PRIORITY );  
    start_phoenix( );
    return 0;
}


void *fish( void )
{
    set_mode( VGA_MODE );  
    xvideo_put_image(  10, 10, "rocket" );
    xvideo_put_image( 100, 10, "rocket" );
    return NULL;
}

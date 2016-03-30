/*!
 * \file  phxlives.c
 * \brief Basic Phoenix demonstration program and template.
 *
 * This simple program demonstrates basic Phoenix functionality and serves as a template for
 * creating Phoenix applications. To build your own application start by copying this template
 * and editing the main thread function appropriately.
 */

#include <stddef.h>   // For NULL

#include <Phoenix.h>  // For kernel API (thread management, etc).
#include "video.h"    // For run_video( ), the video module's start-up thread.
#include "xvideo.h"   // Application interface to the video module.

void *main_thread( void );

int main( void )
{
    processID video_id;
    processID main_id;

    // All well-known threads have low PID values given by symbolic names. A module should
    // always be started on its well-known PID or else library functions that try to communicate
    // with that module won't work. The main thread should be started on MAIN_PID. Additional
    // application threads should be given PIDs that count down from MAIN_PID. A maximum of
    // MAX_THREADS threads is supported, including the special idle thread created by Phoenix
    // (see Phoenix.h).
    //
    // Be aware that modules may start additional threads on their own but this is not
    // recommended practice. Consult the documentation for each module for more information.
    // 
    video_id.pid = VIDEO_PID;
    main_id.pid = MAIN_PID;

    // Programs start in "load context" which is a special context that only applies to code
    // executing from the boot loader. In load context only a small fraction of Phoenix system
    // calls will work. Threads created here do not start until start_phoenix( ) is called.
    // Recommended practice is to start threads for needed modules and a single main thread for
    // the application. Further application initialization, including creating more threads, can
    // be done inside the main thread.
    //
    xthread_create( video_id, run_video, NORMAL_PRIORITY );
    xthread_create( main_id, main_thread, NORMAL_PRIORITY );

    // The start_phoenix( ) function makes Phoenix operational. After start_phoenix( ) only
    // Phoenix threads will execute. The thread running in load context ends. The start_phoenix
    // function never returns.
    //
    start_phoenix( );
    return 0;
}


void *main_thread( void )
{
    int  i;
    int  j;
    int  row = 0;
    byte color = 0x01;
  
    // Communication with modules is always done through the xmodule_* interfaces. Do not
    // attempt to use module internal functions directly. The xmodule_* functions send messages
    // to the modules in a coordinated way and allow the modules to remain synchronized.
    //
    xvideo_clear_screen( );
    while( 1 ) {
        // TODO: There should be a way to ask the video module how large the screen is.
        if( row > 24 ) {
            row = 0;
        }
  
        xvideo_print_at( row, 1, "Phoenix Lives!!", color );
        row++;
        color++;
    
        // TODO: The kernel should provide a sleep or delay facility.
        for( i = 0; i < 1000; i++ ) {
            for( j = 0; j < 2000; j++ ) { }
        }
    }
    // QUESTION: What happens if a thread function returns?
    return NULL;
}

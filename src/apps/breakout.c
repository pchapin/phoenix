/****************************************************************************
FILE      : breakout.c
SUBJECT   : A breakout clone.
PROGRAMMER: (C) Copyright 2008  The Phoenix Team.

This is a clone of the Atari game called breakout.

This program is not a proper Phoenix application since it does everything in load context and
makes forbidden calls to module private functions.

TODO: Convert into a proper Phoenix application.

Please send comments or bug reports to

    Phoenix Team
    c/o Peter C. Chapin
    Vermont Technical College
    Williston, VT 05495
    PChapin@vtc.vsc.edu
****************************************************************************/

#include <Phoenix.h>
#include "beep.h"
#include "keyboard.h"   // Needed for run_keyboard( )
#include "xkeyboard.h"
#include "vga.h"
#include "video.h"      // Needed for run_video( )
#include "xvideo.h"

#define RIGHT 0
#define LEFT 1
#define NONE 2

void *main_thread( void );

int main( void )
{
    processID vidid;
    processID keyid;
    processID mainid;

    vidid.pid  = VIDEO_PID;
    keyid.pid  = KEYBOARD_PID;
    mainid.pid = MAIN_PID;
	
    xthread_create( vidid, run_video, NORMAL_PRIORITY );
    xthread_create( keyid, run_keyboard, NORMAL_PRIORITY );
    xthread_create( mainid, main_thread, NORMAL_PRIORITY );
    start_phoenix( );
    return 0;
}


//This is a startup intro for the console
void intro( )
{
    //The palette will change when pu_image is called, so the color values have to be based on that palette
    xvideo_rect_fill( 0, 0, SCREEN_WIDTH-1, SCREEN_HEIGHT-1, 0xCD );
    xvideo_put_image( 120, 60, "phoenix" );
    xvideo_circle( 160, 95, 60, 0xAC );
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
    while( xkeyboard_getKey( ) == 0 ) { }
    xvideo_clear_screen( );
}


void *main_thread( void )
{
    char myLastKey;
    char direction = NONE;
    unsigned int i, j, color = 32, timerCount = 0;
    int paddleX      = 150;
    int pixelX       = 319;
    int pixelY       = 100;
    int pixelSlopeX  = -1;
    int pixelSlopeY  = 1;
    int paddleWidth  = 20;
    int paddleHeight = 5;
    int curLocColor  = 0;
    int blockCount   = 12 * 16;
  
    set_mode( VGA_MODE );
  
    //Initialization
    intro( );
    xvideo_rect_fill(
        paddleX, SCREEN_HEIGHT - 1 - paddleHeight, paddleX + paddleWidth, SCREEN_HEIGHT - 1, 4 );
    xvideo_put_pixel( pixelX, pixelY, 0x0F );
    
    //Create blocks
    for( i = 0; i < 12; i++ ) {
        for( j = 0; j < 16; j++ ) {
            xvideo_rect_fill( j * 20, i * 6, j*20 + 19, i*6 + 5, color );
        }
        color++;
    }
  
    //Loop forever
    while( 1 ) {
        //Get key press
        myLastKey = xkeyboard_getKey( );
        if ( myLastKey == 75 ) {
            direction = LEFT;
        }
        else if( myLastKey == 77 ) {
            direction = RIGHT;
        }
        else if( myLastKey == 80 ) {
            direction = NONE;
        }
    
        //Move paddle
        if( direction == LEFT && paddleX > 0 ) {
            paddleX--;
        }
        else if( direction == RIGHT && paddleX + paddleWidth < SCREEN_WIDTH - 1 ) {
            paddleX++;
        }
    
        //Show paddle
        if( direction == LEFT ) {
            xvideo_line(
                paddleX, SCREEN_HEIGHT - 1 - paddleHeight, paddleX, SCREEN_HEIGHT - 1, 4 );
            if( paddleX + paddleWidth + 1 <= SCREEN_WIDTH - 1 ) {
                xvideo_line(
                    paddleX + paddleWidth + 1,
                    SCREEN_HEIGHT - 1 - paddleHeight,
                    paddleX + paddleWidth + 1,
                    SCREEN_HEIGHT - 1,
                    0 );
            }
        }
        if( direction == RIGHT ) {
            xvideo_line(
                paddleX + paddleWidth,
                SCREEN_HEIGHT - 1 - paddleHeight,
                paddleX + paddleWidth,
                SCREEN_HEIGHT - 1,
                4 );
            if( paddleX - 1 >= 0 ) {
                xvideo_line(
                    paddleX - 1,
                    SCREEN_HEIGHT - 1 - paddleHeight,
                    paddleX - 1,
                    SCREEN_HEIGHT - 1,
                    0 );
            }
        }
    
        // Only move the ball every other count.
        if( timerCount % 2 == 0 ) {
            // Erase ball.
            xvideo_put_pixel( pixelX, pixelY, 0x00 );
            // Calculate position.
            if( (curLocColor = xvideo_get_pixel( pixelX + pixelSlopeX, pixelY + pixelSlopeY )) != 0x00 ) {
                //We hit a color
                if( curLocColor == 0x04 ) {
                    pixelSlopeY = -pixelSlopeY;
                }
                else {
                    //Remove block
                    xvideo_rect_fill(
                        ( pixelX + pixelSlopeX )/20 * 20,
                        ( pixelY + pixelSlopeY )/ 6 *  6, 
                        ( pixelX + pixelSlopeX )/20 * 20 + 19,
                        ( pixelY + pixelSlopeY )/ 6 *  6 +  5,
                        0x00 );
                    blockCount--;
                    if( blockCount == 0 ) {
                        break;
                    }
                    //Check corners
                    if( ( pixelX + pixelSlopeX ) % 20 == 0 &&
                        ( pixelY + pixelSlopeY ) %  6 == 0 || 
                        ( pixelX + pixelSlopeX ) % 20 == 0 &&
                        ( pixelY + pixelSlopeY ) %  6 +  5 == 0 ||
                        ( pixelX + pixelSlopeX ) % 20 + 19 == 0 &&
                        ( pixelY + pixelSlopeY ) %  6 == 0 ||
                        ( pixelX + pixelSlopeX ) % 20 + 19 == 0 &&
                        ( pixelY + pixelSlopeY ) %  6 +  5 == 0 ) {

                        pixelSlopeX = -pixelSlopeX;
                        pixelSlopeY = -pixelSlopeY;
                    }
                    else if( ( pixelY + pixelSlopeY ) % 6 != 0 &&
                             ( pixelY + pixelSlopeY ) % 6 != 5 ) {
                        pixelSlopeX = -pixelSlopeX;
                    }
                    else {
                        pixelSlopeY = -pixelSlopeY;
                    }
                }
            }
            if( pixelY + pixelSlopeY < 0 ) {
                pixelSlopeY = -pixelSlopeY;
            }
            else if( pixelY + pixelSlopeY > SCREEN_HEIGHT - 1 ) {
                break;
            }
            if( pixelX + pixelSlopeX > SCREEN_WIDTH - 1 || pixelX + pixelSlopeX < 0 ) {
                pixelSlopeX = -pixelSlopeX;
            }
            pixelY += pixelSlopeY;
            pixelX += pixelSlopeX;
      
            // Show Ball.
            xvideo_put_pixel( pixelX, pixelY, 0x0F );
        }
    
        // Delay.
        for( i = 0; i < 1000; i++ ) {
            for( j = 0; j < 2000; j++ ) { }
        }
    
        timerCount++;
    }
    if( blockCount < 0 ) {
        xvideo_vga_print_at( 11, 16, "You Win!", 0x07 );
    }
    else {
        xvideo_vga_print_at( 11, 15, "Game Over", 0x07 );
    }
    while( 1 ) { }
    return 0;
}


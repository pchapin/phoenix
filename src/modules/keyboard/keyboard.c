/****************************************************************************
FILE          : keyboard.c
LAST REVISION : 2008-03-06
SUBJECT       : keboard interface
PROGRAMMER    : (C) Copyright 2008  The Phoenix Team.

This module initialize the keyboard interface.

Please send comments or bug reports to

    Phoenix Team
    c/o Peter C. Chapin
    Vermont Technical College
    Randolph Center, VT 05061
    Peter.Chapin@vtc.vsc.edu
****************************************************************************/

#include <string.h>

#include <Phoenix.h>
#include "internal.h"

static unsigned char Lastkey = 0x00;
enum E_STATE { REGULER_KEY = 0, CAP_KEY, EXTENDED_KEY, ARROW_KEY };
static enum E_STATE KEY_STATE = REGULER_KEY;
static bool has_read = false;
int count = 0;
static char key = 0;

void ( __interrupt * __far * __far IVTK )( void );

const char key_map[256] = {
    0x0, 0x27, '1', '2', '3', '4', '5', '6',  '7', '8', '9',  '0',  '-', '=', 0x08, 0x09,
    'q',  'w', 'e', 'r', 't', 'y', 'u', 'i',  'o', 'p', '[',  ']', 0x13, 0x0,  'a',  's',
    'd',  'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0x0, '\\',  'z', 'x',  'c',  'v',
    'b',  'n', 'm', ',', '.', '/', 0x0, '*',  0x0, ' ', 0x0,  0x0,  0x0, 0x0,  0x0,  0x0,
    0x0,  0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,  0x0, 0x0, 0x0,  0x0,  0x0, 0x0,  0x0,  0x0,
    0x0,  0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,  0x0, 0x0, 0x0,  0x0,  0x0, 0x0,  0x0,  0x0,
    0x0,  0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,  0x0, 0x0, 0x0,  0x0,  0x0, 0x0,  0x0,  0x0,
    0x0,  0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,  0x0, 0x0, 0x0,  0x0,  0x0, 0x0,  0x0,  0x0, 0x0};

void ( __interrupt __far *Old_keyboardISR )( );

void initialize_keyboardISR( )
{
    IVTK[KeyboardISR] = keyboard_ISR;
    outp( 0x60, 0x75 );
}


void ReadScanCode( )
{
    switch( KEY_STATE ) {
    case REGULER_KEY:
        count = 0;
        if( key == 0x00 && (int)Lastkey > 0x39 ) {
            has_read = false;
        }
        else {
            key = key_map[(int)Lastkey];
            has_read = true;
        }
        break;

    case EXTENDED_KEY:
        if( count <= 2 ) {
            break;
        }
        else {
            key = Lastkey;
            KEY_STATE = REGULER_KEY;
            count = 0;
            has_read = true;
        }
        break;

    case CAP_KEY:
        while( Lastkey == 0xE0 ) { };
        if( Lastkey == 0xAA || Lastkey == 0xB6 ) {
            KEY_STATE = REGULER_KEY;
        }
        else if( key_map[(int)Lastkey] >= 0x61 && key_map[(int)Lastkey] <= 0x7A ) {
            key = key_map[(int)Lastkey] - 0x20;
            count = 0;
            has_read = true;
        }
        else {
            key = 0x00;
            count = 0;
        }
        break;

    case ARROW_KEY:
        if( Lastkey > 0x47  && Lastkey < 0x51 ) {
            count = 0;
            has_read = true;
            key = Lastkey;
        }
        break;
    }
}


void check_scan_code( )
{
    if( KEY_STATE == ARROW_KEY ) {
        if( Lastkey == 0xAA ) {
            return;
        }
        if( Lastkey == 0x2A ) {
            KEY_STATE = REGULER_KEY;
            return;
        }
    }
    if( Lastkey == 0x2A || Lastkey == 0x36 ) { // Check shift is press.
        KEY_STATE = CAP_KEY;
    }
    if( Lastkey == 0xAA ) { // Release of shift press.
        KEY_STATE = REGULER_KEY;
    }
    if( Lastkey == 0xE0 ) { // Check of extended key is press.
        KEY_STATE = EXTENDED_KEY;
    }
    if( Lastkey > 0x47  && Lastkey < 0x51 ) {
        KEY_STATE = ARROW_KEY;
    }
    ReadScanCode( );
}


void __interrupt __far keyboard_ISR( )
{
    _asm {
        CLI
    };

    Lastkey = inp( 0x60 );
    check_scan_code( );
    count++;
    outp( 0x20, 0x20 );
}


char *getNewkeys( )
{
    return "Not implemented";
}


char *getKeyString( )
{
    char c[2];

    c[0] = 77;
    c[1] = '\0';
    return c;
}


char getKey( )
{
    char temp;

    temp = key;
    key = 0x00;
    return temp;
}


int getNewkeys_size( )
{
    return strlen( getNewkeys( ) );
}


bool hasRead( )
{
    return has_read;
}


void finishRead( )
{
    has_read = false;
}


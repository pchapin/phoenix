/****************************************************************************
FILE          : gamepad.h
LAST REVISION : 2008-04-18
SUBJECT       : Phoenix gamepad handling.
PROGRAMMER    : (C) Copyright 2008  The Phoenix Team.

This program is specifically made to be used with a Gravis gamepad.

Please send comments or bug reports to

    Phoenix Team
    c/o Peter C. Chapin
    Vermont Technical College
    Randolph Center, VT 05061
    Peter.Chapin@vtc.vsc.edu
****************************************************************************/

#ifndef GAMEPAD_H
#define GAMEPAD_H

void initialize_gamepad( );

byte get_data_register( );
byte get_status_register( );
byte get_control_register( );
byte get_register4( );
byte get_register5( );
byte get_register6( );
byte get_register7( );
byte get_register8( );

#endif


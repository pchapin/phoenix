/****************************************************************************
FILE          : xstring.h
LAST REVISION : 2008-02-17
SUBJECT       : Phoenix Strings
PROGRAMMER    : (C) Copyright 2008 by The Phoenix Team.

This is a library of string functions.

Please send comments or bug reports to

    Phoenix Team
    c/o Peter C. Chapin
    Vermont Technical College
    Randolph Center, VT 05061
    Peter.Chapin@vtc.vsc.edu
****************************************************************************/
#ifndef XSTRING_H
#define XSTRING_H

int   strlen( const char *string );
int   strcmp( const char *string1, const char *string2 );
char *strcpy( char *destination, const char *source );

#endif


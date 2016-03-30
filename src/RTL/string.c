/****************************************************************************
FILE          : xstring.c
LAST REVISION : 2008-02-17
SUBJECT       : Phoenix Strings
PROGRAMMER    : (C) Copyright 2008 by The Phoenix Contributors

This is a library of string functions.

Please send comments or bug reports to

    Phoenix Team
    c/o Peter C. Chapin
    Vermont Technical College
    Randolph Center, VT 05061
    Peter.Chapin@vtc.vsc.edu
****************************************************************************/

#include <string.h>

/*! \file xstring.c Basic string handling functions.
 *
 * This file contains string handling facilities that reproduce the string functions in the C
 * standard. Since using the standard C run time library inside Phoenix is not supported, these
 * important functions must be duplicated here. Note that the names have been chosen to avoid
 * conflicts with standard names in case both the Phoenix run time library and the C standard
 * run time library ever end up coexisting in the same program.
 */
 

//! Finds the length of a string.
/*!
 * The given string must be null terminated. The null byte is not considered part of the
 * string's length.
 *
 * \param string The string to measure.
 * \return The length of the string.
 */
int strlen( const char *string )
{
    int length = 0;

    while( *string ) {
        length++;
        string++;
    }

    return length;
}


//! Compares two strings.
/*!
 * The two strings must be null terminated. The comparison is done in a case sensitive manner.
 *  
 * \param string1 The first string to compare.
 * \param string2 The second string to compare.
 * \return Zero if the strings are the same, a value greater than zero if the first string is
 * "greater" than the second, and a value less than zero if the second string is "greater" than
 * the first.
 */
int strcmp( const char * string1, const char * string2 )
{
    while( *string1 && *string2 ) {
        if( *string1 > *string2 )  { // This character of string1 is greater
            return 1;
        }
        else if( *string1 < *string2 ) { // This character of string2 is greater
            return -1;
        }
        string1++;
        string2++;
    }
  
    if( *string1 && !*string2 ) { // string1 is longer
        return 1;
    }
    else if( !*string1 && *string2 ) { // string2 is longer
        return -1;
    }
    return 0;
}


//! Copies a null terminated string.
/*! 
 * \param destination Pointer to a place where the source string is copied. Enough memory must
 * be allocated or else the behavior is undefined.
 * \param source Pointer to a null terminated string to copy.
 * \return Pointer to the destination buffer.
 */
char *strcpy( char *destination, const char *source )
{
    char *temp = destination;
    while( *destination++ = *source++ ) ;
    return temp;
}


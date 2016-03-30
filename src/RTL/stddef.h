/****************************************************************************
FILE          : xstddef.h
LAST REVISION : 2008-02-14
SUBJECT       : Standard defines for Phoenix
PROGRAMMER    : (C) Copyright 2008 by The Phoenix Team.

Please send comments or bug reports to

    Phoenix Team
    c/o Peter C. Chapin
    Vermont Technical College
    Williston, VT 05495
    PChapin@vtc.vsc.edu
****************************************************************************/

#ifndef XSTDDEF_H
#define XSTDDEF_H

/*! \file xstddef.h Various standard definitions.
 *
 * This file contains useful macros and other definitions that are needed in many other files.
 * This file is intended to mimic the standard header stddef.h.
 */

#define NULL ((void*)0)                 //!< A pointer that doesn't point at anything.
#define sgn(x) ((x < 0) ? -1 :((x > 0) ? 1 : 0))  //!< A macro to return the sign of a number
#define abs(a) (((a) < 0) ? -(a) : (a)) //!< A macro to return the absolute value of a number.

#endif


/****************************************************************************
FILE          : beep.h
LAST REVISION : 2008-04-18
SUBJECT       : Phoenix BEEP
PROGRAMMER    : (C) Copyright 2008  The Phoenix Team.

This the header file for beep.

Please send comments or bug reports to

    Phoenix Team
    c/o Peter C. Chapin
    Vermont Technical College
    Randolph Center, VT 05061
    Peter.Chapin@vtc.vsc.edu
****************************************************************************/

#ifndef BEEP_H
#define BEEP_H

#include <Phoenix.h>

#define C3  9121
#define C3S 8609
#define D3  8126
#define D3S 7670
#define E3  7239
#define F3  6833
#define F3S 6449
#define G3  6087
#define G3S 5746
#define A3  5423
#define A3S 5119
#define B3  4831
#define C4  4560 // Middle C.
#define C4S 4304
#define D4  4063
#define D4S 3834
#define E4  3619
#define F4  3416
#define F4S 3224
#define G4  3043
#define G4S 2873
#define A4  2711
#define A4S 2559
#define B4  2415
#define C5  2280
#define C5S 2152
#define D5  2031
#define D5S 1917
#define E5  1809
#define F5  1715
#define F5S 1612
#define G5  1521
#define G5S 1436
#define A5  1355
#define A5S 1292
#define B5  1207
#define C6  1140 // High C.

// Produces a beep.
void beep( word frequency );

#endif

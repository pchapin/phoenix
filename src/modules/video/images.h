/****************************************************************************
FILE          : images.h
LAST REVISION : 2008-01-25
SUBJECT       : Images for bmp display.
PROGRAMMER    : (C) Copyright 2008  The Phoenix Team.

This is where the game designer can add bitmaps to their game.  Use the bmp2hex tool in the
tools folder to get the hex array to add to this file.  Use the put_image function to display
the image.

Please send comments or bug reports to

    Phoenix Team
    c/o Peter C. Chapin
    Vermont Technical College
    Randolph Center, VT 05061
    Peter.Chapin@vtc.vsc.edu
****************************************************************************/
#ifndef IMAGES_H
#define IMAGES_H

// Image structure
struct image {
    int width;
    int height;
    char *data;
    char *palette;
};

// Takes an image name and returns the image.
struct image get_image( const char *name );

#endif


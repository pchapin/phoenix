
#ifndef VIDEO_INTERNAL_H
#define VIDEO_INTERNAL_H

#include <Phoenix.h>

typedef enum { OK = 0x00, BAD_ARGS = 0x01, NOT_IMPL = 0x02 } video_error_t;
typedef enum {
    BOUNDARY_FILL,
    CIRCLE, 
    CLEAR_SCREEN,
    FILL,
    GET_PIXEL,
    LINE,
    POLYGON,
    PRINT_AT,
    PUT_IMAGE,
    PUT_PIXEL,
    PUT_TRANS_IMAGE,
    RECT,
    RECT_FILL,
    SET_MODE,
    V_DEFAULT,
    VGA_PRINT_AT
} video_func_t;

// Prints a character string to a specific location with color information (used in mode 0x03);
void print_at( int row, int column, const char *message, byte color );

// Clears the screen.
void clear_screen( );

#define VIDEO_INT 0x10  // The BIOS video interrupt.
#define SET_MODE  0x00  // BIOS func to set the video mode.
#define GET_MODE  0x0F  // BIOS func to get the video mode.

#endif

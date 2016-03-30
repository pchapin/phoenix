/*!
 * \file  video.h
 * \brief Interface to video driver start-up thread.
 *
 * This header declares the video driver main thread entry point. Applications must start this
 * thread if they intend to use the video module.
 */

#ifndef VIDEO_H
#define VIDEO_H

#define VIDEO_PID  1  //!< The "well known" process ID of the video module.

//! The thread that reads and parses messages to the video module.
void *run_video( void );

// ==========
// The function xvideo_set_mode( ) is currently not implemented. Once it is, applications should
// switch to using it and then direct access to set_mode( ) won't be necessary.
// ==========

#include <Phoenix.h>  // For the type 'byte' needed in set_mode's declaration.

#define VGA_MODE 0x13 // Mode identifier.

// Sets the video mode.
void set_mode( byte mode );

//////////////////////////////////////////////////////////////////////

#endif

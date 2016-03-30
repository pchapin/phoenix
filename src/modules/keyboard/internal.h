/*!
 * \file  internal.h
 * \brief Keyboard module internal header.
 *
 * This file contains declarations of resources needed only by the various keyboard module
 * source files. This header should never be included into application level code.
 */

#ifndef KEYBOARD_INTERNAL_H
#define KEYBOARD_INTERNAL_H

#include "keyboard.h"

#define KeyboardISR 0x09

void __interrupt __far keyboard_ISR( );
void  initialize_keyboardISR( void );
char *getKeyString( );
char  getKey( );
char *getNewkeys( );
int   getNewkeys_size( );
bool  hasRead( );
void  finishRead( );

#endif

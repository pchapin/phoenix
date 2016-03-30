/*!
 * \file  keyboard.h
 * \brief Keyboard module header.
 *
 * This file contains declarations of keyboard module resources that can be accessed by
 * applications.
 */

#ifndef KEYBOARD_INTERFACE_H
#define KEYBOARD_INTERFACE_H

typedef enum { K_DEFAULT, GETKEY, GETNEWKEYS, GETKEYSTRING } keyboard_func_t;

#define KEYBOARD_PID  2  //!< Process ID of the keyboard module.

// The thread that reads and parses messages to the keyboard module. See kparser.c.
void *run_keyboard( void );

#endif

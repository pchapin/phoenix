############################################################################
# FILE        : makefile
# LAST REVISED: 2024-06-12
# AUTHOR      : Phoenix Contributors
#
# This is the makefile for the keyboard module.
############################################################################

.c.obj: .AUTODEPEND
    wcc -q -ms -s -w4 -bt=dos -x -fo=$[*.obj -I..\..\RTL -I..\..\kernel $[*.c

OBJ=keyboard.obj kparser.obj xkeyboard.obj

keyboard.lib: $(OBJ)
    wlib -q -n keyboard.lib @<<keyboard.lbc 
+kparser.obj
+keyboard.obj
+xkeyboard.obj
<<

clean: .SYMBOLIC
    rm -f *.obj *.err *.bak keyboard.lib

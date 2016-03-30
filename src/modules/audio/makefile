############################################################################
# FILE        : makefile
# LAST REVISED: 2008-04-18
# AUTHOR      : Phoenix Contributors
#
# This is the makefile for the audio module.
############################################################################

.c.obj: .AUTODEPEND
    wcc -q -ms -s -w4 -bt=dos -x -fo=$[*.obj -I../../RTL -I../../kernel $[*.c

OBJ=beep.obj

audio.lib: $(OBJ)
    wlib -q -n audio.lib @<<audio.lbc 
+beep.obj
<<

clean: .SYMBOLIC
    rm -f *.obj *.err *.bak audio.lib




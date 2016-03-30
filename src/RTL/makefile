############################################################################
# FILE        : makefile
# LAST REVISED: 2008-01-30
# AUTHOR      : Phoenix Contributors
#
# This is the makefile for the run-time library.
############################################################################

.c.obj: .AUTODEPEND
    wcc -q -ms -s -w4 -bt=dos -x -fo=$[*.obj $(DEBUG) -I. -I../modules/video -I../modules/keyboard -I../kernel $[*.c

OBJ=debug.obj string.obj

RTL.lib: $(OBJ)
    wlib -q -n RTL.lib @<<RTL.lbc 
+debug.obj
+string.obj
<<

clean: .SYMBOLIC
    rm -f *.obj *.err *.bak RTL.lib




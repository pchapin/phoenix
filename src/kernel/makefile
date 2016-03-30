############################################################################
# FILE        : makefile
# LAST REVISED: 2008-02-02
# AUTHOR      : Phoenix Contributors
#
# This is the makefile for the kernel.
############################################################################

.c.obj: .AUTODEPEND
    wcc -q -ms -s -w4 -bt=dos -x -fo=$[*.obj $[*.c

.asm.obj:
    wasm -q -fo=$[*.obj $[*.asm

OBJ=Phoenix.obj xbuffer.obj xmessage.obj xrndbuff.obj xthread.obj xtimer.obj xt_asm.obj

kernel.lib: $(OBJ)
    wlib -q -n kernel.lib @<<kernel.lbc 
+Phoenix.obj
+xbuffer.obj
+xmessage.obj
+xrndbuff.obj
+xthread.obj
+xt_asm.obj
+xtimer.obj
<<

clean: .SYMBOLIC
    rm -f *.obj *.err *.bak kernel.lib


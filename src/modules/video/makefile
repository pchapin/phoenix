############################################################################
# FILE        : makefile
# LAST REVISED: 2024-12-11
# AUTHOR      : Phoenix Contributors
#
# This is the makefile for the video module.
############################################################################

.c.obj: .AUTODEPEND
    wcc -q -ms -s -w4 -bt=dos -x -fo=$[*.obj -I../../RTL -I../../kernel $[*.c

OBJ=images.obj vga.obj video.obj vparser.obj xvideo.obj

video.lib: $(OBJ)
    wlib -q -n video.lib @<<video.lbc 
+images.obj
+vga.obj
+video.obj
+vparser.obj
+xvideo.obj
<<

clean: .SYMBOLIC
    rm -f *.obj *.err *.bak video.lib




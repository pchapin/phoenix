############################################################################
# FILE        : makefile
# LAST REVISED: 2024-06-12
# AUTHOR      : Phoenix Contributors
#
# This is the makefile for the bootloader.
############################################################################

.c.obj: .AUTODEPEND
    wcc -q -x -I$(%WATCOM)\h -ms -s -w4 -wcd201 -bt=dos -fo=$[*.obj $[*.c
# The -x -I... options are needed because right now loader.c references certain functions in
# the Open Watcom library. However, loader.c should be more like a Phoenix application and
# not need the Open Watcom library at all. When that is fixed, the -x -I... can be removed
# from the command line above.

all: boot.bin loader.com

boot.bin:   boot.asm
	nasm -o boot.bin boot.asm

loader.com: loader.obj
    wlink @<<link.rsp
option quiet
option map
system com
libpath (%WATCOM)\lib286\dos
library clibs.lib   # Eventually this should not be needed.
name loader.com
file loader.obj
<<

clean: .SYMBOLIC
    rm -f *.obj *.err *.bak *.exe *.map *.bin *.com

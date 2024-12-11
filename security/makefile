############################################################################
# FILE        : makefile
# LAST REVISED: 2024-12-11
# AUTHOR      : Phoenix Contributors
#
# This is the makefile for the stage 2 bootloader.
############################################################################

.c.obj: .AUTODEPEND
    wcc -q -ms -s -w4 -wcd201 -bt=dos $[*.c

all: loader.com id_inject.exe

loader.com: loader.obj
    wlink @<<link.rsp
option quiet
system com
libpath $(WATCOM)/lib286/dos
library clibs.lib   # Eventually this should not be needed.
name loader.com
file loader.obj
<<

id_inject.exe: id_inject.obj
    wlink @<<link.rsp
option quiet
system dos
libpath $(WATCOM)/lib286/dos
library clibs.lib   # Eventually this should not be needed.
name id_inject.exe
file id_inject.obj
<<

clean: .SYMBOLIC
    del *.obj *.err *.exe


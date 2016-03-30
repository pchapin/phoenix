############################################################################
# FILE        : makefile
# LAST REVISED: 2008-03-13
# AUTHOR      : Phoenix Contributors
#
# This is the makefile for the stage 2 bootloader.
############################################################################

.c.obj: .AUTODEPEND
    wcc /q /ms /s /w4 /wcd201 /bt=dos $[*.c

all: loader.com id_inject.exe

loader.com: loader.obj
    wlink @<<link.rsp
option quiet
system com
libpath c:\watcom\lib286\dos
library clibs.lib   # Eventually this should not be needed.
name loader.com
file loader.obj
<<

id_inject.exe: id_inject.obj
    wlink @<<link.rsp
option quiet
system dos
libpath c:\watcom\lib286\dos
library clibs.lib   # Eventually this should not be needed.
name id_inject.exe
file id_inject.obj
<<

clean: .SYMBOLIC
    del *.obj *.err *.exe




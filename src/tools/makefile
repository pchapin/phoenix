############################################################################
# FILE        : makefile
# LAST REVISED: 2024-06-12
# AUTHOR      : Phoenix Contributors
#
# This is the makefile for the tools.
############################################################################

all: copyboot.exe memalign.exe

copyboot.exe:  copyboot.c
    wcl386 -zq copyboot.c

memalign.exe:  memalign.c
    wcl386 -zq memalign.c

clean: .SYMBOLIC
    rm -f *.obj *.err *.bak *.exe *.o copyboot memalign

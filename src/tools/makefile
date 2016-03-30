############################################################################
# FILE  : makefile
# AUTHOR: Phoenix Contributors
#
# This is the makefile for the tools.
############################################################################

all: copyboot memalign

copyboot:  copyboot.c
    wcl386 -zq copyboot.c

memalign:  memalign.c
    wcl386 -zq memalign.c

clean: .SYMBOLIC
    rm -f *.obj *.err *.bak *.exe *.o copyboot memalign

############################################################################
# FILE   : makefile
# AUTHOR : Phoenix Contributors
#
# This is the master makefile for Phoenix. It builds everything.
############################################################################

all:    .SYMBOLIC
    cd RTL
    wmake -h
    cd ../boot
    wmake -h
    cd ../kernel
    wmake -h
    cd ../modules/video
    wmake -h
    cd ../keyboard
    wmake -h
    cd ../audio
    wmake -h
    cd ../gamepad
    wmake -h
    cd ../../apps
    wmake -h
    cd ../tools
    wmake -h
    cd ..

debug:    .SYMBOLIC
    cd RTL
    wmake -h DEBUG="-dDEBUG"
    cd ../boot
    wmake -h
    cd ../kernel
    wmake -h
    cd ../modules/video
    wmake -h
    cd ../keyboard
    wmake -h
    cd ../audio
    wmake -h
    cd ../gamepad
    wmake -h
    cd ../../apps
    wmake -h
    cd ../tools
    wmake -h
    cd ..


clean:  .SYMBOLIC
    cd RTL
    wmake -h clean
    cd ../boot
    wmake -h clean
    cd ../kernel
    wmake -h clean
    cd ../modules/video
    wmake -h clean
    cd ../keyboard
    wmake -h clean
    cd ../audio
    wmake -h clean
    cd ../gamepad
    wmake -h clean
    cd ../../apps
    wmake -h clean
    cd ../tools
    wmake -h clean
    cd ..


# The following rules build the old KOS system. They are retained for
# future reference. Eventually, once this attempt at entering protected
# mode is integrated into the main Phoenix code base, the rules below
# can be deleted (probably actually moved elsewhere).
#
#boot.exe:       boot.obj kos.obj
#        wlink sys dos name boot.exe option m file boot.obj,kos.obj
#
#boot.obj:       boot.asm segment.inc
#        wasm boot.asm
#
#kos.obj:        kos.asm segment.inc
#        wasm kos.asm
#
#little.obj:     little.c
#        wcc386 -3s -s little.c


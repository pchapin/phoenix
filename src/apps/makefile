############################################################################
# FILE        : makefile
# LAST REVISED: 2024-06-12
# AUTHOR      : Phoenix Contributors
#
# This is the makefile for the test and demonstration applications.
############################################################################

.c.obj: .AUTODEPEND
    wcc -q -ms -s -w4 -wcd201 -bt=dos -x -fo=$[*.obj -I..\RTL -I..\kernel -I..\modules\video -I..\modules\keyboard -I..\modules\audio -I..\modules\gamepad $[*.c

.asm.obj:
    wasm -fo=$[*.obj $[*.asm

all: phxlives.exe &
     anima.exe    &
     bounce.exe   &
     breakout.exe &
     fish.exe     &
     tstbeep.exe  &
     tstcond.exe  &
     tstgpad.exe  &
     tstgrphc.exe &
     tstkybrd.exe &
     tstmutex.exe &
     sprmutex.exe &
     tstvideo.exe

phxlives.exe: startup.obj phxlives.obj
    wlink @<<link.rsp
option quiet
option NODEFAULTLIBS
option map
system dos
name phxlives.exe
file startup.obj
file phxlives.obj
library ../RTL/RTL.lib
library ../kernel/kernel.lib
library ../modules/video/video.lib
<<

anima.exe: startup.obj anima.obj
    wlink @<<link.rsp
option quiet
option NODEFAULTLIBS
system dos
name anima.exe
file startup.obj
file anima.obj
library ../RTL/RTL.lib
library ../kernel/kernel.lib
library ../modules/video/video.lib
library ../modules/keyboard/keyboard.lib
library ../modules/audio/audio.lib
<<

bounce.exe: startup.obj bounce.obj
    wlink @<<link.rsp
option quiet
option NODEFAULTLIBS
system dos
name bounce.exe
file startup.obj
file bounce.obj
library ../RTL/RTL.lib
library ../kernel/kernel.lib
library ../modules/video/video.lib
library ../modules/keyboard/keyboard.lib
library ../modules/audio/audio.lib
<<

breakout.exe: startup.obj breakout.obj
    wlink @<<link.rsp
option quiet
option NODEFAULTLIBS
system dos
name breakout.exe
file startup.obj
file breakout.obj
library ../RTL/RTL.lib
library ../kernel/kernel.lib
library ../modules/video/video.lib
library ../modules/keyboard/keyboard.lib
library ../modules/audio/audio.lib
<<

fish.exe: startup.obj fish.obj
    wlink @<<link.rsp
option quiet
option NODEFAULTLIBS
system dos
name fish.exe
file startup.obj
file fish.obj
library ../RTL/RTL.lib
library ../kernel/kernel.lib
library ../modules/video/video.lib
<<

tstbeep.exe: startup.obj tstbeep.obj
    wlink @<<link.rsp
option quiet
option NODEFAULTLIBS
system dos
name tstbeep.exe
file startup.obj
file tstbeep.obj
library ../RTL/RTL.lib
library ../modules/audio/audio.lib
<<

tstcond.exe: startup.obj tstcond.obj
    wlink @<<link.rsp
option quiet
option NODEFAULTLIBS
system dos
name tstcond.exe
file startup.obj
file tstcond.obj
library ../RTL/RTL.lib
library ../kernel/kernel.lib
library ../modules/video/video.lib
<<

tstgpad.exe: startup.obj tstgpad.obj
    wlink @<<link.rsp
option quiet
option NODEFAULTLIBS
system dos
name tstgpad.exe
file startup.obj
file tstgpad.obj
library ../RTL/RTL.lib
library ../modules/gamepad/gamepad.lib
library ../modules/video/video.lib
<<

tstgrphc.exe: startup.obj tstgrphc.obj
    wlink @<<link.rsp
option quiet
option NODEFAULTLIBS
system dos
name tstgrphc.exe
file startup.obj
file tstgrphc.obj
library ../RTL/RTL.lib
library ../kernel/kernel.lib
library ../modules/video/video.lib
<<

tstkybrd.exe: startup.obj tstkybrd.obj
    wlink @<<link.rsp
option quiet
option NODEFAULTLIBS
system dos
name tstkybrd.exe
file startup.obj
file tstkybrd.obj
library ../RTL/RTL.lib
library ../kernel/kernel.lib
library ../modules/video/video.lib
library ../modules/keyboard/keyboard.lib
<<

tstmutex.exe: startup.obj tstmutex.obj
    wlink @<<link.rsp
option quiet
option NODEFAULTLIBS
system dos
name tstmutex.exe
file startup.obj
file tstmutex.obj
library ../RTL/RTL.lib
library ../kernel/kernel.lib
library ../modules/video/video.lib
<<

sprmutex.exe: startup.obj sprmutex.obj
    wlink @<<link.rsp
option quiet
option NODEFAULTLIBS
system dos
name sprmutex.exe
file startup.obj
file sprmutex.obj
library ../RTL/RTL.lib
library ../kernel/kernel.lib
library ../modules/video/video.lib
<<

# This program is currently not a Phoenix application.
tstprng.exe: startup.obj tstprng.obj
    wlink @<<link.rsp
option quiet
option NODEFAULTLIBS
system dos
name tstprng.exe
file startup.obj
file tstprng.obj
library ../modules/video/video.lib
library ../kernel/kernel.lib
library ../RTL/RTL.lib
<<

tstvideo.exe: startup.obj tstvideo.obj
    wlink @<<link.rsp
option quiet
option NODEFAULTLIBS
system dos
name tstvideo.exe
file startup.obj
file tstvideo.obj
library ../RTL/RTL.lib
library ../modules/video/video.lib
<<

clean: .SYMBOLIC
    rm -f *.obj *.err *.exe


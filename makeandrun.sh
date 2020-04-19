#!/usr/bin/env bash
#
# Contributor: Marcel Deslauriers
# Contributor: Jeremy "Ichimonji10" Audet <ichimonji10 at gmail dot com>
#
# Create a Phoenix boot image and start bochs.
#

set -o errexit
set -o nounset
readonly script_name='makeandrun.sh'
readonly usage="usage: $script_name

$script_name generates a Phoenix boot image and starts bochs. Open Watcom
environment variables (e.g., WATCOM, etc.) must be set for this script to
succeed. (if using VTC's hackbox, run 'source useOW')"

# See: http://mywiki.wooledge.org/BashFAQ/028 for how a script can locate its bundle.
if [ ! -z "${1:-}" ]; then
    echo "$usage"
    exit 1
else
    # Change to the location of the script (the root of the Phoenix project tree).
    cd "${BASH_SOURCE%/*}" || exit
    rootdir=$PWD
fi
# mtoolsrc specifies location of a: (floppy) drive
export MTOOLSRC="$rootdir"/images/mtoolsrc

#=======================================

cd "$rootdir"/src
#wmake clean
wmake

cd "$rootdir"/images
dd if=/dev/zero of=floppy.img count=2880
mformat a:
"$rootdir"/src/tools/copyboot "$rootdir"/src/boot/boot.bin floppy.img

cd "$rootdir"/src/boot
"$rootdir"/src/tools/memalign loader.com bloader.com

cd "$rootdir"/images
mcopy "$rootdir"/src/boot/bloader.com a:BLOADER.COM

# Currently the loader is limited to 10 programs. See search_root() in src/boot/loader.c.
# Demonstration programs.
mcopy "$rootdir"/src/apps/phxlives.exe a:PHXLIVES.EXE
mcopy "$rootdir"/src/apps/bounce.exe a:BOUNCE.EXE
mcopy "$rootdir"/src/apps/breakout.exe a:BREAKOUT.EXE

# Testing primitive Phoenix facilities.
mcopy "$rootdir"/src/apps/tstmutex.exe a:TSTMUTEX.EXE
mcopy "$rootdir"/src/apps/sprmutex.exe a:SPRMUTEX.EXE
mcopy "$rootdir"/src/apps/tstcond.exe a:TSTCOND.EXE

# Testing modules.
mcopy "$rootdir"/src/apps/tstvideo.exe a:TSTVIDEO.EXE
mcopy "$rootdir"/src/apps/tstkybrd.exe a:TSTKYBRD.EXE
mcopy "$rootdir"/src/apps/tstgrphc.exe a:TSTGRPHC.EXE
mcopy "$rootdir"/src/apps/tstbeep.exe a:TSTBEEP.EXE
bochs -f Phoenix.bxrc

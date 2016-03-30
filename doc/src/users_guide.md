Phoenix User's Guide
====================

This document describes how to use Phoenix. It supports both end users and programmers
interested in writing applications for the system. If you are a programmer interested in working
on the Phoenix kernel or supporting modules (drivers), you should also read the [Phoenix
Developer's Guide](developers_guide.html).

Creating a Boot Disk
--------------------

Phoenix is designed to be boot from a floppy disk. We expect most users today will be booting
Phoenix into a virtual machine using a floppy disk image---a file containing the contents of an
appropriate floppy disk. The instructions below apply to both using a physical floppy disk or a
floppy disk image.

If you are using a virtual machine, in cases where it matters, we assume you are booting Phoenix
into [bochs](http://bochs.sourceforge.net/). Adding instructions for other virtualization
software is a topic for future editions of this documentation.

### Requirements

Before you begin verify that you have the following software and/or hardware available.

+ NASM
+ Open Watcom
+ A floppy disk or a floppy disk image file.

### Instructions

In this subsection we give detailed instructions on how to create a boot disk for Phoenix, how
to install some of the sample programs on that disk, and how to load Phoenix and run the
samples.

You may wish to instead use the shell script `makeandrun.sh` in the root folder. This script
executes all the steps indicated here, taking you from a clean copy of the Phoenix source code
to a running bochs instance with the sample programs loaded and ready to run. However, we
provide these instructions for those of you who prefer to not use the script or wish to do only
some of the operations described here.

#### Prepare the System

* Go into the src directory.
* Type 'wmake.' This builds the Phoenix system.

#### Prepare the Floppy

* If using a physical floppy, insert it into drive A: (it is essential the disk be in drive #0).

* If using a disk image (on Linux, for example), do the following:
  + Be sure the mtools package is installed. Set the MTOOLSRC environment variable to point at
    images/mtoolsrc.
  + Go into the images directory.
  + Create the image with the command `dd if=/dev/zero of=floppy.img count=2880`
  + Format the image with the command (note that a: is bound to the image by mtoolsrc) `mformat
    a:`

#### Copy the Boot Sector

* If using a physical floppy
  + Type 'debug boot.bin.'
  + Type 'W 100 0 0 1.'
    This writes starting at address 100 to drive 0, sector 0, length 1 sector.
  + Type 'Q' to quit.

* If using a disk image (while in the images directory)
  + Type '../src/tools/copyboot ../src/boot/boot.bin floppy.img.'
    This copies the boot sector from boot.bin onto the first 512 bytes of the floppy image.

#### Copy Executables

* Go into the src/boot directory.
* Type '../tools/memalign loader.com bloader.com.'
* Copy the new bloader.com file to the floppy disk. (Use mcopy for floppy images)
  Be sure the name is upper case on the disk (use 'mdir a:' to check).
* Copy any of the apps that you want to be able to run to the floppy disk. (Use mcopy if needed)
  Be sure the names are upper case on the disk.


Playing a Game
--------------

1. Make sure the computer is off.
2. Place a Phoenix floppy disk containing Phoenix executables in the floppy drive.
3. Turn the computer on.
4. If you have more than one Phoenix executable on the disk a menu will appear.
5. Select the game you want to play by hitting the corresponding number from zero to nine on the
   keyboard.
6. Follow any additional instructions presented by the game.


Writing Applications
--------------------

The official compiler for creating Phoenix applications is Open Watcom. Phoenix runs on 16 bit
real-mode 80x86 processors and Open Watcom is one of the few compilers still maintained that can
generate code for that architecture.

Phoenix is a micro-kernel system designed for small machines. Each application contains a
complete copy of the operating system linked into its executable image. The executables are
"ordinary" real-mode MS-DOS executables. The Phoenix boot loader understands this format and
will properly load, relocate, and execute such files. However, Phoenix is not MS-DOS and does
not attempt (at this time) to provide an MS-DOS API. As a result you must use only Phoenix
libraries when building your application.

In particular, you must use the Phoenix C runtime library and not the C runtime library that
comes with Open Watcom. The runtime library is tied to the underlying operating system, for
example, by making appropriate system calls, and the libraries that come with Open Watcom are
for the systems Open Watcom supports. Creating the Phoenix C runtime library, in the `RTL`
folder, is a sub-project of Phoenix. The library is very minimal right now.

Note that you must use the `-x` option on the Open Watcom compiler to force it to ignore the C
library header files that come with Open Watcom. Alternatively you could set the `INCLUDE`
environment variable to point at the location of the Phoenix library headers in `RTL`. See the
Phoenix makefiles and the Open Watcom documentation for more information.

The Phoenix environment is arranged as a (micro-)kernel together with a collection of "modules"
that manage various hardware resources and perform other services. Your application communicates
with these modules by way of messages passed via the micro-kernel. To simplify your programming
the runtime library contains wrapper functions that manage the work of creating and interpreting
messages. For example the header `video.h` is an interface to the video module itself. The
header `xvideo.h` is a runtime library header that provides various convenience functions.

When a program starts the thread that enters `main` is a non-Phoenix thread executing in what is
called "load context." There are restrictions on what you can do in load context so your program
should create a Phoenix thread as soon as feasible. Your main logic must be in one (or more)
Phoenix threads. You should even try to do as much initialization as possible in a Phoenix
thread. Thus the ordinary sequence of events is as follows

1. Create a Phoenix thread using `xthread_create`.
2. Call `start_phoenix` to initialize the kernel and begin context switching. The
   `start_phoenix` function does not return. All further activity is done in your Phoenix
   threads.
3. Continue program initialization in the Phoenix thread you created in step 1.
4. Execute your program's logic in the Phoenix thread(s) you created.

Note that `start_phoenix` also creates a special idle thread that the kernel runs when all other
threads are blocked. The idle thread runs at `IDLE_PRIORITY`; you should not use that priority
for any application threads.

The structure of your application can follow the `TEST_01` sample. The template is as follows

    // Include various headers from the Phoenix C runtime library as needed.
    #include <stddef.h>
    #include <string.h>

    #include <Phoenix.h>  // Kernel API, types, etc.
    #include "video.h"    // For run_video( )
    #include "xvideo.h"   // For xvideo_print_at( ) and other video convenience functions.

    void *main_thread( void )
    {
        // Main program logic, including additional initialization.
    }

    int main( void )
    {
        processID vid_id;
        processID main_id;

        vid_id.pid  = VIDEO_PID;  // The video module *must* use this PID.
        main_id.pid = MAIN_PID;   // The main thread *should* use this PID.
	
        xthread_create( vid_id, run_video, NORMAL_PRIORITY );
        xthread_create( main_id, main_thread, NORMAL_PRIORITY );
        start_phoenix( );
        return 0;
    }

All access to the kernel API is by way of the header `Phoenix.h`. Each module is defined by
usually two headers: one for the module itself (low level) and one declaring a set of
convenience functions for processing messages to the module (high level). Since most programs
will likely need to output information on the screen you will almost certainly need to access
the video module.

In Phoenix is it necessary for you to define the process ID (also "thread ID" or "task ID") of
each thread. The video module must use `VIDEO_PID` for this purpose; several of the standard
modules have fixed or "well known" process ID values (check the documentation for each module).
Use `MAIN_PID` as the process ID of your main thread. This is actually the largest allows
process ID value. Use values such as `MAIN_PID - 1` and `MAIN_PID - 2` for your application
threads. The predefined process ID values are all at the lower end of the range and this policy
reduces the chance of process ID collisions. Note that the idle thread has a process ID of zero.

The `xthread_create` function will return an error code if the requested process ID is already
in use. You should check for this error. One exception is that when creating the initial thread
in load context the process ID `MAIN_PID` is guaranteed to be available.

The `xthread_create` function takes a pointer to a function that represents the thread's main
function. It also takes a priority value. Most threads should run at `NORMAL_PRIORITY` however
there are also `HIGH_PRIORITY` and `LOW_PRIORITY` levels. Phoenix currently uses round robin
within priorities as its scheduling algorithm. There is no starvation prevention; a high
priority thread that is always runnable will completely inhibit threads of lower priority.

The video module is driven by a thread running in the `run_video` function. It is necessary to
create a thread for that purpose or else the video module will not operate. Other modules, such
as the keyboard handling module, may have similar requirements.

For more information about the Phoenix API generate the doxygen documentation by running the
Doxgyen program in the top level `src` folder. The documentation is put in `doc/internal`.

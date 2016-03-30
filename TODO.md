
TODO Items
==========

+ Port src/boot/boot.asm to WASM syntax. This will remove the dependency on NASM and allow the
  entire Phoenix system to be built with Open Watcom tools.

+ It would be nice if there was a shell script or equivalent that went through the steps of
  setting up a boot disk. The script should allow a boot disk to be created from scratch and
  also support adding new programs to an existing boot disk. I'm imagining some kind of "boot
  disk management" script.

Documentation
-------------

+ Flesh out the Doxygen documentation.

+ Elaborate on the Developer's Guide.


Clean Up
--------

+ Normalize the test program file names in src/apps.

+ Remove all instances of camelNotation and replace them with names that are more in keeping
  with traditional_style.

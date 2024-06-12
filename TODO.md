
TODO Items
==========

+ Port src/boot/boot.asm to WASM syntax. This will remove the dependency on NASM and allow the
  entire Phoenix system to be built with Open Watcom tools.

+ The script that sets up the boot disk should be ported to Windows (e.g., PowerShell). The
  script coupld also be elaborated to include additional features.
  
+ Creating the boot disk requires `mtools` which is a Unix toolkit. It is available for Cygwin.
  Scripts and documentation need to be updated to account for this.

Documentation
-------------

+ Flesh out the Doxygen documentation.

+ Elaborate on the Developer's Guide.


Clean Up
--------

+ Normalize the test program file names in src/apps.

+ Remove all instances of camelNotation and replace them with names that are more in keeping
  with traditional_style.

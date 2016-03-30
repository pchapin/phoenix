Documentation
=============

This directory and its subdirectories contain documentation for the Phoenix
project. The `src` directory contains "raw" files, such as markdown and dot
files, and the `gen` directory contains compiled files, such as HTML and SVG
files. You should never place contents of `gen` under version control. Instead,
simply edit the files in `src` and compile them as needed.

The `gen-docs.sh` script provides an easy way to compile the files in `src`.
Running `gen-docs.sh` with no arguments arguments causes the script to print out
usage instructions. For the impatient:

    ./gen-docs.sh src

You will not be able to compile files unless you have
[markdown](http://daringfireball.net/projects/markdown/) and
[dot](http://www.linuxdevcenter.com/pub/a/linux/2004/05/06/graphviz_dot.html)
installed.

#!/bin/bash

# This script counts the number of lines in the Phoenix code base.

function process_dir
{
  echo "Processing $1"
  for FILE in $1/*.c $1/*.h $1/*.asm $1/*.inc; do
    wc -l $FILE 2> /dev/null
  done | awk '/^[0-9]+/ { sum += $1 }; END { print " Total lines = ", sum }'

  # Now process subdirectories recursively.
  for SUBDIR in $1/*; do
    if [ -d $SUBDIR ]; then
      process_dir $SUBDIR
    fi
  done
}


process_dir .


#!/usr/bin/env bash
#
# Contributor: Jeremy "Ichimonji10" Audet <ichimonji10 at gmail dot com>
#
# Generate documentation for Phoenix.
#

set -o errexit
set -o nounset
readonly _destdir='../gen'
readonly script_name='gen-docs.sh'
readonly usage="usage: $script_name <srcdir> [<destdir>]

$script_name generates documentation for Phoenix. It copies files from <srcdir>
to <destdir>, compiling markdown and dot files in the process.

<srcdir>
    The directory containing documentation.
<destdir>
    Default: <srcdir>/$_destdir
    The directory into which files are placed."

# Fetch arguments from user. Set `srcdir` and `destdir`.
# Do not set a default `srcdir`. See: http://mywiki.wooledge.org/BashFAQ/028
if [ -z "${1:-}" ]; then
    echo "$usage"
    exit 1
else
    readonly srcdir="`readlink -e "$1"`"
fi
if [ -z "${2:-}" ]; then
    readonly destdir="$srcdir/$_destdir"
else
    readonly destdir="`readlink -e "$2"`"
fi

# Copy and compile files.
# For the confused: http://mywiki.wooledge.org/UsingFind#Complex_actions
#
# $1: absolute path of file being compiled
# $2: absolute path of destination directory
find "$srcdir" -maxdepth 1 -type f -name '*.md' -exec sh -c '
    readonly filename="`basename "$1"`"
    markdown "$1" > "$2/${filename%md}html"
' _ {} "$destdir" ';' -o -type f -name '*.dot' -exec sh -c '
    readonly filename="`basename "$1"`"
    dot -Tsvg -o "$2/${filename%dot}svg" "$1"
' _ {} "$destdir" ';' -o -type f -exec sh -c '
    readonly filename="`basename "$1"`"
    cp "$1" "$2/$filename"
' _ {} "$destdir" ';'

#! /bin/sh
set -e
this_dir="$(readlink -f "$(dirname "$0")")"
export this_dir
export PATH="$this_dir"/bin:"$PATH"
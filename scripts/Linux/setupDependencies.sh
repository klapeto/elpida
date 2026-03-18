#!/bin/bash
#
#  Copyright (c) 2025  Ioannis Panagiotopoulos
#
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <https://www.gnu.org/licenses/>.
#

set -e

if [ "$#" -ne 5 ]; then
    echo "Usage: $0 <binary_file> <destination_folder> <search_dir> <ld_append_dir> <target_triple>"
    exit 1
fi

this_dir="$(readlink -f "$(dirname "$0")")"

BINARY="$1"
DEST_DIR="$2"
SEARCH_DIR="$3"
LD_DIR="$4"
TARGET_TRIPLE="$5"

if [ ! -f "$BINARY" ]; then
    echo "Error: Binary file '$BINARY' does not exist or is not a regular file."
    exit 1
fi

if [ ! -d "$SEARCH_DIR" ]; then
    echo "Error: Root directory '$SEARCH_DIR' does not exist."
    exit 1
fi

if [ ! -d "$DEST_DIR" ]; then
    echo "Error: Destination directory '$DEST_DIR' does not exist."
    exit 1
fi

# trim trailing /
if [ "$SEARCH_DIR" != "/" ]; then
  SEARCH_DIR="${SEARCH_DIR%/}"
fi

if [ "$DEST_DIR" != "/" ]; then
  DEST_DIR="${DEST_DIR%/}"
fi

if [ "$LD_DIR" != "/" ]; then
  LD_DIR="${LD_DIR%/}"
fi

. $this_dir/getDependencies.sh

LIBS=$(getDependencies "$BINARY" "$SEARCH_DIR" "$TARGET_TRIPLE")

if [ -z "$LIBS" ]; then
    echo "No shared libraries found in '$BINARY'."
    exit 0
fi

# clear existing libs
rm -rf "${DEST_DIR:?}/usr"

for lib in $LIBS; do
    rel_path="${lib#$SEARCH_DIR/}"

    dest_dir="$DEST_DIR/$rel_path"

    mkdir -p "$(dirname "$dest_dir")"

    echo "Copying $lib to $dest_dir"
    cp -P "$lib" "$dest_dir"
done

function readConfFile() {
  local conf_file=$1;
  while IFS= read -r line || [ -n "$line" ]; do
    # Skip empty lines and lines that start with '#' (allowing for leading whitespace)

    if [[ -z "$line" ]] || [[ $line =~ .*#.* ]]; then
      continue
    fi

    ldFile="$ldFile:$LD_DIR$line"
  done < "$conf_file"
}

echo "Creating ldconfig environment file"
ldFile="export LD_LIBRARY_PATH=\"\$LD_LIBRARY_PATH:$LD_DIR/usr/lib"

if [ -f "$SEARCH_DIR/etc/ld.so.conf" ]; then
  readConfFile "$SEARCH_DIR/etc/ld.so.conf"
fi

if [ -d "$SEARCH_DIR/etc/ld.so.conf.d" ]; then
    for conf_file in $SEARCH_DIR/etc/ld.so.conf.d/*.conf; do
      readConfFile "$conf_file"
    done
fi

echo "#!/bin/sh" > "$DEST_DIR/ld.env.sh"
echo "$ldFile\"" >> "$DEST_DIR/ld.env.sh"

echo "Created '$DEST_DIR/ld.env.sh'"
echo "Done setting up dependencies"
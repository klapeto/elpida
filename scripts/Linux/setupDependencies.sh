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

if [ "$#" -ne 4 ]; then
    echo "Usage: $0 <binary_file> <destination_folder> <rootdir> <ld_append_dir>"
    exit 1
fi

BINARY="$1"
DEST_DIR="$2"
ROOT_DIR="$3"
LD_DIR="$4"

if [ ! -f "$BINARY" ]; then
    echo "Error: Binary file '$BINARY' does not exist or is not a regular file."
    exit 1
fi

if [ ! -d "$ROOT_DIR" ]; then
    echo "Error: Root directory '$ROOT_DIR' does not exist."
    exit 1
fi

if [ ! -d "$DEST_DIR" ]; then
    echo "Error: Destination directory '$DEST_DIR' does not exist."
    exit 1
fi

# trim trailing /
if [ "$ROOT_DIR" != "/" ]; then
  ROOT_DIR="${ROOT_DIR%/}"
fi

if [ "$DEST_DIR" != "/" ]; then
  DEST_DIR="${DEST_DIR%/}"
fi

if [ "$LD_DIR" != "/" ]; then
  LD_DIR="${LD_DIR%/}"
fi

LIBS=$(objdump -p "$BINARY" | awk '/NEEDED/ {print $NF}')

if [ -z "$LIBS" ]; then
    echo "No shared libraries found in '$BINARY'."
    exit 0
fi

searchDirectories=("usr" "lib" "lib64")

libCopied=0
for lib in $LIBS; do
  for directory in "${searchDirectories[@]}"; do
    if [ $libCopied == 1 ]; then
        # if a previous directory copied, then skipp the rest
        libCopied=0;
        break;
    fi

    found=$(find "$ROOT_DIR/$directory" \( -type l -o -type f \) -name "$lib" 2>/dev/null)

    if [ -z "$found" ]; then
        continue
    fi

    for candidate in $found; do
         if [ -L "$candidate" ]; then
             lib_path="$candidate"
             break;
         fi
    done

    allFiles=$(find -L "$ROOT_DIR" -samefile "$lib_path" 2>/dev/null)

    for candidate in $allFiles; do

        if [[ "$candidate" != *"$directory"* ]]; then
          continue;
        fi

        rel_path="${candidate#$ROOT_DIR/}"

        dest_dir="$DEST_DIR/$rel_path"

        mkdir -p "$(dirname "$dest_dir")"

        echo "Copying $candidate to $dest_dir"
        cp -P "$candidate" "$dest_dir"
        libCopied=1
    done
  done
done

echo "Creating ldconfig environment file"
ldFile="export LD_LIBRARY_PATH=\"\$LD_LIBRARY_PATH"
for conf_file in $ROOT_DIR/etc/ld.so.conf.d/*.conf; do
  while IFS= read -r line || [ -n "$line" ]; do
    # Skip empty lines and lines that start with '#' (allowing for leading whitespace)

    if [[ -z "$line" ]] || [[ $line =~ .*#.* ]]; then
      continue
    fi

    ldFile="$ldFile:$LD_DIR$line"
  done < "$conf_file"
done

echo "#!/bin/sh" > $DEST_DIR/ld.env.sh
echo "$ldFile\"" >> $DEST_DIR/ld.env.sh

echo "Created '$DEST_DIR/ld.env.sh'"
echo "Done."
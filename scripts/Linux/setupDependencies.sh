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

function getDependencies() {
    if [ "$#" -ne 3 ]; then
        echo "Usage: $0 <binary_file> <search_dir> <target_triple>"
        return 1
    fi

    binary="$1"
    searchDir="$2"
    targetTriple="$3"

    if [ ! -f "$binary" ]; then
        echo "Error: Binary file '$binary' does not exist or is not a regular file."
        return 1
    fi

    if [ ! -d "$searchDir" ]; then
        echo "Error: Root directory '$searchDir' does not exist."
        return 1
    fi

    excludedFiles=$(cat "$this_dir/excludelist" | sed 's|#.*||g')

    targetArch=$(expr "$targetTriple" : '\(.*\)-.*-.*')
    targetSuffix=$(expr "$targetTriple" : '.*\(-.*-.*\)')

    # trim trailing /
    if [ "$searchDir" != "/" ]; then
        searchDir="${searchDir%/}"
    fi

    searchDirectories=("usr" "lib" "lib64")

    local allLibs;
    doGetDependencies "$binary" "$searchDir" "$targetTriple" "$allLibs"
}

function doGetDependencies() {

    local binary="$1"
    local searchDir="$2"
    local targetTriple="$3"

    local libs;
    libs=$(objdump -p "$binary" | awk '/NEEDED/ {print $NF}')

    if [ -z "$libs" ]; then
        return
    fi

    local libFound=0
    for lib in $libs; do
        libFound=0;

        if [[ " $excludedFiles " =~ .*$lib.*  ]]; then
            libFound=1
            continue;
        fi

        for directory in "${searchDirectories[@]}"; do
            local found=""
            local allFiles=""
            local lib_path=""
            if [ $libFound == 1 ]; then
                # if a previous directory copied, then skipp the rest
                break;
            fi

            # || true are for errors due to link loops
            found=$(find "$searchDir/$directory" \( -type l -o -type f \) -name "$lib" 2>/dev/null) || true
            if [ -z "$found" ]; then
                continue
            fi

            # search for the arch specific if needed (in case there is multilib installed)
            for candidate in $found; do
                case $candidate in
                *$targetSuffix*)
                    if [[ "$candidate" != *"$targetArch"* ]]; then
                        continue;
                    fi
                    lib_path="$candidate"
                    break;
                  ;;
                esac
            done

            # check whether we found a link and get all files if needed
            allFiles=$(find -L "$searchDir" -samefile "$lib_path" 2>/dev/null) || true

            for candidate in $allFiles; do
                if [[ "$candidate" != *"$directory"* ]]; then
                    continue;
                fi

                if [[ " $allLibs " =~ .*\ $candidate\ .*  ]]; then
                    libFound=1
                    continue;
                fi

                allLibs="$allLibs $candidate"
                echo "$candidate"
                doGetDependencies "$candidate" "$searchDir" "$targetTriple" "$allLibs"
                libFound=1
            done
        done

        if [ $libFound -ne 1 ]; then
            echo "Library: $lib was not found"
            return 1;
        fi
    done
}

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
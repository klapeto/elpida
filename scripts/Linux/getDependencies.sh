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

function log() {
    if [ -z "$VERBOSE" ]; then
        return
    fi
    echo "$1" >&2
}

function getDependencies() {
    if [ "$#" -ne 3 ]; then
        echo "Usage: $0 <binaryFile> <searchDir> <targetTriple>" >&2
        return 1
    fi

    binaryFile="$1"
    searchDir="$2"
    targetTriple="$3"

    if [ ! -f "$binaryFile" ]; then
        echo "Error: Binary file '$binaryFile' does not exist or is not a regular file." >&2
        return 1
    fi

    if [ ! -d "$searchDir" ]; then
        echo "Error: Root directory '$searchDir' does not exist." >&2
        return 1
    fi

    targetArch=$(expr "$targetTriple" : '\(.*\)-.*-.*')
    targetSuffix=$(expr "$targetTriple" : '.*\(-.*-.*\)')

    excludedFiles=$(cat "$this_dir/excludelist" | sed 's|#.*||g')

    # trim trailing /
    if [ "$searchDir" != "/" ]; then
        searchDir="${searchDir%/}"
    fi

    libraryPaths=$("$targetTriple-clang" --sysroot="$searchDir" -print-search-dirs | grep "libraries:")
    libraryPaths=$(expr "$libraryPaths" : 'libraries: =\(.*\)')
    libraryPaths=(${libraryPaths//:/ })

    local additionalLibs="libatomic.so"

    doGetDependencies "$binaryFile" "$additionalLibs"
}

function doGetDependencies() {

    local binaryFile="$1"
    local additionalLibs="$2"

    log "Getting dependencies of $binaryFile"

    local libs
    libs=$(objdump -p "$binaryFile" | awk '/NEEDED/ {print $NF}')

    if [ -z "$libs" ]; then
        log "  No libraries are linked to $binaryFile"
        return
    fi

    libs="$libs $additionalLibs"

    local libFound=0
    for lib in $libs; do
        libFound=0

        if [[ " $excludedFiles " =~ .*$lib.* ]]; then
            log "  $lib is excluded"
            continue
        fi
        log "  Searching for $lib"
        for directory in "${libraryPaths[@]}"; do
            local found=""
            local allFiles=""
            local lib_path=""
            if [ $libFound == 1 ]; then
                # if a previous directory copied, then skipp the rest
                log "    $lib was previously copied. Skipping"
                break
            fi

            # || true are for errors due to link loops
            found=$(find "$directory" \( -type l -o -type f \) -name "$lib" 2>/dev/null) || true
            if [ -z "$found" ]; then
                log "    $lib was not found on $directory"
                continue
            fi

            lib_path=${found[0]}

            # check whether we found a link and get all files if needed
            allFiles=$(find -L "$searchDir" -samefile "$lib_path" 2>/dev/null) || true

            for candidate in $allFiles; do
                if [[ " $allLibs " =~ .*\ $candidate\ .* ]]; then
                    log "      $candidate was previously added. Skipping"
                    libFound=1
                    continue
                fi

                allLibs="$allLibs $candidate"
                echo "$candidate"
                doGetDependencies "$candidate"
                libFound=1
            done
        done

        if [ $libFound -ne 1 ]; then
            log "  Library: $lib was not found"
            return 1
        fi
    done
}

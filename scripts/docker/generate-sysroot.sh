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

# exit if any command fails
set -e

if [ $# -ne 3 ]; then
    echo "Usage $0 target-triple sysroot-target-dir packages-file"
    exit 1;
fi

: ${DEBIAN_DISTRIBUTION:="noble"}
: ${DEBIAN_SOURCES:="https://archive.ubuntu.com/ubuntu/|main,universe https://ports.ubuntu.com/ubuntu-ports/|main,universe"}

TARGET_TRIPLE="$1"
SYSROOT="$2"
PACKAGES_FILE="$3"

case "$TARGET_TRIPLE" in
*-linux-gnu*)
    ;;
*-w64-mingw32)
    echo "Fixing Mingw32 sysroot for $TARGET_TRIPLE"
    ln -s "$SYSROOT" "$SYSROOT/usr"
    exit 0
    ;;
esac

case "$TARGET_TRIPLE" in
x86_64-*)
    DEBIAN_ARCH=amd64
    ;;
arm-linux-gnueabihf)
    DEBIAN_ARCH=armhf
    ;;
aarch64-*)
    DEBIAN_ARCH=arm64
    ;;
riscv64-*)
    DEBIAN_ARCH=riscv64
    ;;
i[63]86-*)
    DEBIAN_ARCH=i386
    ;;
*)
    echo "Unknown target: $TARGET_TRIPLE"
    exit 1;
    ;;
esac

echo "Generating a Linux/GNU sysroot for $TARGET_TRIPLE"

if [ -z "$SYSROOT" ]; then
    echo "Invalid sysroot target dir: $SYSROOT"
    exit 1;
fi

if [ ! -f "$PACKAGES_FILE" ]; then
    echo "Packages file does not exist: $PACKAGES_FILE"
    exit 1;
fi

if [ ! -f ./SysrootGenerator ] || [ ! -x ./SysrootGenerator ]; then
    wget -O ./SysrootGenerator https://github.com/klapeto/SysrootGenerator/releases/download/v1.0.3/SysrootGenerator-linux-x86-64
    chmod +x ./SysrootGenerator
fi

./SysrootGenerator --path "$SYSROOT" \
    --arch "$DEBIAN_ARCH" \
    --distribution "$DEBIAN_DISTRIBUTION" \
    --sources "$DEBIAN_SOURCES" \
    --http-timeout=300 \
    --cache-path ./cache \
    --packages `paste -sd, "$PACKAGES_FILE"` \
    --store-install-state

ldconfig
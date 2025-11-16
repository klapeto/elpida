#!/bin/bash

if [ $# -eq 0 ]; then
    echo "Error: No target architecture provided."
    exit 1
fi

../configure -static  \
    -qt-host-path /lib  \
    -prefix /opt/toolchains/llvm-mingw/$1-w64-mingw32 --  \
    -DCMAKE_SYSTEM_NAME=Windows  \
    -DCMAKE_SYSTEM_PROCESSOR=$1  \
    -DCMAKE_SYSROOT="/opt/toolchains/llvm-mingw/$1-w64-mingw32"  \
    -DQt6HostInfo_DIR=/lib/x86_64-linux-gnu/cmake/Qt6HostInfo/  \
    -DCMAKE_C_COMPILER=$1-w64-mingw32-clang  \
    -DCMAKE_CXX_COMPILER=$1-w64-mingw32-clang++  \
    -DCMAKE_FIND_ROOT_PATH="/opt/toolchains/llvm-mingw/$1-w64-mingw32"
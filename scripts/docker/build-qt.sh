#!/bin/bash

USEHOST=0
HOST=$(gcc -dumpmachine)
if [ $# -eq 0 ]; then
    USEHOST=1
    CROSSPREFIX=$HOST
else
    CROSSPREFIX=$1
fi

ARCH=$(expr match "$CROSSPREFIX" '\(.*\)-.*-.*')

OS=$(expr match "$CROSSPREFIX" '.*-\(.*\)-.*')
if [ $OS == linux ]; then
    OS=Linux
elif [ $OS == w64 ]; then
    OS=Windows
fi

SYSROOTBASE=/opt/sysroots
#SYSROOTBASE=/mnt/Dev/sysroots/
SYSROOT=$SYSROOTBASE/$CROSSPREFIX
PREFIX=$SYSROOT/usr

if [ $USEHOST -eq 0 ]; then
    echo "Cross compiling: $CROSSPREFIX"

    FLAGS="--sysroot=$SYSROOT"
    if [ $OS != Windows ]; then
      FLAGS="$FLAGS -L$SYSROOTBASE/$CROSSPREFIX/lib/$CROSSPREFIX/ -Wl,$SYSROOTBASE/$CROSSPREFIX/lib/$CROSSPREFIX/libc.a"
    fi

  ../configure \
    -static -no-shared -release \
    -submodules qtcharts,qtsvg,qtbase,qtwayland \
    -nomake examples -nomake tests -nomake benchmarks -nomake manual-tests -nomake minimal-static-tests \
    -prefix $PREFIX \
    -qt-host-path $SYSROOTBASE/$HOST/usr -- \
    -DCMAKE_C_COMPILER=$CROSSPREFIX-clang \
    -DCMAKE_CXX_COMPILER=$CROSSPREFIX-clang++ \
    -DCMAKE_ASM_COMPILER=$CROSSPREFIX-clang \
    -DCMAKE_C_FLAGS_INIT="$FLAGS" \
    -DCMAKE_CXX_FLAGS_INIT="$FLAGS" \
    -DCMAKE_LINKER_TYPE=LLD \
    -DCMAKE_SYSTEM_NAME=$OS \
    -DCMAKE_SYSTEM_PROCESSOR=$ARCH \
    -DCMAKE_SYSROOT=$SYSROOT \
    -DCMAKE_FIND_ROOT_PATH=$SYSROOT \
    -DCMAKE_FIND_ROOT_PATH_MODE_PROGRAM=NEVER \
    -DCMAKE_FIND_ROOT_PATH_MODE_LIBRARY=ONLY \
    -DCMAKE_FIND_ROOT_PATH_MODE_INCLUDE=ONLY \
    -DCMAKE_FIND_ROOT_PATH_MODE_PACKAGE=ONLY
else
  echo "Compiling for host: $CROSSPREFIX"
  ../configure \
    -static -no-shared -release \
    -submodules qtcharts,qtsvg,qtbase,qtwayland \
    -nomake examples -nomake tests -nomake benchmarks -nomake manual-tests -nomake minimal-static-tests \
    -prefix $PREFIX -- \
    -DCMAKE_C_COMPILER=$CROSSPREFIX-clang \
    -DCMAKE_CXX_COMPILER=$CROSSPREFIX-clang++ \
    -DCMAKE_ASM_COMPILER=$CROSSPREFIX-clang \
    -DCMAKE_C_FLAGS_INIT="--sysroot=$SYSROOT" \
    -DCMAKE_CXX_FLAGS_INIT="--sysroot=$SYSROOT" \
    -DCMAKE_LINKER_TYPE=LLD \
    -DCMAKE_SYSROOT=$SYSROOT\
    -DCMAKE_FIND_ROOT_PATH=$SYSROOT \
    -DCMAKE_FIND_ROOT_PATH_MODE_PROGRAM=NEVER \
    -DCMAKE_FIND_ROOT_PATH_MODE_LIBRARY=ONLY \
    -DCMAKE_FIND_ROOT_PATH_MODE_INCLUDE=ONLY \
    -DCMAKE_FIND_ROOT_PATH_MODE_PACKAGE=ONLY
fi

cmake --build . --parallel
cmake --install .
rm -rf ./*

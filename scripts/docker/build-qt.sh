#!/bin/bash

USEHOST=0
if [ $# -eq 0 ]; then
    USEHOST=1
    CROSSPREFIX=$(gcc -dumpmachine)
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

PREFIX=/opt/sysroots/$CROSSPREFIX

if [ $USEHOST -eq 0 ]; then
    echo "Cross compiling: $CROSSPREFIX"
  ../configure \
    -static \
    -prefix $PREFIX \
    -qt-host-path / \
    -submodules qtcharts,qtsvg,qtbase \
    -force-bundled-libs \
    -no-tslib -- \
    -DCMAKE_C_COMPILER=$CROSSPREFIX-clang \
    -DCMAKE_CXX_COMPILER=$CROSSPREFIX-clang++ \
    -DCMAKE_ASM_COMPILER=$CROSSPREFIX-clang \
    -DCMAKE_C_FLAGS_INIT="--sysroot=$PREFIX" \
    -DCMAKE_CXX_FLAGS_INIT="--sysroot=$PREFIX" \
    -DCMAKE_LINKER_TYPE=LLD \
    -DCMAKE_SYSTEM_NAME=$OS \
    -DCMAKE_SYSTEM_PROCESSOR=$ARCH \
    -DCMAKE_SYSROOT=$PREFIX \
    -DCMAKE_FIND_ROOT_PATH=$PREFIX \
    -DCMAKE_FIND_ROOT_PATH_MODE_PROGRAM=NEVER \
    -DCMAKE_FIND_ROOT_PATH_MODE_LIBRARY=ONLY \
    -DCMAKE_FIND_ROOT_PATH_MODE_INCLUDE=ONLY \
    -DCMAKE_FIND_ROOT_PATH_MODE_PACKAGE=ONLY
else
  echo "Compiling for host: $CROSSPREFIX"
  ../configure \
    -static \
    -prefix $PREFIX \
    -submodules qtcharts,qtsvg,qtbase \
    -force-bundled-libs \
    -no-tslib -- \
    -DCMAKE_C_COMPILER=$CROSSPREFIX-clang \
    -DCMAKE_CXX_COMPILER=$CROSSPREFIX-clang++ \
    -DCMAKE_ASM_COMPILER=$CROSSPREFIX-clang \
    -DCMAKE_C_FLAGS_INIT="--sysroot=$PREFIX" \
    -DCMAKE_CXX_FLAGS_INIT="--sysroot=$PREFIX" \
    -DCMAKE_LINKER_TYPE=LLD \
    -DCMAKE_SYSROOT=$PREFIX \
    -DCMAKE_FIND_ROOT_PATH=$PREFIX \
    -DCMAKE_FIND_ROOT_PATH_MODE_PROGRAM=NEVER \
    -DCMAKE_FIND_ROOT_PATH_MODE_LIBRARY=ONLY \
    -DCMAKE_FIND_ROOT_PATH_MODE_INCLUDE=ONLY \
    -DCMAKE_FIND_ROOT_PATH_MODE_PACKAGE=ONLY
fi

cmake --build .
cmake --install .
rm -rf ./*

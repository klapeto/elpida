#!/bin/bash

HOST=$(gcc -dumpmachine)
if [ $# -eq 0 ]; then
    USEHOST=1
    CROSSPREFIX=$HOST
else
    CROSSPREFIX=$1
fi

SYSROOTBASE=/opt/sysroots
#SYSROOTBASE=/mnt/Dev/sysroots/
SYSROOT=$SYSROOTBASE/$CROSSPREFIX
PREFIX=$SYSROOT/usr

echo "Compiling: $CROSSPREFIX"

export CC="$CROSSPREFIX-clang"
export CXX="$CROSSPREFIX-clang++"
export RANLIB="$CROSSPREFIX-ranlib"
export AR="$CROSSPREFIX-ar"
export STRIP="$CROSSPREFIX-strip"

../configure \
    --host=$CROSSPREFIX \
    --with-sysroot=$SYSROOT \
    --prefix=$PREFIX \
    --enable-static \
    --disable-shared \
    --enable-plugins=no \
    --disable-readme \
    --disable-cairo \
    --disable-libxml2 \
    --disable-io \
    --disable-pci \
    --disable-opencl \
    --disable-cuda \
    --disable-nvml \
    --disable-rsmi \
    --disable-levelzero \
    --disable-gl \
    --disable-libudev

make -j$(nproc) install
rm -rf ./*

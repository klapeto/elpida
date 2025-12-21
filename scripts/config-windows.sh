#!/bin/bash
cmake .. \
-DTARGET_PREFIX=aarch64-w64-mingw32- \
-DCMAKE_SYSTEM_NAME=Windows \
-DCMAKE_SYSTEM_PROCESSOR=aarch64 \
-DCMAKE_SYSROOT=/opt/sysroots/aarch64-w64-mingw32 \
-DCMAKE_BUILD_TYPE=Release \
-DCMAKE_TOOLCHAIN_FILE=../cmake-modules/llvm-cross.cmake

git clone -b libs-local --depth=1 https://gitlab.com/dev-hood/elpida/elpida.git && cd elpida && git submodule update --init --depth=1 && mkdir build && cd build


make -j20 llvm-tblgen llvm-config clang-tblgen && make -20
cmake -DCMAKE_TOOLCHAIN_FILE=../cmake-modules/llvm-cross.cmake -DTARGET_PREFIX=x86_64-linux-gnu- ..
cmake -DCMAKE_TOOLCHAIN_FILE=../cmake-modules/llvm-cross.cmake -DCMAKE_SYSROOT=/opt/sysroots/aarch64-linux-gnu -DTARGET_PREFIX=aarch64-linux-gnu- ..

RUN git clone -b libs-local --depth=1 https://gitlab.com/dev-hood/elpida/elpida.git && cd elpida && git submodule update --init --depth=1 && mkdir build && cd build
WORKDIR /elpida/build
ADD qt.patch ../qt.patch
RUN cd .. && patch -p1 < qt.patch
RUN rm -rf * && cmake -DCMAKE_TOOLCHAIN_FILE=../cmake-modules/llvm-cross.cmake -DCMAKE_SYSROOT=/opt/sysroots/x86_64-linux-gnu -DTARGET_PREFIX=x86_64-linux-gnu- .. && make -j20 && make -j20 install
RUN rm -rf * && cmake -DCMAKE_TOOLCHAIN_FILE=../cmake-modules/llvm-cross.cmake -DHOST_SYSROOT=/opt/sysroots/x86_64-linux-gnu -DCMAKE_SYSROOT=/opt/sysroots/aarch64-linux-gnu -DTARGET_PREFIX=aarch64-linux-gnu- .. && make -j20 && make -j20 install
RUN rm -rf * && cmake -DCMAKE_TOOLCHAIN_FILE=../cmake-modules/llvm-cross.cmake -DHOST_SYSROOT=/opt/sysroots/x86_64-linux-gnu -DCMAKE_SYSROOT=/opt/sysroots/arm-linux-gnueabihf -DTARGET_PREFIX=arm-linux-gnueabihf- .. && make -j20 && make -j20 install
RUN rm -rf * && cmake -DCMAKE_TOOLCHAIN_FILE=../cmake-modules/llvm-cross.cmake -DHOST_SYSROOT=/opt/sysroots/x86_64-linux-gnu -DCMAKE_SYSROOT=/opt/sysroots/aarch64-w64-mingw32 -DTARGET_PREFIX=aarch64-w64-mingw32- .. && make -j20 && make -j20 install
RUN rm -rf * && cmake -DCMAKE_TOOLCHAIN_FILE=../cmake-modules/llvm-cross.cmake -DHOST_SYSROOT=/opt/sysroots/x86_64-linux-gnu -DCMAKE_SYSROOT=/opt/sysroots/riscv64-w64-mingw32 -DTARGET_PREFIX=riscv64-w64-mingw32- .. && make -j20 && make -j20 install
RUN rm -rf * && cmake -DCMAKE_TOOLCHAIN_FILE=../cmake-modules/llvm-cross.cmake -DHOST_SYSROOT=/opt/sysroots/x86_64-linux-gnu -DCMAKE_SYSROOT=/opt/sysroots/armv7-w64-mingw32 -DTARGET_PREFIX=armv7-w64-mingw32- .. && make -j20 && make -j20 install
RUN rm -rf * && cmake -DCMAKE_TOOLCHAIN_FILE=../cmake-modules/llvm-cross.cmake -DHOST_SYSROOT=/opt/sysroots/x86_64-linux-gnu -DCMAKE_SYSROOT=/opt/sysroots/i686-w64-mingw32 -DTARGET_PREFIX=i686-w64-mingw32- .. && make -j20 && make -j20 install
RUN rm -rf * && cmake -DCMAKE_TOOLCHAIN_FILE=../cmake-modules/llvm-cross.cmake -DHOST_SYSROOT=/opt/sysroots/x86_64-linux-gnu -DCMAKE_SYSROOT=/opt/sysroots/x86_64-w64-mingw32 -DTARGET_PREFIX=x86_64-w64-mingw32- .. && make -j20 && make -j20 install

FROM ubuntu:22.04
LABEL authors="klapeto"

ENV DEBIAN_FRONTEND=noninteractive

#https://github.com/multiarch/qemu-user-static/tree/master

RUN apt update && apt -y full-upgrade
RUN apt install -y \
    wget \
    git \
    ninja-build \
    automake \
    autoconf \
    libtool-bin \
    patch \
    python3 \
    xz-utils \
    build-essential \
    debootstrap \
    qemu-user-binfmt

# install latest cmake
RUN test -f /usr/share/doc/kitware-archive-keyring/copyright || wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | gpg --dearmor - | tee /usr/share/keyrings/kitware-archive-keyring.gpg >/dev/null
RUN echo 'deb [signed-by=/usr/share/keyrings/kitware-archive-keyring.gpg] https://apt.kitware.com/ubuntu/ jammy main' | tee /etc/apt/sources.list.d/kitware.list >/dev/null
RUN apt-get update
RUN test -f /usr/share/doc/kitware-archive-keyring/copyright || rm /usr/share/keyrings/kitware-archive-keyring.gpg
RUN apt-get install -y kitware-archive-keyring cmake

# LLVM
WORKDIR /opt
RUN wget -O llvm https://github.com/llvm/llvm-project/releases/download/llvmorg-21.1.5/LLVM-21.1.5-Linux-X64.tar.xz
RUN mv llvm llvm.tar.xz
RUN tar xf llvm.tar.xz && mv LLVM-21.1.5-Linux-X64/ llvm && rm llvm.tar.xz
ENV PATH=/opt/llvm/bin:$PATH

## LLVM-MINGW
RUN wget -O mingw.tar.xz https://github.com/mstorsjo/llvm-mingw/releases/download/20251021/llvm-mingw-20251021-msvcrt-ubuntu-22.04-x86_64.tar.xz
RUN tar xf mingw.tar.xz && mv llvm-mingw-20251021-msvcrt-ubuntu-22.04-x86_64/ mingw && rm -rf mingw.tar.xz
ENV PATH=/opt/mingw/bin:$PATH

RUN mkdir "/sysroots"
WORKDIR /sysroots

RUN debootstrap --arch=arm64 --variant=minbase --include=build-essential,symlinks,qt6-base-dev,qt6-charts-dev,qt6-svg-dev stable aarch64-linux-gnu
RUN chroot aarch64-linux-gnu symlinks -cr .

WORKDIR /opt
RUN git clone -b 6.2.4 --depth=1 git://code.qt.io/qt/qt5.git qt6
WORKDIR /opt/qt6
RUN ./init-repository --module-subset=qtcharts,qtsvg
RUN mkdir build
WORKDIR /opt/qt6/build
#RUN ../configure -static -qt-host-path /sysroots/amd64-linux-gnu/usr/lib -prefix /opt/mingw/x86_64-w64-mingw32 -- -DCMAKE_SYSTEM_NAME=Windows -DCMAKE_SYSTEM_PROCESSOR=x86_64 -DCMAKE_SYSROOT="/opt/mingw/x86_64-w64-mingw32" -DQt6HostInfo_DIR=/sysroots/amd64-linux-gnu/lib/x86_64-linux-gnu/cmake/Qt6HostInfo/ -DCMAKE_C_COMPILER=x86_64-w64-mingw32-clang -DCMAKE_CXX_COMPILER=x86_64-w64-mingw32-clang++ -DCMAKE_FIND_ROOT_PATH="/opt/mingw/x86_64-w64-mingw32"
#RUN ../configure -static -qt-host-path /lib -prefix /opt/mingw/x86_64-w64-mingw32 -- -DCMAKE_SYSTEM_NAME=Windows -DCMAKE_SYSTEM_PROCESSOR=x86_64 -DCMAKE_SYSROOT="/opt/mingw/x86_64-w64-mingw32" -DQt6HostInfo_DIR=/lib/x86_64-linux-gnu/cmake/Qt6HostInfo/ -DCMAKE_C_COMPILER=x86_64-w64-mingw32-clang -DCMAKE_CXX_COMPILER=x86_64-w64-mingw32-clang++ -DCMAKE_FIND_ROOT_PATH="/opt/mingw/x86_64-w64-mingw32"

#cmake -DTARGET_TRIPLE=x86_64-w64-mingw32 -DTARGET_PREFIX=x86_64-w64-mingw32 -DTARGET_SYSTEM=Windows -DTARGET_ARCH=x86_64 -DTARGET_SYSROOT="/opt/mingw/x86_64-w64-mingw32" -DHOST_TOOLS_SYSROOT="/" -DCMAKE_TOOLCHAIN_FILE=../cmake-modules/llvm-cross.cmake  ..

WORKDIR /
ENTRYPOINT ["/bin/bash"]
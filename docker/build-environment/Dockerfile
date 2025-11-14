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
    build-essential

# install latest cmake
RUN test -f /usr/share/doc/kitware-archive-keyring/copyright || wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | gpg --dearmor - | tee /usr/share/keyrings/kitware-archive-keyring.gpg >/dev/null
RUN echo 'deb [signed-by=/usr/share/keyrings/kitware-archive-keyring.gpg] https://apt.kitware.com/ubuntu/ jammy main' | tee /etc/apt/sources.list.d/kitware.list >/dev/null
RUN apt-get update
RUN test -f /usr/share/doc/kitware-archive-keyring/copyright || rm /usr/share/keyrings/kitware-archive-keyring.gpg
RUN apt-get install -y kitware-archive-keyring cmake

WORKDIR /opt
RUN wget -O llvm https://github.com/llvm/llvm-project/releases/download/llvmorg-21.1.5/LLVM-21.1.5-Linux-X64.tar.xz
RUN mv llvm llvm.tar.xz
RUN tar xf llvm.tar.xz && mv LLVM-21.1.5-Linux-X64/ llvm && rm llvm.tar.xz
ENV PATH=/opt/llvm/bin:$PATH

RUN mkdir "/sysroots"
WORKDIR /sysroots
RUN debootstrap --arch=arm64 --variant=minbase --include=build-essential,symlinks,qt6-base-dev,qt6-charts-dev,qt6-svg-dev stable aarch64-linux-gnu
RUN chroot aarch64-linux-gnu symlinks -cr .
RUN debootstrap --arch=amd64 --variant=minbase --include=qt6-base-dev stable amd64-linux-gnu
RUN chroot amd64-linux-gnu symlinks -cr .
ADD ./cmake-modules/aarch64-llvm.cmake aarch64-llvm.cmake

## LLVM-MINGW
RUN wget -O mingw.tar.xz https://github.com/mstorsjo/llvm-mingw/releases/download/20251021/llvm-mingw-20251021-msvcrt-ubuntu-22.04-x86_64.tar.xz
RUN tar xf mingw.tar.xz && mv llvm-mingw-20251021-msvcrt-ubuntu-22.04-x86_64/ mingw && rm -rf mingw.tar.xz
ENV PATH=/opt/llvm-mingw/bin:$PATH

WORKDIR /
ENTRYPOINT ["/bin/bash"]
FROM ubuntu:noble
LABEL authors="klapeto"

ENV DEBIAN_FRONTEND=noninteractive

# install basic packages
RUN apt-get update && apt -y full-upgrade
RUN apt-get install -y --no-install-recommends \
    ca-certificates \
    gcc \
    g++ \
    gpg \
    wget \
    git \
    ninja-build \
    pkg-config \
    automake \
    autoconf \
    libtool-bin \
    patch \
    python3 \
    xz-utils \
    zip \
    binutils \
    zstd \
    gzip

# install latest cmake
RUN test -f /usr/share/doc/kitware-archive-keyring/copyright || wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | gpg --dearmor - | tee /usr/share/keyrings/kitware-archive-keyring.gpg >/dev/null
RUN echo 'deb [signed-by=/usr/share/keyrings/kitware-archive-keyring.gpg] https://apt.kitware.com/ubuntu/ jammy main' | tee /etc/apt/sources.list.d/kitware.list >/dev/null
RUN apt-get update
RUN test -f /usr/share/doc/kitware-archive-keyring/copyright || rm /usr/share/keyrings/kitware-archive-keyring.gpg
RUN apt-get install -y kitware-archive-keyring cmake

# Create dirs
RUN mkdir /opt/toolchains && mkdir /opt/sysroots && mkdir /opt/sources && mkdir /opt/logs

# Install LLVM-MINGW
WORKDIR /opt/sources
RUN git clone https://github.com/mstorsjo/llvm-mingw.git
WORKDIR /opt/sources/llvm-mingw
ADD ./docker/build-environment/llvm-mingw.patch llvm-mingw.patch
RUN git checkout 20251202 && patch -p1 < llvm-mingw.patch
RUN ./build-all.sh /opt/toolchains/llvm-mingw --with-default-msvcrt=msvcrt

WORKDIR /opt/toolchains
# create stubs for other crosses
ADD ./scripts/docker/create-llvm-mingw-links.sh create-llvm-mingw-links.sh
RUN cd llvm-mingw && ../create-llvm-mingw-links.sh x86_64-linux-gnu && ../create-llvm-mingw-links.sh aarch64-linux-gnu && ../create-llvm-mingw-links.sh arm-linux-gnueabihf && ../create-llvm-mingw-links.sh riscv64-linux-gnu
RUN rm create-llvm-mingw-links.sh

ENV PATH=/opt/toolchains/llvm-mingw/bin:$PATH

# linking existing sysroots for consistency
ADD ./scripts/docker/link-sysroot.sh link-sysroot.sh
RUN ./link-sysroot.sh /opt/toolchains/llvm-mingw/aarch64-w64-mingw32 aarch64-w64-mingw32
RUN ./link-sysroot.sh /opt/toolchains/llvm-mingw/i686-w64-mingw32 i686-w64-mingw32
RUN ./link-sysroot.sh /opt/toolchains/llvm-mingw/x86_64-w64-mingw32 x86_64-w64-mingw32

# cleanup
RUN apt-get -y autoremove && apt-get autoclean

WORKDIR /opt/sources
RUN git clone --depth=1 -b release/19.x https://github.com/llvm/llvm-project.git
RUN git clone --depth=1 -b v6.10.0 git://code.qt.io/qt/qt5.git qt6 && cd qt6 && ./init-repository --module-subset=qtcharts,qtsvg,qtbase
RUN git clone --depth=1 -b hwloc-2.4.0 https://github.com/open-mpi/hwloc.git && cd hwloc && ./autogen.sh

WORKDIR /opt/sysroots
ADD ./scripts/docker/packages ./scripts/docker/generate-sysroot.sh ./
RUN ./generate-sysroot.sh "amd64" "https://archive.ubuntu.com/ubuntu/" "x86_64-linux-gnu"

# Append loader libs for Qt tools run
RUN echo "/opt/sysroots/x86_64-linux-gnu/usr/lib/x86_64-linux-gnu" > /etc/ld.so.conf.d/cross.conf && ldconfig

WORKDIR /opt/sources
RUN mkdir -p qt6/build
WORKDIR /opt/sources/qt6/build
ADD ./scripts/docker/build-qt.sh ../build-qt.sh
ADD ./scripts/docker/build-base.sh ../build-base.sh
RUN TARGET_TRIPLE=x86_64-linux-gnu ../build-qt.sh

WORKDIR /

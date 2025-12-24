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
    zip

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

WORKDIR /opt/sysroots

RUN apt-get -y install binutils zstd gzip xz-utils
ADD ./scripts/docker/packages packages
ADD ./docker/build-environment/SysrootGenerator ./SysrootGenerator
RUN ./SysrootGenerator --arch=amd64 --distribution=noble --sources="https://archive.ubuntu.com/ubuntu/|main,universe" --packages=`paste -sd, packages` --path=x86_64-linux-gnu
RUN ./SysrootGenerator --arch=arm64 --distribution=noble --sources="https://ports.ubuntu.com/ubuntu-ports/|main,universe" --packages=`paste -sd, packages` --path=aarch64-linux-gnu
RUN ./SysrootGenerator --arch=armhf --distribution=noble --sources="https://ports.ubuntu.com/ubuntu-ports/|main,universe" --packages=`paste -sd, packages` --path=arm-linux-gnueabihf
RUN ./SysrootGenerator --arch=riscv64 --distribution=noble --sources="https://ports.ubuntu.com/ubuntu-ports/|main,universe" --packages=`paste -sd, packages` --path=riscv64-linux-gnu
RUN rm ./SysrootGenerator packages

# linking existing sysroots for consistency
RUN ln -s /opt/toolchains/llvm-mingw/aarch64-w64-mingw32 aarch64-w64-mingw32
RUN ln -s /opt/toolchains/llvm-mingw/armv7-w64-mingw32 armv7-w64-mingw32
RUN ln -s /opt/toolchains/llvm-mingw/i686-w64-mingw32 i686-w64-mingw32
RUN ln -s /opt/toolchains/llvm-mingw/x86_64-w64-mingw32 x86_64-w64-mingw32

# Append loader libs for Qt tools run
RUN echo "/opt/sysroots/x86_64-linux-gnu/usr/lib/x86_64-linux-gnu" > /etc/ld.so.conf.d/cross.conf && ldconfig

# build LLVM
WORKDIR /opt/sources
RUN git clone --depth=1 -b release/19.x https://github.com/llvm/llvm-project.git
RUN mkdir /opt/sources/llvm-project/llvm/build
WORKDIR /opt/sources/llvm-project/llvm/build
ADD ./scripts/docker/build-llvm.sh ../build-llvm.sh
ADD ./scripts/docker/build-base.sh ../build-base.sh
RUN ../build-llvm.sh
RUN TARGET_TRIPLE=aarch64-linux-gnu ../build-llvm.sh
RUN TARGET_TRIPLE=arm-linux-gnueabihf ../build-llvm.sh
RUN TARGET_TRIPLE=riscv64-linux-gnu ../build-llvm.sh
RUN TARGET_TRIPLE=aarch64-w64-mingw32 ../build-llvm.sh
RUN TARGET_TRIPLE=i686-w64-mingw32 ../build-llvm.sh
RUN TARGET_TRIPLE=x86_64-w64-mingw32 ../build-llvm.sh

# Clone Qt6
WORKDIR /opt/sources
RUN git clone -b v6.10.0 --depth=1 git://code.qt.io/qt/qt5.git qt6
RUN cd qt6 && ./init-repository --module-subset=qtcharts,qtsvg,qtbase && mkdir build

WORKDIR /opt/sources/qt6/build

ADD ./scripts/docker/build-qt.sh ../build-qt.sh
ADD ./scripts/docker/build-base.sh ../build-base.sh

# Workaround a Qt6 bug where configuration does not detect the sysroot libxkbcommon-x11
RUN apt-get install -y libxkbcommon-x11-dev

# Build Qt
RUN ../build-qt.sh
RUN TARGET_TRIPLE=aarch64-linux-gnu ../build-qt.sh
RUN TARGET_TRIPLE=arm-linux-gnueabihf ../build-qt.sh
RUN TARGET_TRIPLE=riscv64-linux-gnu ../build-qt.sh
RUN TARGET_TRIPLE=aarch64-w64-mingw32 ../build-qt.sh
RUN TARGET_TRIPLE=i686-w64-mingw32 ../build-qt.sh
RUN TARGET_TRIPLE=x86_64-w64-mingw32 ../build-qt.sh

WORKDIR /opt/sources/

# Build HWLOC
RUN git clone --depth=1 -b hwloc-2.4.0 https://github.com/open-mpi/hwloc.git && cd hwloc && ./autogen.sh && mkdir build

WORKDIR /opt/sources/hwloc/build
ADD ./scripts/docker/build-hwloc.sh ../build-hwloc.sh
ADD ./scripts/docker/build-base.sh ../build-base.sh

RUN ../build-hwloc.sh
RUN TARGET_TRIPLE=aarch64-linux-gnu ../build-hwloc.sh
RUN TARGET_TRIPLE=arm-linux-gnueabihf ../build-hwloc.sh
RUN TARGET_TRIPLE=riscv64-linux-gnu ../build-hwloc.sh
RUN TARGET_TRIPLE=aarch64-w64-mingw32 ../build-hwloc.sh
RUN TARGET_TRIPLE=i686-w64-mingw32 ../build-hwloc.sh
RUN TARGET_TRIPLE=x86_64-w64-mingw32 ../build-hwloc.sh

# Cleanup
RUN rm -rf /opt/sources

WORKDIR /

## setup appimagetool
#RUN wget -O appimagetool https://github.com/AppImage/appimagetool/releases/download/continuous/appimagetool-x86_64.AppImage
#RUN chmod +x appimagetool && mkdir /opt/appimagetool && cd /opt/appimagetool && /appimagetool --appimage-extract
#RUN ln -s /opt/appimagetool/squashfs-root/AppRun /opt/appimagetool/squashfs-root/appimagetool
#RUN rm /appimagetool
#ENV PATH=/opt/appimagetool/squashfs-root:$PATH

# cleanup packages
RUN apt-get -y autoremove && apt-get autoclean

CMD ["/bin/bash"]
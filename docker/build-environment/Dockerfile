FROM ubuntu:22.04
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
    automake \
    autoconf \
    libtool-bin \
    patch \
    python3 \
    xz-utils \
    zip \
    libgl1-mesa-dev

# install latest cmake
RUN test -f /usr/share/doc/kitware-archive-keyring/copyright || wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | gpg --dearmor - | tee /usr/share/keyrings/kitware-archive-keyring.gpg >/dev/null
RUN echo 'deb [signed-by=/usr/share/keyrings/kitware-archive-keyring.gpg] https://apt.kitware.com/ubuntu/ jammy main' | tee /etc/apt/sources.list.d/kitware.list >/dev/null
RUN apt-get update
RUN test -f /usr/share/doc/kitware-archive-keyring/copyright || rm /usr/share/keyrings/kitware-archive-keyring.gpg
RUN apt-get install -y kitware-archive-keyring cmake

# Create dirs
RUN mkdir /opt/toolchains && mkdir /opt/sysroots && mkdir /opt/sources && mkdir /opt/logs

# Install LLVM-MINGW
WORKDIR /opt/toolchains
RUN wget -O mingw.tar.xz https://github.com/mstorsjo/llvm-mingw/releases/download/20251021/llvm-mingw-20251021-msvcrt-ubuntu-22.04-x86_64.tar.xz
RUN tar xf mingw.tar.xz && mv llvm-mingw-20251021-msvcrt-ubuntu-22.04-x86_64/ llvm-mingw && rm -rf mingw.tar.xz

# create stubs for other crosses
ADD ./scripts/docker/create-llvm-mingw-links.sh create-llvm-mingw-links.sh
RUN cd llvm-mingw && ../create-llvm-mingw-links.sh x86_64-linux-gnu && ../create-llvm-mingw-links.sh aarch64-linux-gnu && ../create-llvm-mingw-links.sh arm-linux-gnueabihf
RUN rm create-llvm-mingw-links.sh

ENV PATH=/opt/toolchains/llvm-mingw/bin:$PATH

WORKDIR /opt/sysroots

RUN apt-get -y install nano debootstrap
RUN debootstrap --arch=arm64 --variant=buildd --include=libnuma-dev,libgl1-mesa-dev jammy aarch64-linux-gnu
RUN debootstrap --arch=armhf --variant=buildd --include=libnuma-dev,libgl1-mesa-dev jammy arm-linux-gnueabihf

# linking existing sysroots for consistency
RUN ln -s / x86_64-linux-gnu
RUN ln -s /opt/toolchains/llvm-mingw/aarch64-w64-mingw32 aarch64-w64-mingw32
RUN ln -s /opt/toolchains/llvm-mingw/armv7-w64-mingw32 armv7-w64-mingw32
RUN ln -s /opt/toolchains/llvm-mingw/i686-w64-mingw32 i686-w64-mingw32
RUN ln -s /opt/toolchains/llvm-mingw/x86_64-w64-mingw32 x86_64-w64-mingw32

## Clone Qt6
WORKDIR /opt/sources
RUN git clone -b v6.10.0 --depth=1 git://code.qt.io/qt/qt5.git qt6
RUN cd qt6 && ./init-repository --module-subset=qtcharts,qtsvg,qtbase && mkdir build

WORKDIR /opt/sources/qt6/build

ADD ./scripts/docker/build-qt.sh ../build-qt.sh

### Build Qt
RUN ../build-qt.sh
RUN ../build-qt.sh aarch64-linux-gnu
RUN ../build-qt.sh arm-linux-gnueabihf
RUN ../build-qt.sh aarch64-w64-mingw32
RUN ../build-qt.sh armv7-w64-mingw32
RUN ../build-qt.sh i686-w64-mingw32
RUN ../build-qt.sh x86_64-w64-mingw32

# Cleanup
RUN rm -rf /opt/sources

WORKDIR /

# setup appimagetool
RUN wget -O appimagetool https://github.com/AppImage/appimagetool/releases/download/continuous/appimagetool-x86_64.AppImage
RUN chmod +x appimagetool && mkdir /opt/appimagetool && cd /opt/appimagetool && /appimagetool --appimage-extract
RUN ln -s /opt/appimagetool/squashfs-root/AppRun /opt/appimagetool/squashfs-root/appimagetool
RUN rm /appimagetool
ENV PATH=/opt/appimagetool/squashfs-root:$PATH

# cleanup packages
RUN apt-get -y autoremove && apt-get autoclean

CMD ["/bin/bash"]
FROM ubuntu:22.04
LABEL authors="klapeto"

ENV DEBIAN_FRONTEND=noninteractive

RUN apt update && apt -y full-upgrade
RUN apt install -y --no-install-recommends \
    ca-certificates \
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
    build-essential \
    qt6-base-dev \
    libqt6svg6-dev  \
    libqt6charts6-dev \
    libgl1-mesa-dev

# install latest cmake
RUN test -f /usr/share/doc/kitware-archive-keyring/copyright || wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | gpg --dearmor - | tee /usr/share/keyrings/kitware-archive-keyring.gpg >/dev/null
RUN echo 'deb [signed-by=/usr/share/keyrings/kitware-archive-keyring.gpg] https://apt.kitware.com/ubuntu/ jammy main' | tee /etc/apt/sources.list.d/kitware.list >/dev/null
RUN apt-get update
RUN test -f /usr/share/doc/kitware-archive-keyring/copyright || rm /usr/share/keyrings/kitware-archive-keyring.gpg
RUN apt-get install -y kitware-archive-keyring cmake

# Create dirs
RUN mkdir /opt/toolchains && mkdir /opt/sysroots && mkdir /opt/sources

# Install LLVM-MINGW
WORKDIR /opt/toolchains
RUN wget -O mingw.tar.xz https://github.com/mstorsjo/llvm-mingw/releases/download/20251021/llvm-mingw-20251021-msvcrt-ubuntu-22.04-x86_64.tar.xz
RUN tar xf mingw.tar.xz && mv llvm-mingw-20251021-msvcrt-ubuntu-22.04-x86_64/ llvm-mingw && rm -rf mingw.tar.xz
ENV PATH=/opt/toolchains/llvm-mingw/bin:$PATH

# Clone Qt6
WORKDIR /opt/sources
RUN git clone -b 6.2.4 --depth=1 git://code.qt.io/qt/qt5.git qt6
RUN cd qt6 && ./init-repository --module-subset=qtcharts,qtsvg,qtbase && mkdir build

WORKDIR /opt/sources/qt6/build
ADD ./scripts/build-qt-windows.sh ../build-qt-windows.sh

# Build Qt6 x86_64-w64-mingw32
RUN ../build-qt-windows.sh x86_64
RUN cmake --build .
RUN cmake --install .
RUN rm -rf *

# Build Qt6 i686-w64-mingw32
RUN rm -rf *
RUN ../build-qt-windows.sh i686
RUN cmake --build .
RUN cmake --install .
RUN rm -rf *

# Build Qt6 aarch64-w64-mingw32
RUN ../build-qt-windows.sh aarch64
RUN cmake --build .
RUN cmake --install .

# Cleanup
WORKDIR /opt/sources
RUN rm -rf *

# Add ARM repos
RUN echo "deb [arch=armhf,arm64] http://ports.ubuntu.com/ jammy main restricted" > /etc/apt/sources.list.d/arm-cross-compile-sources.list && \
    echo "deb [arch=armhf,arm64] http://ports.ubuntu.com/ jammy-updates main restricted" >> /etc/apt/sources.list.d/arm-cross-compile-sources.list && \
    echo "deb [arch=armhf,arm64] http://ports.ubuntu.com/ jammy universe" >> /etc/apt/sources.list.d/arm-cross-compile-sources.list  && \
    echo "deb [arch=armhf,arm64] http://ports.ubuntu.com/ jammy-updates universe" >> /etc/apt/sources.list.d/arm-cross-compile-sources.list && \
    echo "deb [arch=armhf,arm64] http://ports.ubuntu.com/ jammy multiverse" >> /etc/apt/sources.list.d/arm-cross-compile-sources.list && \
    echo "deb [arch=armhf,arm64] http://ports.ubuntu.com/ jammy-updates multiverse" >> /etc/apt/sources.list.d/arm-cross-compile-sources.list && \
    echo "deb [arch=armhf,arm64] http://ports.ubuntu.com/ jammy-backports main restricted universe multiverse" >> /etc/apt/sources.list.d/arm-cross-compile-sources.list && \
    echo "deb [arch=amd64] http://gr.archive.ubuntu.com/ubuntu/ jammy main restricted universe multiverse" >> /etc/apt/sources.list

# Update caches
RUN apt-get update --fix-missing

# Install linux aarch64 libs
RUN dpkg --add-architecture arm64
RUN apt-get install -y \
    gcc-aarch64-linux-gnu \
    g++-aarch64-linux-gnu \
    libnuma-dev:arm64 \
    qt6-base-dev:arm64 \
    libqt6svg6-dev:arm64  \
    libqt6charts6-dev:arm64 \
    libgl1-mesa-dev:arm64


WORKDIR /

# setup appimagetool
RUN wget -O appimagetool https://github.com/AppImage/appimagetool/releases/download/continuous/appimagetool-x86_64.AppImage
RUN chmod +x appimagetool && mkdir /opt/appimagetool && cd /opt/appimagetool && /appimagetool --appimage-extract
RUN ln -s /opt/appimagetool/squashfs-root/AppRun /opt/appimagetool/squashfs-root/appimagetool
RUN rm /appimagetool
ENV PATH=/opt/appimagetool/squashfs-root:$PATH

CMD ["/bin/bash"]
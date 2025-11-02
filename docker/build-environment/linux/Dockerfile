FROM ubuntu:22.04
LABEL maintainer="Ioannis Panagiotopoulos yiannispana@hotmail.com"

# This part is copied and modified from https://github.com/AppImageCrafters/appimage-builder/blob/0b2f1910b2e99df2fdbb50e12b37d97246a6fa9b/Dockerfile
ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get -y full-upgrade
RUN apt-get install -y \
    ca-certificates \
    gpg \
    wget \
    zsync \
    gtk-update-icon-cache \
    squashfs-tools \
    git \
    make \
    pkg-config \
    automake \
    autoconf \
    libtool-bin \
    patch \
    g++ \
    gcc

# install latest cmake
RUN test -f /usr/share/doc/kitware-archive-keyring/copyright || wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | gpg --dearmor - | tee /usr/share/keyrings/kitware-archive-keyring.gpg >/dev/null
RUN echo 'deb [signed-by=/usr/share/keyrings/kitware-archive-keyring.gpg] https://apt.kitware.com/ubuntu/ jammy main' | tee /etc/apt/sources.list.d/kitware.list >/dev/null
RUN apt-get update
RUN test -f /usr/share/doc/kitware-archive-keyring/copyright || rm /usr/share/keyrings/kitware-archive-keyring.gpg
RUN apt-get install -y kitware-archive-keyring cmake

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

# Add new archs
RUN dpkg --add-architecture arm64 && dpkg --add-architecture armhf

# install cross libraries
RUN apt-get install -y \
    gcc-aarch64-linux-gnu \
    g++-aarch64-linux-gnu \
    libnuma-dev:arm64 \
    libnuma-dev:amd64 \
    qtbase5-dev:arm64 \
    qtbase5-dev:amd64 \
    libqt5svg5:arm64 \
    libqt5svg5:amd64 \
    libqt5x11extras5:arm64 \
    libqt5x11extras5:amd64 \
    libqt5charts5-dev:arm64 \
    libqt5charts5-dev:amd64 \
    qt5-image-formats-plugins:arm64 \
    qt5-image-formats-plugins:amd64

RUN mkdir /sources
WORKDIR /sources
RUN git clone --depth 1 -b release/19.x https://github.com/llvm/llvm-project.git llvm
RUN git clone --depth 1 -b v2.4 https://github.com/open-mpi/hwloc.git
RUN git clone --depth 1 https://github.com/klapeto/openssl-cmake.git
RUN git clone --depth 1 https://github.com/google/googletest.git
RUN git clone --depth 1 -b boost-1.89.0 https://github.com/boostorg/regex.git boost-regex
RUN git clone --depth 1 -b v1.6.50 https://github.com/pnggroup/libpng.git
RUN git clone --depth 1 -b v1.3.1 https://github.com/madler/zlib.git

COPY cmake-modules/hwloc.patch hwloc.patch
RUN patch --directory=/sources/hwloc -p1 --input=/sources/hwloc.patch

WORKDIR /

# setup appimagetool
RUN wget -O appimagetool https://github.com/AppImage/appimagetool/releases/download/continuous/appimagetool-x86_64.AppImage
RUN chmod +x appimagetool && mkdir /opt/appimagetool && cd /opt/appimagetool && /appimagetool --appimage-extract
RUN ln -s /opt/appimagetool/squashfs-root/AppRun /opt/appimagetool/squashfs-root/appimagetool
RUN rm /appimagetool
ENV PATH=/opt/appimagetool/squashfs-root:$PATH

CMD ["/bin/bash"]
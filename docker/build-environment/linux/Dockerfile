FROM ubuntu:22.04
LABEL maintainer="Ioannis Panagiotopoulos yiannispana@hotmail.com"

# This part is copied and modified from https://github.com/AppImageCrafters/appimage-builder/blob/0b2f1910b2e99df2fdbb50e12b37d97246a6fa9b/Dockerfile
ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get -y full-upgrade
RUN apt-get install -y \
    ca-certificates \
    gpg \
    wget \
    python3 \
    python-is-python3 \
    python3-pip \
    zsync \
    gtk-update-icon-cache \
    squashfs-tools \
    libglib2.0-bin \
    git \
    make \
    patch \
    pkg-config \
    automake \
    autoconf \
    libtool-bin \
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

# install patchelf
WORKDIR /tmp
RUN wget https://github.com/NixOS/patchelf/releases/download/0.12/patchelf-0.12.tar.bz2; \
    tar -xvf patchelf-0.12.tar.bz2;  \
    cd patchelf-0.12.20200827.8d3a16e; \
    ./configure && make -j$(nproc) && make install; \
    rm -rf patchelf-*

# install appimage-builder
WORKDIR /opt
RUN git clone https://github.com/AppImageCrafters/appimage-builder.git

# Force 0.14.x lief version because it crashes
RUN sed -i 's/"lief"/"lief<0.15.0"/' appimage-builder/setup.py

# python snafu requires this now...
RUN pip install --upgrade setuptools

RUN python3 -m pip install /opt/appimage-builder
RUN rm -rf /opt/appimage-builder

RUN mkdir /sources
WORKDIR /sources
RUN git clone --depth 1 -b llvmorg-19.1.7 https://github.com/llvm/llvm-project.git llvm
RUN git clone --depth 1 -b v2.4 https://github.com/open-mpi/hwloc.git
RUN git clone --depth 1 https://github.com/klapeto/openssl-cmake.git
RUN git clone --depth 1 https://github.com/google/googletest.git
RUN git clone --depth 1 -b boost-1.89.0 https://github.com/boostorg/regex.git
RUN git clone --depth 1 -b v1.6.50 https://github.com/pnggroup/libpng.git
RUN git clone --depth 1 -b v1.3.1 https://github.com/madler/zlib.git

COPY cmake-modules/hwloc.patch hwloc.patch
RUN patch --directory=/sources/hwloc -p1 --input=/sources/hwloc.patch

WORKDIR /

CMD ["/bin/bash"]
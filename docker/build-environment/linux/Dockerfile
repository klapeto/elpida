FROM ubuntu:20.04
LABEL maintainer="Ioannis Panagiotopoulos yiannispana@hotmail.com"

# This part is copied and modified from https://github.com/AppImageCrafters/appimage-builder/blob/0b2f1910b2e99df2fdbb50e12b37d97246a6fa9b/Dockerfile
ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get -y full-upgrade
RUN apt install -y \
    ca-certificates \
    gpg \
    wget \
    python3 \
    python-is-python3 \
    python3-pip \
    zsync \
    gtk-update-icon-cache \
    squashfs-tools \
    libglib2.0 \
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
RUN echo 'deb [signed-by=/usr/share/keyrings/kitware-archive-keyring.gpg] https://apt.kitware.com/ubuntu/ focal main' | tee /etc/apt/sources.list.d/kitware.list >/dev/null
RUN apt-get update
RUN test -f /usr/share/doc/kitware-archive-keyring/copyright || rm /usr/share/keyrings/kitware-archive-keyring.gpg
RUN apt-get install -y kitware-archive-keyring cmake

# Add ARM repos
RUN echo "deb [arch=armhf,arm64] http://ports.ubuntu.com/ focal main restricted" > /etc/apt/sources.list.d/arm-cross-compile-sources.list && \
    echo "deb [arch=armhf,arm64] http://ports.ubuntu.com/ focal-updates main restricted" >> /etc/apt/sources.list.d/arm-cross-compile-sources.list && \
    echo "deb [arch=armhf,arm64] http://ports.ubuntu.com/ focal universe" >> /etc/apt/sources.list.d/arm-cross-compile-sources.list  && \
    echo "deb [arch=armhf,arm64] http://ports.ubuntu.com/ focal-updates universe" >> /etc/apt/sources.list.d/arm-cross-compile-sources.list && \
    echo "deb [arch=armhf,arm64] http://ports.ubuntu.com/ focal multiverse" >> /etc/apt/sources.list.d/arm-cross-compile-sources.list && \
    echo "deb [arch=armhf,arm64] http://ports.ubuntu.com/ focal-updates multiverse" >> /etc/apt/sources.list.d/arm-cross-compile-sources.list && \
    echo "deb [arch=armhf,arm64] http://ports.ubuntu.com/ focal-backports main restricted universe multiverse" >> /etc/apt/sources.list.d/arm-cross-compile-sources.list && \
    echo "deb [arch=amd64] http://gr.archive.ubuntu.com/ubuntu/ focal main restricted universe multiverse" >> /etc/apt/sources.list

# Add KDE repo for latest QT
#RUN echo 'deb http://archive.neon.kde.org/user/ focal main' > /etc/apt/sources.list.d/neon.list && \
#    wget -qO - https://archive.neon.kde.org/public.key | apt-key add -

# Update caches
RUN apt-get update --fix-missing

# Add new archs
RUN dpkg --add-architecture arm64 && dpkg --add-architecture armhf

# install cross libraries
RUN apt install -y \
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

# install hwloc amd64
RUN git clone -b hwloc-2.4.0 https://github.com/open-mpi/hwloc.git
RUN cd hwloc && ./autogen.sh && ./configure  --enable-static --disable-cairo --disable-libxml2 --disable-io --disable-pci --disable-opencl --disable-cuda --disable-nvml --disable-rsmi --disable-gl --disable-libudev && \
    make -j$(nproc) && make install

# install hwloc aarch64
RUN cd hwloc && ./autogen.sh && ./configure --build=amd64-linux-gnu --host=aarch64-linux-gnu --prefix=/usr/aarch64-linux-gnu --with-sysroot=/usr/aarch64-linux-gnu --enable-static --disable-cairo --disable-libxml2 --disable-io --disable-pci --disable-opencl --disable-cuda --disable-nvml --disable-rsmi --disable-gl --disable-libudev && \
    make -j$(nproc) && make install
RUN rm -rf hwloc

# install patchelf
WORKDIR /tmp
RUN wget https://github.com/NixOS/patchelf/releases/download/0.12/patchelf-0.12.tar.bz2; \
    tar -xvf patchelf-0.12.tar.bz2;  \
    cd patchelf-0.12.20200827.8d3a16e; \
    ./configure && make -j$(nproc) && make install; \
    rm -rf patchelf-*

WORKDIR /opt
RUN git clone https://github.com/AppImageCrafters/appimage-builder.git

# Force 0.14.x lief version because it crashes
RUN sed -i 's/"lief"/"lief<0.15.0"/' appimage-builder/setup.py

RUN python3 -m pip install /opt/appimage-builder
RUN rm -rf /opt/appimage-builder

WORKDIR /

CMD ["/bin/bash"]
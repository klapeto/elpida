FROM ubuntu:20.04
LABEL maintainer="Ioannis Panagiotopoulos yiannispana@hotmail.com"

# This part is copied and modified from https://github.com/AppImageCrafters/appimage-builder/blob/0b2f1910b2e99df2fdbb50e12b37d97246a6fa9b/Dockerfile
ENV DEBIAN_FRONTEND=noninteractive

# install all packages needed
RUN apt-get update && \
    apt-get -y full-upgrade && \
    apt-get -yq install \
        breeze-icon-theme \
        desktop-file-utils \
        elfutils \
        fakeroot \
        file \
        git \
        gnupg2 \
        gtk-update-icon-cache \
        libgdk-pixbuf2.0-dev \
        libglib2.0-bin \
        librsvg2-dev \
        libyaml-dev \
        python3 \
        python3-pip \
        python3-setuptools \
        strace \
        wget \
        squashfs-tools \
        software-properties-common \
        zsync  \
        automake \
        libtool  \
        pkg-config \
        libnuma-dev \
        ca-certificates \
        gpg && \
    apt-get -yq autoclean

# install gcc-9, make it default
RUN apt-get update && \
    add-apt-repository -y ppa:ubuntu-toolchain-r/test && \
    apt-get -y update && \
    apt-get -y install gcc-9 g++-9 && \
    update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-9 60 --slave /usr/bin/g++ g++ /usr/bin/g++-9 && \
    apt-get -yq autoclean

# install latest cmake
RUN test -f /usr/share/doc/kitware-archive-keyring/copyright || wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | gpg --dearmor - | tee /usr/share/keyrings/kitware-archive-keyring.gpg >/dev/null
RUN echo 'deb [signed-by=/usr/share/keyrings/kitware-archive-keyring.gpg] https://apt.kitware.com/ubuntu/ focal main' | tee /etc/apt/sources.list.d/kitware.list >/dev/null
RUN apt-get update
RUN test -f /usr/share/doc/kitware-archive-keyring/copyright || rm /usr/share/keyrings/kitware-archive-keyring.gpg
RUN apt-get install -y kitware-archive-keyring cmake

# install patchelf
WORKDIR /tmp
RUN wget https://github.com/NixOS/patchelf/releases/download/0.12/patchelf-0.12.tar.bz2; \
    tar -xvf patchelf-0.12.tar.bz2;  \
    cd patchelf-0.12.20200827.8d3a16e; \
    ./configure && make -j$(nproc) && make install; \
    rm -rf patchelf-*

# install hwloc
RUN git clone -b hwloc-2.4.0 https://github.com/open-mpi/hwloc.git && \
    cd hwloc && \
    ./autogen.sh && ./configure --enable-static --disable-cairo --disable-libxml2 --disable-io --disable-pci --disable-opencl --disable-cuda --disable-nvml --disable-rsmi --disable-gl --disable-libudev && \
    make -j$(nproc) && make install && \
    cd .. && \
    rm -rf hwloc && \
    ldconfig

# update qt
RUN echo 'deb http://archive.neon.kde.org/user/ focal main' > /etc/apt/sources.list.d/neon.list && \
    wget -qO - https://archive.neon.kde.org/public.key | apt-key add - && \
    apt-get update && \
    apt-get install qtbase5-dev libqt5svg5 libqt5x11extras5 libqt5charts5-dev qt5-image-formats-plugins -y

# install appimage-builder
WORKDIR /opt
RUN git clone https://github.com/AppImageCrafters/appimage-builder.git

# Force 0.14.x lief version because it crashes
RUN sed -i 's/"lief"/"lief<0.15.0"/' appimage-builder/setup.py

RUN python3 -m pip install /opt/appimage-builder
RUN rm -rf /opt/appimage-builder

WORKDIR /

CMD ["/bin/bash"]
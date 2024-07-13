FROM ubuntu:20.04
LABEL maintainer="Ioannis Panagiotopoulos yiannispana@hotmail.com"

# This part is copied and modified from https://github.com/AppImageCrafters/appimage-builder/blob/0b2f1910b2e99df2fdbb50e12b37d97246a6fa9b/Dockerfile
ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && \
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
        zsync && \
    apt-get -yq autoclean

WORKDIR /opt
RUN git clone https://github.com/AppImageCrafters/appimage-builder.git

WORKDIR /tmp
RUN wget https://github.com/NixOS/patchelf/releases/download/0.12/patchelf-0.12.tar.bz2; \
    tar -xvf patchelf-0.12.tar.bz2;  \
    cd patchelf-0.12.20200827.8d3a16e; \
    ./configure && make -j$(nproc) && make install; \
    rm -rf patchelf-*

RUN python3 -m pip install /opt/appimage-builder
RUN rm -rf /opt/appimage-builder

WORKDIR /

 # install some hwloc build requirements
RUN apt-get install automake libtool pkg-config -y

# install hwloc
RUN git clone -b hwloc-2.4.0 https://github.com/open-mpi/hwloc.git && \
    cd hwloc && \
    ./autogen.sh && ./configure && make -j$(nproc) && make install && \
    rm -rf /hwloc && \
    ldconfig

# update qt
RUN echo 'deb http://archive.neon.kde.org/user/ focal main' > /etc/apt/sources.list.d/neon.list && \
    wget -qO - https://archive.neon.kde.org/public.key | apt-key add - && \
    apt-get update && \
    apt-get install qtbase5-dev libqt5svg5 libqt5x11extras5 libqt5charts5-dev qt5-image-formats-plugins -y

# install elpida build requirements
RUN apt-get install libnuma-dev cmake libssl-dev -y

CMD ["/bin/bash"]
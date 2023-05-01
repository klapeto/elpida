FROM appimagecrafters/appimage-builder:latest
LABEL maintainer="Ioannis Panagiotopoulos yiannispana@hotmail.com"

RUN export DEBIAN_FRONTEND=noninteractive && \
    apt-get -y update && \
    apt-get -y upgrade && \
    apt-get -y install \
        g++ \
        git \
        cmake \
        libnuma-dev \
        pkg-config \
        qtbase5-dev \
        libqt5svg5 \
        libqt5x11extras5 \
        qt5-image-formats-plugins \
        automake \
        libtool \
        libssl-dev \
        libgdk-pixbuf2.0-dev && \
    git clone https://github.com/open-mpi/hwloc && \
    cd hwloc && \
    git checkout hwloc-2.4.0 && \
    ./autogen.sh && \
    ./configure && \
    make install

CMD ["/bin/bash"]

FROM appimagecrafters/appimage-builder:latest
LABEL maintainer="Ioannis Panagiotopoulos yiannispana@hotmail.com"

RUN export DEBIAN_FRONTEND=noninteractive && \
    apt-get -y update && \
    apt-get -y upgrade && \
    apt-get -y install software-properties-common && \
    add-apt-repository -y ppa:ubuntu-toolchain-r/test && \
    apt-get -y update && \
    apt-get -y install \
        git \
        gcc-9 \
        g++-9 \
        libnuma-dev \
        pkg-config \
        qtbase5-dev \
        libqt5svg5 \
        libqt5x11extras5 \
        qt5-image-formats-plugins \
        automake \
        libtool \
        libssl-dev \
        libgdk-pixbuf2.0-dev \
        wget && \
    update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-9 90 --slave /usr/bin/g++ g++ /usr/bin/g++-9 --slave /usr/bin/gcov gcov /usr/bin/gcov-9 && \
    wget https://github.com/Kitware/CMake/releases/download/v3.26.3/cmake-3.26.3-linux-x86_64.sh && \
    chmod +x cmake-3.26.3-linux-x86_64.sh && \
    ./cmake-3.26.3-linux-x86_64.sh --skip-license && \
    git clone https://github.com/open-mpi/hwloc && \
    cd hwloc && \
    git checkout hwloc-2.4.0 && \
    ./autogen.sh && \
    ./configure && \
    make install

CMD ["/bin/bash"]

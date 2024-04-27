FROM appimagecrafters/appimage-builder
LABEL maintainer="Ioannis Panagiotopoulos yiannispana@hotmail.com"

# update GCC to 11. modified version of: https://gist.github.com/application2000/73fd6f4bf1be6600a2cf9f56315a2d91
RUN apt-get update && \
    apt-get install build-essential software-properties-common -y && \
    add-apt-repository ppa:ubuntu-toolchain-r/test -y && \
    apt-get update && \
    apt-get install gcc-snapshot -y && \
    apt-get update && \
    apt-get install gcc-11 g++-11 -y && \
    update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-11 60 --slave /usr/bin/g++ g++ /usr/bin/g++-11 && \
    update-alternatives --config gcc

 # install some cmake build requirements
RUN apt-get install git libssl-dev -y

# install latest cmake
RUN git clone https://gitlab.kitware.com/cmake/cmake.git && \
    cd cmake && \
    ./bootstrap --parallel=$(nproc) && make -j$(nproc) && make install

 # install some hwloc build requirements
RUN apt-get install automake libtool pkg-config -y

# install hwloc
RUN git clone -b hwloc-2.4.0 https://github.com/open-mpi/hwloc.git && \
    cd hwloc && \
    ./autogen.sh && ./configure && make -j$(nproc) && make install && \
    ldconfig

# update qt
RUN echo 'deb http://archive.neon.kde.org/user/ bionic main' > /etc/apt/sources.list.d/neon.list && \
    wget -qO - https://archive.neon.kde.org/public.key | apt-key add - && \
    apt-get update && \
    apt-get install qt5-default libqt5svg5 libqt5x11extras5 qt5-image-formats-plugins -y

# install elpida build requirements
RUN apt-get install libnuma-dev -y

CMD ["/bin/bash"]
FROM ubuntu:18.04
LABEL maintainer="Ioannis Panagiotopoulos yiannispana@hotmail.com"

# This part is copied and modified from https://github.com/AppImageCrafters/appimage-builder/blob/0b2f1910b2e99df2fdbb50e12b37d97246a6fa9b/Dockerfile
RUN apt-get update && apt-get -y install python3 python3-setuptools python3-pip wget fakeroot gnupg2 libglib2.0-bin file \
 desktop-file-utils libgdk-pixbuf2.0-dev librsvg2-dev libyaml-dev zsync gtk-update-icon-cache strace elfutils

ADD . /opt/appimage-builder

WORKDIR /opt/appimage-builder
RUN python3 ./setup.py install && rm -rf /opt/appimage-builder

RUN wget https://github.com/AppImage/AppImageKit/releases/download/13/appimagetool-aarch64.AppImage -O /opt/appimagetool \
    && chmod +x /opt/appimagetool \
    && cd /opt/; sed -i 's|AI\x02|\x00\x00\x00|' appimagetool; /opt/appimagetool --appimage-extract \
    && mv /opt/squashfs-root /opt/appimagetool.AppDir \
    && ln -s /opt/appimagetool.AppDir/AppRun /usr/local/bin/appimagetool

WORKDIR /tmp
RUN wget https://github.com/NixOS/patchelf/releases/download/0.12/patchelf-0.12.tar.bz2; \
    tar -xvf patchelf-0.12.tar.bz2;  \
    cd patchelf-0.12.20200827.8d3a16e; \
    ./configure && make && make install; \
    rm -rf patchelf-* \

WORKDIR /

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
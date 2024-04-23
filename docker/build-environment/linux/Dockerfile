FROM gcc:12
LABEL maintainer="Ioannis Panagiotopoulos yiannispana@hotmail.com"

RUN export DEBIAN_FRONTEND=noninteractive && \
    apt-get -y update && \
    apt-get -y upgrade && \
    apt-get -y install \
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
        wget \
        python3-pip \
        python3-setuptools \
        patchelf \
        desktop-file-utils \
        libgdk-pixbuf2.0-dev \
        fakeroot \
        strace && \
    pip3 install --break-system-packages appimage-builder && \
    wget https://github.com/AppImage/AppImageKit/releases/download/continuous/appimagetool-x86_64.AppImage -O /opt/appimagetool && \
    chmod +x /opt/appimagetool && \
    cd /opt/ && \
    /opt/appimagetool --appimage-extract && \
    mv /opt/squashfs-root /opt/appimagetool.AppDir && \
    ln -s /opt/appimagetool.AppDir/AppRun /usr/local/bin/appimagetool && \
    git clone https://github.com/open-mpi/hwloc && \
    cd hwloc && \
    git checkout hwloc-2.4.0 && \
    ./autogen.sh && \
    ./configure && \
    make install

CMD ["/bin/bash"]
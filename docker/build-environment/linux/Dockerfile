FROM ubuntu
LABEL maintainer="Ioannis Panagiotopoulos yiannispana@hotmail.com"

RUN export DEBIAN_FRONTEND=noninteractive && \
    apt-get -y update && \
    apt-get -y upgrade && \
    apt-get -y install \
        git \
        g++ \
        cmake \
        libnuma-dev \
        pkg-config \
        qtbase5-dev \
        libssl-dev \
        wget \
        libhwloc-dev \
        python3-pip \
        python3-setuptools \
        patchelf \
        desktop-file-utils \
        libgdk-pixbuf2.0-dev && \
    pip3 install appimage-builder && \
    wget https://github.com/AppImage/AppImageKit/releases/download/continuous/appimagetool-x86_64.AppImage -O /opt/appimagetool && \
    chmod +x /opt/appimagetool && \
    cd /opt/ && \
    /opt/appimagetool --appimage-extract && \
    mv /opt/squashfs-root /opt/appimagetool.AppDir && \
    ln -s /opt/appimagetool.AppDir/AppRun /usr/local/bin/appimagetool

CMD ["/bin/bash"]
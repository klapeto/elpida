FROM ubuntu
LABEL maintainer="Ioannis Panagiotopoulos yiannispana@hotmail.com"

RUN export DEBIAN_FRONTEND=noninteractive && \
    apt-get -y update && \
    apt-get -y upgrade && \
    apt-get -y install \
        autoconf \
        automake \
        autopoint \
        bash \
        bison \
        bzip2 \
        flex \
        g++ \
        g++-multilib \
        gettext \
        git \
        gperf \
        intltool \
        libc6-dev-i386 \
        libgdk-pixbuf2.0-dev \
        libltdl-dev \
        libssl-dev \
        libtool-bin \
        libxml-parser-perl \
        lzip \
        make \
        openssl \
        p7zip-full \
        patch \
        perl \
        python \
        ruby \
        sed \
        unzip \
        wget \
        xz-utils && \
    git clone https://github.com/klapeto/mxe.git && \
    cd mxe && \
    make MXE_TARGETS='x86_64-w64-mingw32.shared' MXE_PLUGIN_DIRS=plugins/gcc9 cmake hwloc qtbase qtsvg gcc openssl

ENV PATH=/mxe/usr/bin:$PATH

CMD ["/bin/bash"]
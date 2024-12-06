FROM ubuntu:22.04
LABEL maintainer="Ioannis Panagiotopoulos yiannispana@hotmail.com"

RUN export DEBIAN_FRONTEND=noninteractive && \
    apt-get -y update && \
    apt-get -y full-upgrade && \
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
    libgl-dev \
    libpcre3-dev \
    libssl-dev \
    libtool-bin \
    libxml-parser-perl \
    lzip \
    make \
    openssl \
    p7zip-full \
    patch \
    perl \
    python3 \
    python3-distutils \
    python3-mako \
    python3-pkg-resources \
    python3-packaging \
    python-is-python3 \
    ruby \
    sed \
    unzip \
    wget \
    xz-utils

RUN git clone -b elpida_updates https://github.com/klapeto/mxe.git
WORKDIR mxe

# winxp and no QT support
RUN sed -i 's/--with-default-win32-winnt=0x0601/--with-default-win32-winnt=0x0501/g' src/gcc.mk
RUN make -j$(nproc) MXE_TARGETS='i686-w64-mingw32.static' cmake hwloc gcc

WORKDIR /

ENV PATH=/mxe/usr/bin:$PATH

CMD ["/bin/bash"]

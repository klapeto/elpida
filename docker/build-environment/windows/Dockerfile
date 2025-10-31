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
    xz-utils \
    lsb-release \
    software-properties-common \
    gnupg \
    cmake

RUN git clone -b elpida_updates https://github.com/klapeto/mxe.git

COPY cmake-modules/mxe.patch mxe.patch
RUN patch --directory=/mxe -p1 --input=/mxe.patch && rm /mxe.patch

WORKDIR /mxe
RUN make --jobs=$(nproc) JOBS=$(nproc) MXE_TARGETS='x86_64-w64-mingw32.static' cmake qtbase qtsvg qtcharts gcc
RUN make --jobs=$(nproc) JOBS=$(nproc) MXE_TARGETS='i686-w64-mingw32.static' MXE_PLUGIN_DIRS=plugins/windows-xp cmake gcc

ENV PATH=/mxe/usr/bin:$PATH

# reduce size of the image
RUN make clean-junk

RUN mkdir /sources
WORKDIR /sources
RUN git clone --depth 1 -b llvmorg-19.1.7 https://github.com/llvm/llvm-project.git llvm
RUN git clone --depth 1 -b v2.4 https://github.com/open-mpi/hwloc.git
RUN git clone --depth 1 https://github.com/klapeto/openssl-cmake.git
RUN git clone --depth 1 https://github.com/google/googletest.git
RUN git clone --depth 1 -b boost-1.89.0 https://github.com/boostorg/regex.git
RUN git clone --depth 1 -b v1.6.50 https://github.com/pnggroup/libpng.git
RUN git clone --depth 1 -b v1.3.1 https://github.com/madler/zlib.git

COPY cmake-modules/llvm.WindowsXp.patch llvm.WindowsXp.patch
RUN patch --directory=/sources/llvm -p1 --input=/sources/llvm.WindowsXp.patch && rm /sources/llvm.WindowsXp.patch

COPY cmake-modules/hwloc.patch hwloc.patch
RUN patch --directory=/sources/hwloc -p1 --input=/sources/hwloc.patch  && rm /sources/hwloc.patch

WORKDIR /

CMD ["/bin/bash"]

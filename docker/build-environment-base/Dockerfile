ARG DEB_DISTRIBUTION=noble

FROM ubuntu:$DEB_DISTRIBUTION
LABEL authors="klapeto"

ARG DEB_DISTRIBUTION
ARG HOST_TRIPLE=x86_64-linux-gnu
ARG LLVM_VERSION=llvmorg-21.1.0
ENV DEBIAN_FRONTEND=noninteractive

WORKDIR /tmp

# install basic packages
RUN apt-get update && apt -y full-upgrade
RUN apt-get install -y --no-install-recommends \
    ca-certificates \
    software-properties-common \
    gcc \
    g++ \
    gpg \
    wget \
    git \
    ninja-build \
    pkg-config \
    automake \
    autoconf \
    libtool-bin \
    patch \
    python3 \
    xz-utils \
    zip \
    binutils \
    zstd \
    gzip \
    make

RUN ln -s /usr/bin/python3 /usr/bin/python

# install latest cmake
RUN wget https://github.com/Kitware/CMake/releases/download/v4.2.3/cmake-4.2.3-linux-x86_64.tar.gz -O cmake.tar.gz
RUN tar xf ./cmake.tar.gz -C /opt
ENV PATH=/opt/cmake-4.2.3-linux-x86_64/bin:$PATH

# install latest ninja
RUN wget https://github.com/ninja-build/ninja/releases/download/v1.13.2/ninja-linux.zip -O ninja.zip
RUN apt-get install -y unzip
RUN apt-get purge -y ninja-build
RUN unzip ninja.zip -d /opt/ninja
ENV PATH=/opt/ninja:$PATH

# Create dirs
RUN mkdir -p /opt/toolchains

# Build LLVM cross toolchain
RUN git clone -b unified-toolchain https://github.com/klapeto/llvm-mingw.git
WORKDIR /tmp/llvm-mingw
RUN ./build-llvm.sh /opt/toolchains/llvm

ENV PATH=/opt/toolchains/llvm/bin:$PATH

RUN ln -s /opt/toolchains/llvm/ /opt/sysroots

WORKDIR /tmp

# Clone repos that we will cross compile
RUN git clone --depth=1 -b hwloc-2.4.0 https://github.com/open-mpi/hwloc.git && \
    cd hwloc && \
    ./autogen.sh && \
    mkdir -p build

RUN git clone --depth=1 -b v6.10.0 git://code.qt.io/qt/qt5.git qt6 && \
    cd qt6 && \
    ./init-repository --module-subset=qtcharts,qtsvg,qtbase && \
    mkdir -p build

RUN git clone --depth=1 -b release/19.x https://github.com/llvm/llvm-project.git && \
    mkdir -p ./llvm-project/llvm/build

WORKDIR /tmp/

RUN apt-get install -y libgl1-mesa-dev

WORKDIR /tmp/qt6/build
RUN ../configure
RUN cmake --build . --parallel
RUN cmake --install .

WORKDIR /

CMD ["/bin/bash"]
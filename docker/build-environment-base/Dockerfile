FROM ubuntu:focal
LABEL authors="klapeto"

ENV DEBIAN_FRONTEND=noninteractive

WORKDIR /tmp

# install basic packages
ADD ./scripts/docker/packages ./
RUN apt-get update && apt -y full-upgrade
RUN apt-get install -y --no-install-recommends \
    ca-certificates \
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
    python3.8 \
    xz-utils \
    zip \
    binutils \
    zstd \
    gzip \
    make

RUN ln -s /usr/bin/python3 /usr/bin/python
# install latest cmake
RUN test -f /usr/share/doc/kitware-archive-keyring/copyright || wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | gpg --dearmor - | tee /usr/share/keyrings/kitware-archive-keyring.gpg >/dev/null
RUN echo "deb [signed-by=/usr/share/keyrings/kitware-archive-keyring.gpg] https://apt.kitware.com/ubuntu/ focal main" | tee /etc/apt/sources.list.d/kitware.list >/dev/null
RUN apt-get update
RUN test -f /usr/share/doc/kitware-archive-keyring/copyright || rm /usr/share/keyrings/kitware-archive-keyring.gpg
RUN apt-get install -y kitware-archive-keyring cmake

# Create dirs
RUN mkdir -p /opt/toolchains

# Build LLVM cross toolchain
RUN git clone -b unified-toolchain https://github.com/klapeto/llvm-mingw.git
WORKDIR /tmp/llvm-mingw
RUN git pull
RUN LLVM_VERSION=llvmorg-21.1.0 ./build-llvm.sh /opt/toolchains/llvm
RUN DEB_DISTRIBUTION=focal TARGET_TRIPLES="x86_64-linux-gnu" ./build-all.sh /opt/toolchains/llvm --with-default-msvcrt=msvcrt

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
ADD ./scripts/docker/generate-sysroot.sh ./scripts/docker/build-base.sh  ./scripts/docker/build-qt.sh ./scripts/docker/build-hwloc.sh ./scripts/docker/build-llvm.sh  ./

# qt tools will be built against the sysroot and will run for the rest archs and will need to find the sysroot libs
ENV LD_LIBRARY_PATH="/opt/sysroots/x86_64-linux-gnu/lib:/opt/sysroots/x86_64-linux-gnu/lib/x86_64-linux-gnu"

# Workaround a Qt6 bug where configuration does not detect the sysroot libxkbcommon-x11
RUN apt-get -y install libxkbcommon-x11-dev
RUN DEB_DISTRIBUTION=focal ./generate-sysroot.sh x86_64-linux-gnu /opt/sysroots/x86_64-linux-gnu /tmp/packages
RUN ./build-qt.sh x86_64-linux-gnu /opt/sysroots/x86_64-linux-gnu

WORKDIR /

CMD ["/bin/bash"]
FROM mcr.microsoft.com/devcontainers/cpp:1-ubuntu-20.04
LABEL authors="klapeto"

ARG HOST_TRIPLE
ARG TARGET_TRIPLE

ENV DEBIAN_FRONTEND=noninteractive

WORKDIR /tmp

# install basic packages
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
    make \
    xutils-dev

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
RUN LLVM_VERSION=llvmorg-21.1.0 ./build-llvm.sh /opt/toolchains/llvm
RUN DEB_DISTRIBUTION=focal TARGET_TRIPLES="$HOST_TRIPLE" ./build-all.sh /opt/toolchains/llvm --with-default-msvcrt=msvcrt

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

RUN git clone --recursive https://gitlab.freedesktop.org/xorg/lib/libxcb-cursor.git && \
    cd libxcb-cursor && \
    NOCONFIGURE=1 ./autogen.sh && \
    mkdir -p build

WORKDIR /tmp/

ADD ./scripts/docker/generate-sysroot.sh ./scripts/docker/packages ./
RUN DEB_DISTRIBUTION=focal ./generate-sysroot.sh $HOST_TRIPLE /opt/sysroots/$HOST_TRIPLE /tmp/packages

# Workaround a Qt6 bug where configuration does not detect the sysroot libxkbcommon-x11
RUN apt-get -y install libxkbcommon-x11-dev pkg-config dpkg-dev gperf
ADD ./scripts/docker/build-autotools-based.sh ./scripts/docker/build-base.sh ./
RUN PKG_CONFIG_PATH=/opt/sysroots/$HOST_TRIPLE/usr/lib/$HOST_TRIPLE/pkgconfig/ ./build-autotools-based.sh $HOST_TRIPLE /opt/sysroots/$HOST_TRIPLE $PWD/libxcb-cursor

# qt tools will be built against the sysroot and will run for the rest archs and will need to find the sysroot libs
ENV LD_LIBRARY_PATH="/opt/sysroots/$HOST_TRIPLE/lib:/opt/sysroots/$HOST_TRIPLE/lib/$HOST_TRIPLE"
ADD ./scripts/docker/build-qt.sh ./
RUN ./build-qt.sh $HOST_TRIPLE /opt/sysroots/$HOST_TRIPLE

# help qt tools because they are built against the sysroot
ENV LD_LIBRARY_PATH="/opt/sysroots/$HOST_TRIPLE/lib:/opt/sysroots/$HOST_TRIPLE/lib/$HOST_TRIPLE"
ENV DEBIAN_FRONTEND=noninteractive

WORKDIR /tmp/llvm-mingw
RUN TARGET_TRIPLES="$TARGET_TRIPLE" ./build-all.sh /opt/toolchains/llvm --with-default-msvcrt=msvcrt

WORKDIR /tmp/
ADD ./scripts/docker/packages ./scripts/docker/generate-sysroot.sh ./
RUN DEB_DISTRIBUTION=focal ./generate-sysroot.sh "$TARGET_TRIPLE" "/opt/sysroots/$TARGET_TRIPLE" /tmp/packages

ADD ./scripts/docker/build-base.sh ./scripts/docker/build-autotools-based.sh ./
RUN ./build-autotools-based.sh "$TARGET_TRIPLE" "/opt/sysroots/$TARGET_TRIPLE" "$PWD/hwloc" "--enable-plugins=no --disable-readme --disable-cairo --disable-libxml2 --disable-io --disable-pci --disable-opencl --disable-cuda --disable-nvml --disable-rsmi --disable-levelzero--disable-gl --disable-libudev"

ADD ./scripts/docker/build-linux-specific.sh ./
RUN ./build-linux-specific.sh "$TARGET_TRIPLE" "/opt/sysroots/$TARGET_TRIPLE"

ADD ./scripts/docker/build-qt.sh ./
RUN ./build-qt.sh "$TARGET_TRIPLE" "/opt/sysroots/$TARGET_TRIPLE"

ADD ./scripts/docker/build-llvm.sh ./
RUN ./build-llvm.sh "$TARGET_TRIPLE" "/opt/sysroots/$TARGET_TRIPLE"

WORKDIR /
RUN rm -rf /tmp/* \
    "/opt/sysroots/$TARGET_TRIPLE/tmp" \
    "/opt/sysroots/$TARGET_TRIPLE/var" \
    "/opt/sysroots/$TARGET_TRIPLE/usr/share" \
    "/opt/sysroots/$TARGET_TRIPLE/usr/sbom" \
    "/opt/sysroots/$TARGET_TRIPLE/usr/doc" \
    "/opt/sysroots/$TARGET_TRIPLE/usr/metatypes"

RUN if [ "$TARGET_TRIPLE" != $(gcc -dumpmachine) ]; then \
    rm -rf "/opt/sysroots/$TARGET_TRIPLE/bin" \
    "/opt/sysroots/$TARGET_TRIPLE/sbin" \
    "/opt/sysroots/$TARGET_TRIPLE/usr/bin" \
    "/opt/sysroots/$TARGET_TRIPLE/usr/sbin"; \
    fi

# cleanup
RUN apt-get -y purge g++ gcc libc6-dev automake autoconf libtool-bin xutils-dev
RUN apt-get -y autoremove && apt-get autoclean
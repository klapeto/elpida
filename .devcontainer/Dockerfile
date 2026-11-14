# Development container: docker/build-environment-base and docker/build-environment (x86_64-linux-gnu by default) in
# one file, because a dev container builds from a single Dockerfile. Keep the stages in sync with those two files.
# The final stage is the CI image plus gdb and the libraries elpida-qt needs to run here (docs/development/toolchain.md).
ARG DEB_DISTRIBUTION=focal

# Stage 1: docker/build-environment-base
FROM ubuntu:$DEB_DISTRIBUTION AS base

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

# LLVM libraries of the Compilation benchmark (scripts/docker/build-llvm.sh). Pinned: the benchmark measures this
# code, so every target must use exactly the same LLVM.
RUN git clone --depth=1 -b llvmorg-19.1.7 https://github.com/llvm/llvm-project.git && \
    mkdir -p ./llvm-project/llvm/build

RUN git clone --recursive https://gitlab.freedesktop.org/xorg/lib/libxcb-cursor.git && \
    cd libxcb-cursor && \
    NOCONFIGURE=1 ./autogen.sh && \
    mkdir -p build

WORKDIR /tmp/

RUN apt-get install -y libgl1-mesa-dev

# Host Qt (QT_HOST_PATH of the target build). The build tree is removed in the same layer.
WORKDIR /tmp/qt6/build
RUN ../configure && \
    cmake --build . --parallel && \
    cmake --install . && \
    rm -rf /tmp/qt6/build/*


# Stage 2: the builder stage of docker/build-environment
FROM base AS build

ARG DEB_DISTRIBUTION
ARG HOST_TRIPLE=x86_64-linux-gnu
ARG TARGET_TRIPLE=x86_64-linux-gnu
ARG LLVM_VERSION=llvmorg-21.1.0

ENV DEBIAN_FRONTEND=noninteractive
ENV QT_HOST_PATH=/usr/local/Qt-6.10.0
ENV TARGET_TRIPLE=$TARGET_TRIPLE
ENV HOST_TRIPLE=$HOST_TRIPLE
ENV DEB_DISTRIBUTION=$DEB_DISTRIBUTION
ENV LLVM_VERSION=$LLVM_VERSION
ENV PKG_CONFIG_SYSROOT_DIR=/opt/sysroots/$TARGET_TRIPLE
ENV PKG_CONFIG_PATH=/opt/sysroots/$TARGET_TRIPLE/usr/lib/$TARGET_TRIPLE/pkgconfig/

# Benchmark code parity flags for the runtimes and the LLVM libraries (docs/development/code-parity.md)
ADD ./scripts/docker/parity-flags.sh /tmp/

WORKDIR /tmp/llvm-mingw
ADD ./scripts/docker/llvm.xp.toolchain.v3.patch ./
RUN patch -p1 -d ./llvm-project < llvm.xp.toolchain.v3.patch
# The host compiler from stage 1 is not rebuilt (see docker/build-environment/Dockerfile)
RUN ./strip-llvm.sh /opt/toolchains/llvm && \
    TARGET_TRIPLES="$TARGET_TRIPLE" ./install-wrappers.sh /opt/toolchains/llvm && \
    TARGET_TRIPLES="$TARGET_TRIPLE" ./build-mingw-w64-tools.sh /opt/toolchains/llvm && \
    TARGET_TRIPLES="$TARGET_TRIPLE" ./build-all.sh /opt/toolchains/llvm --no-tools --with-default-msvcrt=msvcrt --with-default-win32-winnt=0x0501 --additional-cflags-script=/tmp/parity-flags.sh

WORKDIR /tmp/
ADD ./scripts/docker/packages ./scripts/docker/generate-sysroot.sh ./
RUN ./generate-sysroot.sh "$TARGET_TRIPLE" "/opt/sysroots/$TARGET_TRIPLE" /tmp/packages

ADD ./scripts/docker/build-base.sh ./scripts/docker/build-autotools-based.sh ./
RUN ./build-autotools-based.sh "$TARGET_TRIPLE" "/opt/sysroots/$TARGET_TRIPLE" "$PWD/hwloc" "--enable-plugins=no --disable-readme --disable-cairo --disable-libxml2 --disable-io --disable-pci --disable-opencl --disable-cuda --disable-nvml --disable-rsmi --disable-levelzero --disable-gl --disable-libudev"

ADD ./scripts/docker/build-llvm.sh ./scripts/docker/llvm.parity.patch ./scripts/docker/llvm.xp.lib.v3.patch ./
RUN patch -p1 -d ./llvm-project < llvm.xp.lib.v3.patch
RUN ./build-llvm.sh "$TARGET_TRIPLE" "/opt/sysroots/$TARGET_TRIPLE"

ADD ./scripts/docker/build-linux-specific.sh ./
RUN apt-get install -y gperf dpkg-dev
RUN ./build-linux-specific.sh "$TARGET_TRIPLE" "/opt/sysroots/$TARGET_TRIPLE"

ADD ./scripts/docker/build-qt.sh ./scripts/docker/generate-toolchain-file.sh ./scripts/docker/cross.linux.cmake.template ./
RUN ./generate-toolchain-file.sh "$TARGET_TRIPLE" "/opt/sysroots/$TARGET_TRIPLE"
RUN ./build-qt.sh "$TARGET_TRIPLE" "/opt/sysroots/$TARGET_TRIPLE"

# usr/share/pkgconfig stays: xproto.pc and the other X11 protocol .pc files the Qt xcb plugins look up
RUN find "/opt/sysroots/$TARGET_TRIPLE/usr/share" -mindepth 1 -maxdepth 1 ! -name pkgconfig -exec rm -rf {} + && \
    rm -rf "/opt/sysroots/$TARGET_TRIPLE/tmp" \
    "/opt/sysroots/$TARGET_TRIPLE/var" \
    "/opt/sysroots/$TARGET_TRIPLE/usr/sbom" \
    "/opt/sysroots/$TARGET_TRIPLE/usr/doc" \
    "/opt/sysroots/$TARGET_TRIPLE/usr/metatypes" && \
    if [ "$TARGET_TRIPLE" != $(gcc -dumpmachine) ]; then \
        rm -rf "/opt/sysroots/$TARGET_TRIPLE/bin" \
        "/opt/sysroots/$TARGET_TRIPLE/sbin" \
        "/opt/sysroots/$TARGET_TRIPLE/usr/bin" \
        "/opt/sysroots/$TARGET_TRIPLE/usr/sbin"; \
    fi


# Stage 3: the final stage of docker/build-environment plus the development extras
FROM ubuntu:$DEB_DISTRIBUTION
LABEL authors="klapeto"

ARG DEB_DISTRIBUTION
ARG HOST_TRIPLE=x86_64-linux-gnu
ARG TARGET_TRIPLE=x86_64-linux-gnu
ARG LLVM_VERSION=llvmorg-21.1.0

ENV DEBIAN_FRONTEND=noninteractive
ENV QT_HOST_PATH=/usr/local/Qt-6.10.0
ENV TARGET_TRIPLE=$TARGET_TRIPLE
ENV HOST_TRIPLE=$HOST_TRIPLE
ENV DEB_DISTRIBUTION=$DEB_DISTRIBUTION
ENV LLVM_VERSION=$LLVM_VERSION
ENV PKG_CONFIG_SYSROOT_DIR=/opt/sysroots/$TARGET_TRIPLE
ENV PKG_CONFIG_PATH=/opt/sysroots/$TARGET_TRIPLE/usr/lib/$TARGET_TRIPLE/pkgconfig/:/opt/sysroots/$TARGET_TRIPLE/usr/lib/pkgconfig/:/opt/sysroots/$TARGET_TRIPLE/usr/share/pkgconfig/

# The CI image's packages (docker/build-environment/Dockerfile), then the development extras: gdb, and the libraries
# elpida-qt (x86_64-linux-gnu) links dynamically (libnuma, libdrm, udev, mtdev, brotli, X11/xcb/xkbcommon, SM/ICE,
# GLX, freetype, png, fontconfig) with a font, so it runs in the container on the host's X server.
RUN apt-get update && \
    apt-get install -y --no-install-recommends \
        ca-certificates \
        git \
        make \
        perl \
        pkg-config \
        patch \
        xz-utils \
        zip \
        unzip \
        zstd \
        libgl1 \
        libegl1 \
        libopengl0 \
        libpcre2-16-0 \
        gdb \
        libnuma1 \
        libdrm2 \
        libudev1 \
        libmtdev1 \
        libbrotli1 \
        libx11-6 \
        libx11-xcb1 \
        libxcb1 \
        libxcb-glx0 \
        libxcb-icccm4 \
        libxcb-util1 \
        libxcb-image0 \
        libxcb-keysyms1 \
        libxcb-randr0 \
        libxcb-render0 \
        libxcb-render-util0 \
        libxcb-shape0 \
        libxcb-shm0 \
        libxcb-sync1 \
        libxcb-xfixes0 \
        libxcb-xkb1 \
        libxkbcommon0 \
        libxkbcommon-x11-0 \
        libsm6 \
        libice6 \
        libglx0 \
        libfreetype6 \
        libpng16-16 \
        libfontconfig1 \
        fonts-dejavu-core && \
    rm -rf /var/lib/apt/lists/*

COPY --from=build /opt/cmake-4.2.3-linux-x86_64 /opt/cmake-4.2.3-linux-x86_64
COPY --from=build /opt/ninja /opt/ninja
COPY --from=build /opt/toolchains /opt/toolchains
COPY --from=build /usr/local/Qt-6.10.0 /usr/local/Qt-6.10.0
RUN ln -s /opt/toolchains/llvm/ /opt/sysroots

ENV PATH=/opt/toolchains/llvm/bin:/opt/cmake-4.2.3-linux-x86_64/bin:/opt/ninja:$PATH

# Fail the image build if a host binary misses a shared library
RUN missing=$(for f in /opt/toolchains/llvm/bin/* /usr/local/Qt-6.10.0/bin/* /usr/local/Qt-6.10.0/libexec/* \
                      /usr/local/Qt-6.10.0/lib/*.so*; do \
                  [ -f "$f" ] && ldd "$f" 2>/dev/null | grep "not found" | sed "s|^|$f: |"; \
              done | sort -u); \
    if [ -n "$missing" ]; then echo "$missing"; exit 1; fi

WORKDIR /

CMD ["/bin/bash"]

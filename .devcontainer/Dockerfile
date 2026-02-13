FROM mcr.microsoft.com/devcontainers/cpp:1-ubuntu-24.04
LABEL authors="klapeto"

ARG DEB_DISTRIBUTION=noble
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

ARG TARGET_TRIPLE=x86_64-linux-gnu
ENV QT_HOST_PATH=/usr/local/Qt-6.10.0
ENV TARGET_TRIPLE=$TARGET_TRIPLE
ENV HOST_TRIPLE=$HOST_TRIPLE
ENV DEB_DISTRIBUTION=$DEB_DISTRIBUTION
ENV LLVM_VERSION=$LLVM_VERSION
ENV PKG_CONFIG_SYSROOT_DIR=/opt/sysroots/$TARGET_TRIPLE
ENV PKG_CONFIG_PATH=/opt/sysroots/$TARGET_TRIPLE/usr/lib/$TARGET_TRIPLE/pkgconfig/

WORKDIR /tmp/llvm-mingw
RUN TARGET_TRIPLES="$TARGET_TRIPLE" ./build-all.sh /opt/toolchains/llvm --with-default-msvcrt=msvcrt

WORKDIR /tmp/
ADD ./scripts/docker/packages ./scripts/docker/generate-sysroot.sh ./
RUN ./generate-sysroot.sh "$TARGET_TRIPLE" "/opt/sysroots/$TARGET_TRIPLE" /tmp/packages

ADD ./scripts/docker/build-base.sh ./scripts/docker/build-autotools-based.sh ./
RUN ./build-autotools-based.sh "$TARGET_TRIPLE" "/opt/sysroots/$TARGET_TRIPLE" "$PWD/hwloc" "--enable-plugins=no --disable-readme --disable-cairo --disable-libxml2 --disable-io --disable-pci --disable-opencl --disable-cuda --disable-nvml --disable-rsmi --disable-levelzero--disable-gl --disable-libudev"

ADD ./scripts/docker/build-qt.sh ./scripts/docker/generate-toolchain-file.sh ./scripts/docker/cross.linux.cmake.template ./
RUN ./generate-toolchain-file.sh "$TARGET_TRIPLE" "/opt/sysroots/$TARGET_TRIPLE"
RUN ./build-qt.sh "$TARGET_TRIPLE" "/opt/sysroots/$TARGET_TRIPLE"

ADD ./scripts/docker/build-llvm.sh ./
RUN ./build-llvm.sh "$TARGET_TRIPLE" "/opt/sysroots/$TARGET_TRIPLE"

WORKDIR /

# cleanup
RUN apt-get -y purge g++ gcc libc6-dev automake autoconf libtool-bin xutils-dev
RUN apt-get -y autoremove && apt-get autoclean

RUN apt-get purge -y `xargs < /tmp/packages`
RUN apt-get autoremove -y

RUN rm -rf /tmp \
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

CMD ["/bin/bash"]
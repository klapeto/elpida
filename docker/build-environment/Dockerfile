ARG IMAGE=registry.gitlab.com/dev-hood/elpida/elpida/build-environment-base:latest
FROM $IMAGE
LABEL authors="klapeto"

ARG TARGET_TRIPLE
ARG SYSROOT_SOURCE
ARG SYSROOT_ARCH

ENV DEBIAN_FRONTEND=noninteractive

WORKDIR /opt/sysroots
RUN ./generate-sysroot.sh "$SYSROOT_ARCH" "$SYSROOT_SOURCE" "$TARGET_TRIPLE"

# build LLVM
WORKDIR /opt/sources
RUN mkdir /opt/sources/llvm-project/llvm/build
WORKDIR /opt/sources/llvm-project/llvm/build
ADD ./scripts/docker/build-llvm.sh ../build-llvm.sh
ADD ./scripts/docker/build-base.sh ../build-base.sh
RUN TARGET_TRIPLE=$TARGET_TRIPLE ../build-llvm.sh

# Build Qt
WORKDIR /opt/sources
RUN mkdir -p qt6/build
WORKDIR /opt/sources/qt6/build
ADD ./scripts/docker/build-qt.sh ../build-qt.sh
ADD ./scripts/docker/build-base.sh ../build-base.sh
RUN TARGET_TRIPLE=$TARGET_TRIPLE ../build-qt.sh;

# Build HWLOC
WORKDIR /opt/sources
RUN mkdir -p hwloc/build
WORKDIR /opt/sources/hwloc/build
ADD ./scripts/docker/build-hwloc.sh ../build-hwloc.sh
ADD ./scripts/docker/build-base.sh ../build-base.sh
RUN TARGET_TRIPLE=$TARGET_TRIPLE ../build-hwloc.sh

# Cleanup
RUN rm -rf /opt/sources

# cleanup
RUN apt-get -y autoremove && apt-get autoclean

WORKDIR /opt/sysroots
RUN rm -rf ./SysrootGenerator \
    ./packages \
    ./link-sysroot.sh \
    ./*/tmp \
    ./*/var \
    ./*/etc \
    ./*/usr/share \
    ./*/usr/sbom \
    ./*/usr/doc \
    ./*/usr/metatypes \
    ./[ar]*-linux-gnu*/bin \
    ./[ar]*-linux-gnu*/sbin \
    ./[ar]*-linux-gnu*/usr/bin \
    ./[ar]*-linux-gnu*/usr/sbin \
    ./*-w64-mingw32/bin \
    ./*-w64-mingw32/usr/bin \
    /opt/toolchains/llvm-mingw/arm64ec-w64-mingw32 \
    /opt/toolchains/llvm-mingw/armv7-w64-mingw32 \
    /opt/toolchains/llvm-mingw/aarch64-w64-mingw32/bin \
    /opt/toolchains/llvm-mingw/i686-w64-mingw32/bin \
    /opt/toolchains/llvm-mingw/x86_64-w64-mingw32/bin

WORKDIR /

CMD ["/bin/bash"]
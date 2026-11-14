ARG IMAGE=build-base
ARG DEB_DISTRIBUTION=focal

# Stage 1: builds the target's runtimes, sysroot, hwloc, LLVM libraries and Qt on top of the base image (host compiler,
# host Qt and the sources). Its sources and build trees are not part of the final image.
FROM $IMAGE AS build

ARG DEB_DISTRIBUTION
ARG HOST_TRIPLE=x86_64-linux-gnu
ARG TARGET_TRIPLE
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
# The base image built the host compiler. llvm.xp.toolchain.v3.patch only changes the runtimes and Windows-only code
# in llvm/lib/Support, so the (Linux) compiler isn't rebuilt: what build-all.sh does without --no-tools, minus
# build-llvm.sh and build-lldb-mi.sh. The runtimes get linked into the benchmarks, so they get the parity flags.
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

# What the final image doesn't need from the sysroot. usr/share/pkgconfig stays: it has the .pc files of the X11
# protocol headers (xproto.pc, which xau.pc requires) that the Qt xcb plugins look up through pkg-config.
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


# Stage 2: the image the CI build jobs run in: the toolchain (with the target sysroot), the host Qt, CMake and Ninja on
# a plain Ubuntu, without stage 1's sources and build trees (~40 GB -> a few GB).
FROM ubuntu:$DEB_DISTRIBUTION
LABEL authors="klapeto"

ARG DEB_DISTRIBUTION
ARG HOST_TRIPLE=x86_64-linux-gnu
ARG TARGET_TRIPLE
ARG LLVM_VERSION=llvmorg-21.1.0

ENV DEBIAN_FRONTEND=noninteractive
ENV QT_HOST_PATH=/usr/local/Qt-6.10.0
ENV TARGET_TRIPLE=$TARGET_TRIPLE
ENV HOST_TRIPLE=$HOST_TRIPLE
ENV DEB_DISTRIBUTION=$DEB_DISTRIBUTION
ENV LLVM_VERSION=$LLVM_VERSION
ENV PKG_CONFIG_SYSROOT_DIR=/opt/sysroots/$TARGET_TRIPLE
# The target's .pc files only: unlike stage 1, this image has no host -dev packages whose .pc files pkg-config could
# fall back to
ENV PKG_CONFIG_PATH=/opt/sysroots/$TARGET_TRIPLE/usr/lib/$TARGET_TRIPLE/pkgconfig/:/opt/sysroots/$TARGET_TRIPLE/usr/lib/pkgconfig/:/opt/sysroots/$TARGET_TRIPLE/usr/share/pkgconfig/

# What the CI build (ci/build.yml) runs besides the toolchain: git (submodules), make/sh/perl (OpenSSL's Configure and
# build), ca-certificates (OpenSSL download), pkg-config; libgl1/libegl1/libopengl0/libpcre2-16-0 for the host Qt
# libraries.
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
        libpcre2-16-0 && \
    rm -rf /var/lib/apt/lists/*

COPY --from=build /opt/cmake-4.2.3-linux-x86_64 /opt/cmake-4.2.3-linux-x86_64
COPY --from=build /opt/ninja /opt/ninja
COPY --from=build /opt/toolchains /opt/toolchains
COPY --from=build /usr/local/Qt-6.10.0 /usr/local/Qt-6.10.0
RUN ln -s /opt/toolchains/llvm/ /opt/sysroots

ENV PATH=/opt/toolchains/llvm/bin:/opt/cmake-4.2.3-linux-x86_64/bin:/opt/ninja:$PATH

# Fail the image build if a host binary misses a shared library that stage 2 doesn't have
RUN missing=$(for f in /opt/toolchains/llvm/bin/* /usr/local/Qt-6.10.0/bin/* /usr/local/Qt-6.10.0/libexec/* \
                      /usr/local/Qt-6.10.0/lib/*.so*; do \
                  [ -f "$f" ] && ldd "$f" 2>/dev/null | grep "not found" | sed "s|^|$f: |"; \
              done | sort -u); \
    if [ -n "$missing" ]; then echo "$missing"; exit 1; fi

WORKDIR /

CMD ["/bin/bash"]

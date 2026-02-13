ARG IMAGE=build-base
FROM $IMAGE

ARG DEB_DISTRIBUTION=noble
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
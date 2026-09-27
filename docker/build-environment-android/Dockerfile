FROM mobiledevops/android-sdk-image:36.1.0
LABEL authors="klapeto"

ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get install -y \
    dotnet-sdk-10.0  \
    wget \
    pkg-config \
    automake \
    autoconf \
    libtool-bin \
    patch \
    python3.8 \
    xz-utils \
    zip \
    binutils \
    make \
    cmake \
    ninja-build

RUN wget https://dl.google.com/android/repository/android-ndk-r30-linux.zip
RUN unzip -d /opt/ndk ./android-ndk-r30-linux.zip
RUN rm ./android-ndk-r30-linux.zip

ENV PATH=/opt/ndk/android-ndk-r30/toolchains/llvm/prebuilt/linux-x86_64/bin:$PATH
ENV NDK_DIR=/opt/ndk/android-ndk-r30
ENV NDK_SYSROOT=$NDK_DIR/toolchains/llvm/prebuilt/linux-x86_64/sysroot

ADD ./scripts/docker/build-base.sh ./scripts/docker/build-autotools-based.sh ./
RUN git clone --depth=1 -b hwloc-2.4.0 https://github.com/open-mpi/hwloc.git && \
    cd hwloc && \
    ./autogen.sh && \
    mkdir -p build
RUN ./build-autotools-based.sh "aarch64-linux-android21" "$NDK_SYSROOT" "$PWD/hwloc" "--enable-plugins=no --disable-readme --disable-cairo --disable-libxml2 --disable-io --disable-pci --disable-opencl --disable-cuda --disable-nvml --disable-rsmi --disable-levelzero--disable-gl --disable-libudev"
RUN mv "$NDK_SYSROOT/usr/lib/libhwloc.a" "$NDK_SYSROOT/usr/lib/libhwloc.la" "$NDK_SYSROOT/usr/lib/aarch64-linux-android"
RUN ./build-autotools-based.sh "x86_64-linux-android21" "$NDK_SYSROOT" "$PWD/hwloc" "--enable-plugins=no --disable-readme --disable-cairo --disable-libxml2 --disable-io --disable-pci --disable-opencl --disable-cuda --disable-nvml --disable-rsmi --disable-levelzero--disable-gl --disable-libudev"
RUN mv "$NDK_SYSROOT/usr/lib/libhwloc.a" "$NDK_SYSROOT/usr/lib/libhwloc.la" "$NDK_SYSROOT/usr/lib/x86_64-linux-android"

RUN git clone --depth=1 -b release/19.x https://github.com/llvm/llvm-project.git && \
    mkdir -p ./llvm-project/llvm/build
ADD ./scripts/docker/build-llvm.sh ./
RUN ./build-llvm.sh "aarch64-linux-android21" "$NDK_SYSROOT"
RUN find "$NDK_SYSROOT/usr/lib/" -maxdepth 1 -name "lib*" -exec mv '{}' "$NDK_SYSROOT/usr/lib/aarch64-linux-android" \;
RUN ./build-llvm.sh "x86_64-linux-android21" "$NDK_SYSROOT"
RUN find "$NDK_SYSROOT/usr/lib/" -maxdepth 1 -name "lib*" -exec mv '{}' "$NDK_SYSROOT/usr/lib/x86_64-linux-android" \;

RUN dotnet workload install android
RUN dotnet workload install maui-android

RUN rm -rf ./hwloc ./llvm-project build-autotools-based.sh  build-base.sh  build-llvm.sh

CMD ["/bin/bash"]
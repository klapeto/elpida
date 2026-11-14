# Android build environment (see docs/development/android-toolchain.md).
#
# Stage 1 builds the Android targets of the llvm-mingw toolchain on top of the base image (which already has the host
# clang, the llvm-mingw sources and the hwloc/LLVM checkouts): the runtimes for API 29 (benchmarks) and API 21 (app part),
# hwloc for every sysroot and the LLVM 19.1.7 libraries of the Compilation benchmark for API 29.
# Stage 2 is the Android SDK + .NET image the app is built with; it gets the finished toolchain.

ARG IMAGE=build-base
FROM $IMAGE AS toolchain

# Only the NDK's bionic sysroot is used; its clang and libc++ are not. llvm-mingw's build-android-sysroots.sh downloads
# the NDK and extracts only the sysroot (r29 is the version the sysroots were tested with).
ARG ANDROID_NDK_VERSION=r29
ARG ANDROID_TRIPLES="x86_64-linux-android aarch64-linux-android armv7a-linux-androideabi"
ARG ANDROID_BENCHMARKS_API=29
ARG ANDROID_APP_API=21

ENV DEBIAN_FRONTEND=noninteractive
ENV TOOLCHAIN=/opt/toolchains/llvm

RUN apt-get update && apt-get install -y unzip

# Shared wrappers (clang-target-wrapper.sh, linux-common.cfg, ...); the base image only built the compiler.
# TARGET_TRIPLES must name a target that has a .cfg in llvm-mingw's wrappers/ directory.
WORKDIR /tmp/llvm-mingw
RUN TARGET_TRIPLES=x86_64-linux-gnu ./install-wrappers.sh $TOOLCHAIN

# Toolchain targets <triple><api>-clang with their sysroots (bionic from the NDK), then compiler-rt and libc++ with the
# benchmark code parity flags (docs/development/code-parity.md)
ADD ./scripts/docker/parity-flags.sh /tmp/
RUN triples=""; \
    for api in $ANDROID_BENCHMARKS_API $ANDROID_APP_API; do \
        for triple in $ANDROID_TRIPLES; do triples="$triples $triple$api"; done; \
    done; \
    export TARGET_TRIPLES="$triples"; \
    ANDROID_NDK_VERSION=$ANDROID_NDK_VERSION ./build-android-sysroots.sh $TOOLCHAIN && \
    ./install-wrappers.sh $TOOLCHAIN && \
    ./build-compiler-rt.sh $TOOLCHAIN --additional-cflags-script=/tmp/parity-flags.sh && \
    ./build-libcxx.sh $TOOLCHAIN --additional-cflags-script=/tmp/parity-flags.sh

WORKDIR /tmp
ADD ./scripts/docker/build-base.sh ./scripts/docker/build-autotools-based.sh ./

# hwloc: elpida-core links it, so every sysroot needs it
RUN for api in $ANDROID_BENCHMARKS_API $ANDROID_APP_API; do \
        for triple in $ANDROID_TRIPLES; do \
            ./build-autotools-based.sh "$triple$api" "$TOOLCHAIN/$triple$api" "$PWD/hwloc" \
                "--enable-plugins=no --disable-readme --disable-cairo --disable-libxml2 --disable-io --disable-pci --disable-opencl --disable-cuda --disable-nvml --disable-rsmi --disable-levelzero --disable-gl --disable-libudev" \
                || exit 1; \
        done; \
    done

# LLVM 19.1.7 libraries for the Compilation benchmark, only needed by the benchmarks (API 29) part.
# build-llvm.sh uses the base image's ./llvm-project checkout and applies llvm.parity.patch.
ADD ./scripts/docker/build-llvm.sh ./scripts/docker/llvm.parity.patch ./
RUN test "$(git -C llvm-project describe --tags)" = "llvmorg-19.1.7" || \
    (echo "llvm-project must be at llvmorg-19.1.7 (see docker/build-environment-base)" && exit 1)
RUN for triple in $ANDROID_TRIPLES; do \
        ./build-llvm.sh "$triple$ANDROID_BENCHMARKS_API" "$TOOLCHAIN/$triple$ANDROID_BENCHMARKS_API" || exit 1; \
    done


FROM mobiledevops/android-sdk-image:36.1.0
LABEL authors="klapeto"

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    dotnet-sdk-10.0  \
    wget \
    unzip \
    pkg-config \
    automake \
    autoconf \
    libtool-bin \
    patch \
    python3.8 \
    xz-utils \
    zip \
    binutils \
    make

# Same CMake and Ninja as the other build environments (the root CMakeLists.txt needs CMake >= 3.29 for
# CMAKE_LINKER_TYPE)
COPY --from=toolchain /opt/cmake-4.2.3-linux-x86_64 /opt/cmake-4.2.3-linux-x86_64
COPY --from=toolchain /opt/ninja /opt/ninja
ENV PATH=/opt/cmake-4.2.3-linux-x86_64/bin:/opt/ninja:$PATH

# The llvm-mingw toolchain with the Android targets. Elpida.Mobile.csproj finds it through ELPIDA_TOOLCHAIN.
COPY --from=toolchain /opt/toolchains/llvm /opt/toolchains/llvm
ENV ELPIDA_TOOLCHAIN=/opt/toolchains/llvm

# No NDK: Elpida's native code uses the toolchain above, and .NET for Android's Mono AOT (Release) brings its own tools.

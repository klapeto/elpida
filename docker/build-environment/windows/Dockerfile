FROM ubuntu:22.04
LABEL maintainer="Ioannis Panagiotopoulos yiannispana@hotmail.com"

RUN export DEBIAN_FRONTEND=noninteractive && \
    apt-get -y update && \
    apt-get -y full-upgrade && \
    apt-get -y install \
    autoconf \
    automake \
    autopoint \
    bash \
    bison \
    bzip2 \
    flex \
    g++ \
    g++-multilib \
    gettext \
    git \
    gperf \
    intltool \
    libc6-dev-i386 \
    libgdk-pixbuf2.0-dev \
    libltdl-dev \
    libgl-dev \
    libpcre3-dev \
    libssl-dev \
    libtool-bin \
    libxml-parser-perl \
    lzip \
    make \
    openssl \
    p7zip-full \
    patch \
    perl \
    python3 \
    python3-distutils \
    python3-mako \
    python3-pkg-resources \
    python3-packaging \
    python-is-python3 \
    ruby \
    sed \
    unzip \
    wget \
    xz-utils \
    lsb-release \
    software-properties-common \
    gnupg \
    cmake

RUN git clone -b elpida_updates https://github.com/klapeto/mxe.git
WORKDIR mxe
RUN make -j$(nproc) MXE_TARGETS='x86_64-w64-mingw32.static' cmake hwloc qtbase qtsvg qtcharts gcc
RUN make -j$(nproc) MXE_TARGETS='i686-w64-mingw32.static' cmake hwloc gcc

ENV PATH=/mxe/usr/bin:$PATH

# install llvm both archs
# first build the tlbgens
RUN git clone --depth=1 -b release/19.x https://github.com/llvm/llvm-project.git
RUN mkdir /mxe/llvm-project/llvm/build &&  \
    cd /mxe/llvm-project/llvm/build &&  \
    cmake .. -DCMAKE_BUILD_TYPE=Release -DLLVM_ENABLE_PROJECTS=clang && \
    make -j$(nproc) llvm-tblgen llvm-config clang-tblgen && \
    cp ./bin/* /mxe/usr/x86_64-pc-linux-gnu/bin

# build x86_64
RUN cd /mxe/llvm-project/llvm/build && rm -rf * && x86_64-w64-mingw32.static-cmake -S .. \
    -DCMAKE_BUILD_TYPE=Release \
    -DLLVM_TABLEGEN=/mxe/usr/x86_64-pc-linux-gnu/bin/llvm-tblgen \
    -DCMAKE_SKIP_INSTALL_RPATH=ON \
    -DBUILD_SHARED_LIBS=OFF \
    -DLLVM_STATIC_LINK_CXX_STDLIB=ON \
    -DLLVM_TARGETS_TO_BUILD=X86 \
    -DLLVM_BUILD_TOOLS=ON \
    -DLLVM_ENABLE_PROJECTS=clang \
    -DLLVM_BUILD_STATIC=ON \
    -DLLVM_ENABLE_ZLIB=OFF \
    -DLLVM_ENABLE_ZSTD=OFF \
    -DLLVM_INCLUDE_BENCHMARKS=OFF \
    -DLLVM_INCLUDE_EXAMPLES=OFF \
    -DLLVM_INCLUDE_TESTS=OFF \
    -DLIBCLANG_BUILD_STATIC=ON && \
    make -j$(nproc) && make install

# build i686
RUN cd /mxe/llvm-project/llvm/build && rm -rf * && i686-w64-mingw32.static-cmake -S .. \
    -DCMAKE_BUILD_TYPE=Release \
    -DLLVM_TABLEGEN=/mxe/usr/x86_64-pc-linux-gnu/bin/llvm-tblgen \
    -DCMAKE_SKIP_INSTALL_RPATH=ON \
    -DBUILD_SHARED_LIBS=OFF \
    -DLLVM_STATIC_LINK_CXX_STDLIB=ON \
    -DLLVM_TARGETS_TO_BUILD=X86 \
    -DLLVM_BUILD_TOOLS=ON \
    -DLLVM_ENABLE_PROJECTS=clang \
    -DLLVM_BUILD_STATIC=ON \
    -DLLVM_ENABLE_ZLIB=OFF \
    -DLLVM_ENABLE_ZSTD=OFF \
    -DLLVM_INCLUDE_BENCHMARKS=OFF \
    -DLLVM_INCLUDE_EXAMPLES=OFF \
    -DLLVM_INCLUDE_TESTS=OFF \
    -DLIBCLANG_BUILD_STATIC=ON && \
    make -j$(nproc) && make install

# reduce size of the image
RUN rm -rf /mxe/pkg /mxe/src /mxe/log /mxe/llvm-project
RUN apt-get autoclean

WORKDIR /

CMD ["/bin/bash"]

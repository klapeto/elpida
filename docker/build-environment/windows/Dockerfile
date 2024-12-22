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
RUN make -j$(nproc) MXE_TARGETS='i686-w64-mingw32.static' MXE_PLUGIN_DIRS=plugins/windows-xp cmake hwloc gcc

ENV PATH=/mxe/usr/bin:$PATH

# install llvm both archs
# first build the tlbgens
RUN git clone --depth=1 -b release/19.x https://github.com/llvm/llvm-project.git

RUN mkdir /mxe/llvm-project/llvm/build
WORKDIR /mxe/llvm-project/llvm/build

RUN cmake .. -DCMAKE_BUILD_TYPE=Release -DLLVM_ENABLE_PROJECTS=clang && \
    make -j$(nproc) llvm-tblgen llvm-config clang-tblgen && \
    cp ./bin/* /mxe/usr/x86_64-pc-linux-gnu/bin

# build x86_64
RUN rm -rf * && x86_64-w64-mingw32.static-cmake -S .. \
    -DCMAKE_BUILD_TYPE=Release \
    -DLLVM_TABLEGEN=/mxe/usr/x86_64-pc-linux-gnu/bin/llvm-tblgen \
    -DLLVM_ENABLE_PROJECTS=clang \
    -DCMAKE_SKIP_INSTALL_RPATH=ON \
    -DBUILD_SHARED_LIBS=OFF \
    -DLLVM_STATIC_LINK_CXX_STDLIB=ON \
    -DLIBCLANG_BUILD_STATIC=ON \
    -DLLVM_TARGETS_TO_BUILD=X86 \
    -DLLVM_ENABLE_ZSTD=OFF \
    -DLLVM_BUILD_DOCS=OFF \
    -DLLVM_BUILD_EXAMPLES=OFF \
    -DLLVM_BUILD_RUNTIME=OFF \
    -DLLVM_BUILD_RUNTIMES=OFF \
    -DLLVM_BUILD_TESTS=OFF \
    -DLLVM_BUILD_TOOLS=OFF \
    -DLLVM_BUILD_UTILS=OFF \
    -DLLVM_ENABLE_BINDINGS=OFF \
    -DLLVM_ENABLE_DOXYGEN=OFF \
    -DLLVM_ENABLE_OCAMLDOC=OFF \
    -DLLVM_ENABLE_SPHINX=OFF \
    -DLLVM_INCLUDE_BENCHMARKS=OFF \
    -DLLVM_INCLUDE_EXAMPLES=OFF \
    -DLLVM_INCLUDE_TESTS=OFF \
    -DLLVM_INCLUDE_DOCS=OFF \
    -DLLVM_INCLUDE_GO_TESTS=OFF \
    -DLLVM_INCLUDE_RUNTIMES=OFF \
    -DLLVM_INCLUDE_TOOLS=ON \
    -DLLVM_INCLUDE_UTILS=OFF

RUN make -j$(nproc)
RUN make install

# apply patch to be able to build on windows XP. FileWatcher/Delete will not work but it is ok
COPY llvm.WindowsXp.patch llvm.WindowsXp.patch
RUN cd .. && patch -p1 -f llvm.WindowsXp.patch
# build i686
RUN rm -rf * && i686-w64-mingw32.static-cmake -S .. \
    -DCMAKE_BUILD_TYPE=Release \
    -DLLVM_TABLEGEN=/mxe/usr/x86_64-pc-linux-gnu/bin/llvm-tblgen \
    -DLLVM_ENABLE_PROJECTS=clang \
    -DCMAKE_SKIP_INSTALL_RPATH=ON \
    -DBUILD_SHARED_LIBS=OFF \
    -DLLVM_STATIC_LINK_CXX_STDLIB=ON \
    -DLIBCLANG_BUILD_STATIC=ON \
    -DLLVM_TARGETS_TO_BUILD=X86 \
    -DLLVM_ENABLE_ZSTD=OFF \
    -DLLVM_BUILD_DOCS=OFF \
    -DLLVM_BUILD_EXAMPLES=OFF \
    -DLLVM_BUILD_RUNTIME=OFF \
    -DLLVM_BUILD_RUNTIMES=OFF \
    -DLLVM_BUILD_TESTS=OFF \
    -DLLVM_BUILD_TOOLS=OFF \
    -DLLVM_BUILD_UTILS=OFF \
    -DLLVM_ENABLE_BINDINGS=OFF \
    -DLLVM_ENABLE_DOXYGEN=OFF \
    -DLLVM_ENABLE_OCAMLDOC=OFF \
    -DLLVM_ENABLE_SPHINX=OFF \
    -DLLVM_INCLUDE_BENCHMARKS=OFF \
    -DLLVM_INCLUDE_EXAMPLES=OFF \
    -DLLVM_INCLUDE_TESTS=OFF \
    -DLLVM_INCLUDE_DOCS=OFF \
    -DLLVM_INCLUDE_GO_TESTS=OFF \
    -DLLVM_INCLUDE_RUNTIMES=OFF \
    -DLLVM_INCLUDE_TOOLS=ON \
    -DLLVM_INCLUDE_UTILS=OFF

RUN make -j$(nproc)
RUN make install

# reduce size of the image
RUN rm -rf /mxe/pkg /mxe/src /mxe/log /mxe/llvm-project llvm.WindowsXp.patch
RUN apt-get autoclean

WORKDIR /

CMD ["/bin/bash"]

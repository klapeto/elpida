FROM ubuntu:22.04
LABEL maintainer="Ioannis Panagiotopoulos yiannispana@hotmail.com"

# This part is copied and modified from https://github.com/AppImageCrafters/appimage-builder/blob/0b2f1910b2e99df2fdbb50e12b37d97246a6fa9b/Dockerfile
ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get -y full-upgrade
RUN apt-get install -y \
    ca-certificates \
    gpg \
    wget \
    python3 \
    python-is-python3 \
    python3-pip \
    zsync \
    gtk-update-icon-cache \
    squashfs-tools \
    libglib2.0-bin \
    git \
    make \
    patch \
    pkg-config \
    automake \
    autoconf \
    libtool-bin \
    g++ \
    gcc

# install latest cmake
RUN test -f /usr/share/doc/kitware-archive-keyring/copyright || wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | gpg --dearmor - | tee /usr/share/keyrings/kitware-archive-keyring.gpg >/dev/null
RUN echo 'deb [signed-by=/usr/share/keyrings/kitware-archive-keyring.gpg] https://apt.kitware.com/ubuntu/ jammy main' | tee /etc/apt/sources.list.d/kitware.list >/dev/null
RUN apt-get update
RUN test -f /usr/share/doc/kitware-archive-keyring/copyright || rm /usr/share/keyrings/kitware-archive-keyring.gpg
RUN apt-get install -y kitware-archive-keyring cmake

# Add ARM repos
RUN echo "deb [arch=armhf,arm64] http://ports.ubuntu.com/ jammy main restricted" > /etc/apt/sources.list.d/arm-cross-compile-sources.list && \
    echo "deb [arch=armhf,arm64] http://ports.ubuntu.com/ jammy-updates main restricted" >> /etc/apt/sources.list.d/arm-cross-compile-sources.list && \
    echo "deb [arch=armhf,arm64] http://ports.ubuntu.com/ jammy universe" >> /etc/apt/sources.list.d/arm-cross-compile-sources.list  && \
    echo "deb [arch=armhf,arm64] http://ports.ubuntu.com/ jammy-updates universe" >> /etc/apt/sources.list.d/arm-cross-compile-sources.list && \
    echo "deb [arch=armhf,arm64] http://ports.ubuntu.com/ jammy multiverse" >> /etc/apt/sources.list.d/arm-cross-compile-sources.list && \
    echo "deb [arch=armhf,arm64] http://ports.ubuntu.com/ jammy-updates multiverse" >> /etc/apt/sources.list.d/arm-cross-compile-sources.list && \
    echo "deb [arch=armhf,arm64] http://ports.ubuntu.com/ jammy-backports main restricted universe multiverse" >> /etc/apt/sources.list.d/arm-cross-compile-sources.list && \
    echo "deb [arch=amd64] http://gr.archive.ubuntu.com/ubuntu/ jammy main restricted universe multiverse" >> /etc/apt/sources.list

# Update caches
RUN apt-get update --fix-missing

# Add new archs
RUN dpkg --add-architecture arm64 && dpkg --add-architecture armhf

# install cross libraries
RUN apt-get install -y \
    gcc-aarch64-linux-gnu \
    g++-aarch64-linux-gnu \
    libnuma-dev:arm64 \
    libnuma-dev:amd64 \
    qtbase5-dev:arm64 \
    qtbase5-dev:amd64 \
    libqt5svg5:arm64 \
    libqt5svg5:amd64 \
    libqt5x11extras5:arm64 \
    libqt5x11extras5:amd64 \
    libqt5charts5-dev:arm64 \
    libqt5charts5-dev:amd64 \
    qt5-image-formats-plugins:arm64 \
    qt5-image-formats-plugins:amd64

# install hwloc amd64
RUN git clone -b hwloc-2.4.0 https://github.com/open-mpi/hwloc.git
RUN cd hwloc && ./autogen.sh && ./configure  --enable-static --disable-cairo --disable-libxml2 --disable-io --disable-pci --disable-opencl --disable-cuda --disable-nvml --disable-rsmi --disable-gl --disable-libudev && \
    make -j$(nproc) && make install

# install hwloc aarch64
RUN cd hwloc && ./autogen.sh && ./configure --build=amd64-linux-gnu --host=aarch64-linux-gnu --prefix=/usr/aarch64-linux-gnu --with-sysroot=/usr/aarch64-linux-gnu --enable-static --disable-cairo --disable-libxml2 --disable-io --disable-pci --disable-opencl --disable-cuda --disable-nvml --disable-rsmi --disable-gl --disable-libudev && \
    make -j$(nproc) && make install
RUN rm -rf hwloc

# install patchelf
WORKDIR /tmp
RUN wget https://github.com/NixOS/patchelf/releases/download/0.12/patchelf-0.12.tar.bz2; \
    tar -xvf patchelf-0.12.tar.bz2;  \
    cd patchelf-0.12.20200827.8d3a16e; \
    ./configure && make -j$(nproc) && make install; \
    rm -rf patchelf-*

# install appimage-builder
WORKDIR /opt
RUN git clone https://github.com/AppImageCrafters/appimage-builder.git

# Force 0.14.x lief version because it crashes
RUN sed -i 's/"lief"/"lief<0.15.0"/' appimage-builder/setup.py

RUN python3 -m pip install /opt/appimage-builder
RUN rm -rf /opt/appimage-builder

# install llvm both archs
RUN git clone --depth=1 -b release/19.x https://github.com/llvm/llvm-project.git
RUN mkdir /opt/llvm-project/llvm/build
WORKDIR /opt/llvm-project/llvm/build
RUN cmake -S .. \
    -DCMAKE_BUILD_TYPE=Release \
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

RUN  rm -rf * && cmake -S .. \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_SYSTEM_NAME=Linux \
    -DCMAKE_CROSSCOMPILING=ON \
    -DCMAKE_SYSTEM_PROCESSOR=aarch64 \
    -DCMAKE_C_COMPILER=/usr/bin/aarch64-linux-gnu-gcc \
    -DCMAKE_CXX_COMPILER=/usr/bin/aarch64-linux-gnu-g++ \
    -DCMAKE_FIND_ROOT_PATH=/usr/aarch64-linux-gnu \
    -DCMAKE_INSTALL_PREFIX=/usr/aarch64-linux-gnu \
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

# reduce the size of the image
RUN apt-get autoclean
RUN rm -rf /opt/llvm-project

WORKDIR /

CMD ["/bin/bash"]
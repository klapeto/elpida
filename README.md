# Elpida
[![pipeline status](https://gitlab.com/dev-hood/elpida/elpida/badges/master/pipeline.svg)](https://gitlab.com/dev-hood/elpida/elpida/-/commits/master)

Elpida is a simple CPU/Algorithm benchmarking library, aimed to be as transparent and as accurate as can be.

## Contents

* [Dependencies](#dependencies)
* [Build for Windows](#build-for-windows)
* [Build for Linux](#build-for-linux)
* [License](#license)

## Dependencies

To be able to build elpida from sources, you will need a small build system consisting of:

* [Cmake](https://cmake.org/ "CMAKE Build system")
* [LLVM](https://gcc.gnu.org "LLVM/Clang") for C++ that supports c++17
* [Qt6](https://www.qt.io/ "Qt framework") For the GUI frontend (optional).

There are some other dependencies that are built together with Elpida.

## Build for Windows

Building Elpida on Windows natively is not supported yet and not recommended. 
If you want to build it anyway, you can use [WSL2](https://docs.microsoft.com/en-us/windows/wsl/install-win10) and follow the instructions for Linux.

## Build for Linux

Assuming you can use similar commands on your distro, you can follow these steps to build Elpida.
To keep your host system clean, you can go the way official builds are produced. This means that you will need to create 
a sysroot for the target you need (most likely x86_64). Keep in mind this will require a lot of disk space (about 1,9GiB
as of December 2025). Also, you will need 32GB of RAM to build llvm and as many cores as you have (to speed up the build).
1. Install required packages:
```bash
sudo apt install git make pkg-config automake autoconf libtool-bin patch python3 clang cmake debootstrap
```
2. First, clone the repo and update the submodules:
```bash
git clone --recurse-submodules https://gitlab.com/dev-hood/elpida.git
```
3. Set up the sysroot with the initial packages:
```bash
mkdir -p elpida/sysroot && sudo debootstrap --arch=amd64 --variant=buildd --components=main,universe --include=`paste -sd, elpida/scripts/docker/packages` noble elpida/sysroot/x86_64-linux-gnu
```
4. Clone and build LLVM:
```bash
extern ROOTDIR=$PWD && extern SYSROOTBASE=$PWD/elpida/sysroot && git clone --depth=1 -b release/19.x https://github.com/llvm/llvm-project.git && mkdir -p llvm-project/llvm/build && cd llvm-project/llvm/build && ../../../elpida/scripts/build-llvm.sh && cd $ROOTDIR
```
5. Clone and build hwloc:
```bash
git clone --depth=1 -b hwloc-2.4.0 https://github.com/open-mpi/hwloc.git && cd hwloc && ./autogen.sh && mkdir build && cd build && ../../../elpida/scripts/build-hwloc.sh && cd $ROOTDIR
```
6. (Optional if you want to build the QT GUI) Clone and build Qt:
```bash
git clone -b v6.10.0 --depth=1 git://code.qt.io/qt/qt5.git qt6 && cd qt6 && ./init-repository --module-subset=qtcharts,qtsvg,qtbase && mkdir build && cd build && ../../../elpida/scripts/build-qt.sh && cd $ROOTDIR
```
7. Now you have a setup sysroot. You can optionally remove the cloned repos we no longer need:
```bash
rm -rf $PWD/llvm-project && rm -rf $PWD/hwloc && rm -rf $PWD/qt6
```
8. Configure elpida
```bash
cd elpida && mkdir build && cd build && cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/llvm-cross.cmake -DCMAKE_SYSROOT=../sysroot/x86_64-linux-gnu ..
```
9. Build elpida
```bash
cmake --build . --parallel && cmake --install .
```

## License
Elpida Library, Qt Front end and Default Benchmarks are licensed under GPL v3. For more information see the LICENSE file.
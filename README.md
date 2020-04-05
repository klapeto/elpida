# Elpida

Elpida is a simple x86 CPU/Algorithm benchmarking library, aimed to be as transparent and as accurate as can be.
To fully test the capabilities of the CPU, the benchmark is offered to be compiled against the specific architecture that it currently runs (eg. -march=native). Elpida is under development.

## Contents

* [Dependencies](#dependecies)
* [Build for Windows 7+ (For simple users)](#build-for-windows-7-for-simple-users)
* [Build for Windows 7+ (For advanced users)](#build-for-windows-7-for-advanced-users)
* [Build for Linux (Debian Based, Fedora)](#build-for-linux-debian-based-fedora)

## Dependecies

To be able to build elpida from sources you will need a small build system consisted of:

* [Cmake](https://cmake.org/ "CMAKE Build system")
* [GCC](https://gcc.gnu.org "The GNU Compiler collection") for C++ that support c++11 (gcc 4.8+)
* [Qt5](https://www.qt.io/) For the GUI frontend.

On Windows, Debian, Ubuntu and Fedora these dependencies are covered by the build scripts, so you will not probably need to manually install them.

## Build for Windows 7+ (For simple users)

Currently on windows is only tested on with [MSYS2](https://www.msys2.org/ "MSYS2 is a software distro and building platform for Windows") building platform. To be able to build it, I provided some scripts that download and and creates the build environment required to build elpida. If you already have a MSYS2 installation on your system, you can go to [Build for Windows 7+ (For advanced users)](#build-for-windows-7-for-simple-users)

Requirements:

* Internet connection (the script will download 300+ MB of data)
* 2GB+ of hard disk space

The script will setup the environment in the folder you want. It will not install anything system-wise. When not needing anymore delete the folders it created.

To bootstrap the process do the following:

1. Copy all files/folders from [Scripts/MSYS2](Scripts/MSYS2) to where you want to setup the environment.
2. Run the `bootstrap.bat` script. It will take some time until it finishes.

Once it is finished, press any key to close the command line. From now on you will not have to run `bootstrap.bat` again.

* To run Elpida, run `runElpida.bat`
* To rebuild Elpida, run `rebuildElpida.bat`
* To update Elpida's sources from repository, run `updateElpida.bat` then `rebuildElpida.bat`

## Build for Windows 7+ (For advanced users)

This is for programmers or advanced users that have MSYS2 installed on the system. You will need to install these packages from the MSYS command line:

* If you are on 32bit MSYS

``` bash
pacman -S --needed git base-devel unzip mingw-w64-i686-toolchain mingw-w64-i686-python3 mingw-w64-i686-ninja mingw-w64-i686-meson
```

* If you are on 64bit MSYS

``` bash
pacman -S --needed git base-devel unzip mingw-w64-x86_64-toolchain mingw-w64-x86_64-python3 mingw-w64-x86_64-ninja mingw-w64-x86_64-meson
```

* Get Elpida sources:

``` bash
cd ~
git clone https://github.com/klapeto/elpida.git elpida
cd elpida
git checkout master
git pull
```

* Copy scripts on sources root folder

``` bash
cp Scripts/MSYS2/bash/*.sh .
```

* Get the libraries sources

``` bash
./downloadLibrariesSources.sh
```

* Build Elpida

``` bash
./rebuildElpida.sh
```

* Run Elpida

``` bash
./runElpida.sh
```

## Build for Linux (Debian Based, Fedora)

I provide some scripts for easier setup on Debian based and Fedora distributions.

* If you already have git installed:

``` bash
git clone https://github.com/klapeto/elpida.git elpida
```

* If you don't have git installed then download the sources from here and extract them where you want

* Copy the scripts from the [Scripts/Linux](Scripts/Linux) folder to the source root:

``` bash
cd elpida
cp Scripts/Linux/* .
chmod u+x ./*.sh
```

* Download the packages needed to build elpida:

``` bash
./downloadPackages.sh
```

* Download the sources of the libraries used (you will probably need administration rights):

``` bash
./downloadLibrariesSources.sh
```

* Build elpida:

``` bash
./rebuildElpida.sh
```

* Run elpida:

``` bash
install/bin/elpida-cli
```

or if you build the Qt frontend too

``` bash
install/bin/elpida-qt
```

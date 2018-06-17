# Elpida

Elpida is a simple x86 CPU/Algorithm benchmarking library, aimed to be as transparent and as accurate as can be.
To fully test the capabilities of the CPU, the benchmark is offered to be compiled the the specific architecture that it currently runs (eg. -march=native).


## Dependecies


*   [Meson](https://mesonbuild.com "The Meson Build system")
*   GCC that support c++11 (gcc 4.8+)


## Build for Windows 7+

Currently on windows is only tested on with MSYS2 building platform. To be able to build it, I provided some scripts that download and and creates the build environment required to build elpida.

Requirements:
*	Internet connection (script will download 300+ MB of data)
*	2GB+ of hard disk space

The script will setup the environment in the folder you want. It will not install anything system-wise. When not needing anymore delete the folders it created.

To bootstrap the process do the following:
1.	Copy all files/folders from [scripts/msys2](scripts/msys2) to where you want to setup the environment.
2.	Run the `bootstrap.bat` script. It will take some time until it finishes.

Once it is finished, press any key to close the command line. From now on you will not have to run bootstrap again.

*	To run Elpida, run `runElpida.bat`
*	To rebuild Elpida, run `rebuildElpida.bat`
*	To update Elpida's sources from repository, run `updateElpida.bat` then `rebuildElpida.bat`


## Build for Ubuntu 16.04


1.  install Meson

```
sudo apt install python3 ninja-build python3-pip
pip3 install --user meson
```

2.  Run the build script

```Shell
./build.sh
```
	
3.	Run the executable

```Shell
./builddir/elpida
```



# Elpida

Elpida is a x86 benchmarking tool, aimed to be transparent and as accurate as can be.
To fully test the capabilities of the CPU, the benchmark is offered to be compiled the the specific architecture that it currently runs (eg. -march=native).


## Dependecies


*   [Meson](mesonbuild.com "The Meson Build system")
*   GCC that support c++11 (gcc 4.8+)



## Build for Ubuntu 16.04


1.  install Meson

`sudo apt install python3 ninja-build python3-pip`
`pip3 install --user meson`

2.  Run the build script

`./build.sh`
	
3.	Run the executable

`./builddir/elpida`



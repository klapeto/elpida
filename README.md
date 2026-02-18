# Elpida
[![pipeline status](https://gitlab.com/dev-hood/elpida/elpida/badges/master/pipeline.svg)](https://gitlab.com/dev-hood/elpida/elpida/-/commits/master)

Elpida is a simple CPU/Algorithm benchmarking library, aimed to be as transparent and as accurate as can be.

## Contents

* [Building](#Building)
* [License](#license)

## Building
It is strongly recommended to use [Dev Containers](https://containers.dev/) to avoid messing with your host system and
will create a docker instance with all the dependencies required to build. We already provide a docker configuration to setup the container. 
Once you setup it with your IDE, you can build Elpida.

### IDEA IDES
We provide CMAKE/Run Configurations in the `./.idea` folder

### Other IDES
You need to configure the project via CMake and use the dev container sysroot (assuming your host machine is x86_64 architecture)
```bash
mkdir build
cd build
cmake .. -DQT_HOST_PATH=/usr/local/Qt-6.10.0 -DCMAKE_TOOLCHAIN_FILE=cmake-modules/llvm-cross.cmake -DCMAKE_SYSROOT=/opt/sysroots/x86_64-linux-gnu -DTARGET_PREFIX=x86_64-linux-gnu-
make -j$(nproc)
make install
```
The above will install the elpida on the `./install` folder of the source folder. To run it, you need to specify the `LD_LIBRARY_PATH` to use the sysroot like:
```bash
LD_LIBRARY_PATH=/opt/sysroots/x86_64-linux-gnu/usr/lib/x86_64-linux-gnu:/opt/sysroots/x86_64-linux-gnu/usr/lib:$LD_LIBRARY_PATH ./install/bin/elpida-qt
```

## License
Elpida Library, Qt Front end and Default Benchmarks are licensed under GPL v3. For more information see the LICENSE file.
#!/bin/bash

folders=("Qt" "CLI")

for fn in "${folders[@]}"; do
  mkdir cmake-build-debug/apps/$fn/Benchmarks
  ln -s "../InfoDumper/elpida-info-dumper" "cmake-build-debug/apps/$fn/elpida-info-dumper"
  ln -s "../../../src/Benchmarks/Compression/elpida-compression-benchmarks" "cmake-build-debug/apps/$fn/Compression/elpida-compression-benchmarks"
  ln -s "../../../src/Benchmarks/Encryption/elpida-encryption-benchmarks" "cmake-build-debug/apps/$fn/Encryption/elpida-encryption-benchmarks"
  ln -s "../../../src/Benchmarks/Image/elpida-image-benchmarks" "cmake-build-debug/apps/$fn/Benchmarks/elpida-image-benchmarks"
  ln -s "../../../src/Benchmarks/Math/elpida-math-benchmarks" "cmake-build-debug/apps/$fn/Benchmarks/elpida-math-benchmarks"
  ln -s "../../../src/Benchmarks/Memory/elpida-memory-benchmarks" "cmake-build-debug/apps/$fn/Benchmarks/elpida-memory-benchmarks"
  ln -s "../../../src/Benchmarks/StdLib/elpida-stdlib-benchmarks" "cmake-build-debug/apps/$fn/StdLib/elpida-stdlib-benchmarks"
  ln -s "../../../src/Benchmarks/Web/elpida-web-benchmarks" "cmake-build-debug/apps/$fn/Web/elpida-web-benchmarks"
done



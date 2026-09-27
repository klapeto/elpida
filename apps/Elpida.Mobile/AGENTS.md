# Mobile Android native build

- `ELPIDA_ENABLE_CLI` builds the standalone CLI interface. The mobile app does not use `elpida-cli`: keep this option `OFF` and do not copy or package that executable.
- The mobile native bridge needs `libElpida.so`, `elpida-info-dumper`, and the `*-benchmarks` executables for both Android ABIs. `ELPIDA_ENABLE_BENCHMARK_TOOLS` enables the dumper and benchmarks independently of the CLI; `ELPIDA_ENABLE_APPLICATION_LIBRARIES` enables the shared application code used by the bridge.
- When changing CMake options or packaging, verify a clean Android build and check the APK contents for both ABIs. Do not enable the CLI to work around missing benchmark tools.

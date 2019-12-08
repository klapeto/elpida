configure_file(Config.hpp.in Config.hpp)

add_library(Elpida CpuFeature.cpp)

target_include_directories(Elpida PUBLIC
        "${PROJECT_BINARY_DIR}"
        )
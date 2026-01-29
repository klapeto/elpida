project("Elpida Math")

add_library(elpida-math
        Matrix.cpp
        DynamicMatrix.cpp
)

target_link_libraries(elpida-math)

if (ELPIDA_INSTALL_CORE_LIBRARIES)
    install(TARGETS elpida-math ARCHIVE)
endif ()
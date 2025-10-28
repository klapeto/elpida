function(include_hwloc sourceDir)

    include(GNUInstallDirs)
    include(ExternalProject)

    set_property(DIRECTORY PROPERTY EP_UPDATE_DISCONNECTED true)

    set(HWLOC_LIB_DIR ${ELPIDA_LOCAL_INSTALL_DIR}/${CMAKE_INSTALL_LIBDIR})
    set(HWLOC_INCLUDE_DIR ${ELPIDA_LOCAL_INSTALL_DIR}/${CMAKE_INSTALL_INCLUDEDIR})
    set(HWLOC_LIBRARY ${HWLOC_LIB_DIR}/${CMAKE_STATIC_LIBRARY_PREFIX}hwloc${CMAKE_STATIC_LIBRARY_SUFFIX})

    find_program(MAKE_EXECUTABLE NAMES gmake make mingw32-make REQUIRED)
    find_program(PATCH_EXECUTABLE NAMES patch REQUIRED)

    # patch to avoid building stuff we do not need
    execute_process(COMMAND patch --directory=${sourceDir} -p1 --input=${CMAKE_CURRENT_FUNCTION_LIST_DIR}/hwloc.patch)

    if (CMAKE_CROSSCOMPILING OR ANDROID)
        set(HWLOC_CFLAGS ${CMAKE_C_FLAGS})
        set(HWLOC_LDFLAGS ${CMAKE_LINKER_FLAGS})
        set(HWLOC_TRIPLE ${CMAKE_C_COMPILER_TARGET})
        set(HWLOC_C_COMPILER "${CMAKE_C_COMPILER}")

        if (CMAKE_C_COMPILER_ID STREQUAL "Clang")
            set(HWLOC_C_COMPILER "${HWLOC_C_COMPILER} --target=${CMAKE_C_LIBRARY_ARCHITECTURE}")
        endif ()

        if (NOT CMAKE_C_COMPILER_SYSROOT STREQUAL "")
            set(HWLOC_C_COMPILER "${HWLOC_C_COMPILER} ${CMAKE_C_COMPILE_OPTIONS_SYSROOT}${CMAKE_C_COMPILER_SYSROOT}"})
        endif ()

        set(HWLOC_CXX_COMPILER "${CMAKE_CXX_COMPILER} --target=${CMAKE_CXX_LIBRARY_ARCHITECTURE}")

        if (NOT CMAKE_CXX_COMPILER_SYSROOT STREQUAL "")
            set(HWLOC_CXX_COMPILER "${HWLOC_CXX_COMPILER} ${CMAKE_CXX_COMPILE_OPTIONS_SYSROOT}${CMAKE_CXX_COMPILER_SYSROOT}"})
        endif ()

        if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
            set(HWLOC_CXX_COMPILER "${HWLOC_CXX_COMPILER} --target=${CMAKE_CXX_LIBRARY_ARCHITECTURE}")
        endif ()

        set(HWLOC_LINKER "${CMAKE_LINKER}")
        set(HWLOC_C_ADDITIONAL_FLAGS "CFLAGS=${CMAKE_C_FLAGS}")
        set(HWLOC_CXX_ADDITIONAL_FLAGS "CXXFLAGS=${CMAKE_CXX_FLAGS}")
        set(HWLOC_LDD_ADDITIONAL_FLAGS "LDFLAGS=${CMAKE_LINKER_FLAGS}")

        set(HWLOC_CONFIGURE_COMMAND
                CC=${HWLOC_C_COMPILER}
                CXX=${HWLOC_CXX_COMPILER}
                AR=${CMAKE_AR}
                LD=${HWLOC_LINKER}
                RANLIB=${CMAKE_RANLIB}
                ${HWLOC_C_ADDITIONAL_FLAGS}
                ${HWLOC_CXX_ADDITIONAL_FLAGS}
                ${HWLOC_LDD_ADDITIONAL_FLAGS}
                --host=${HWLOC_TRIPLE}
                --target=${HWLOC_TRIPLE}
        )
    endif ()

    ExternalProject_Add(hwloc_dep
            SOURCE_DIR ${sourceDir}
            CONFIGURE_HANDLED_BY_BUILD true
            CONFIGURE_COMMAND ${sourceDir}/autogen.sh && ${sourceDir}/configure ${HWLOC_CONFIGURE_COMMAND} --prefix=${ELPIDA_LOCAL_INSTALL_DIR} --enable-static --disable-shared --enable-plugins=no --disable-readme --disable-cairo --disable-libxml2 --disable-io --disable-pci --disable-opencl --disable-cuda --disable-nvml --disable-rsmi --disable-levelzero --disable-gl --disable-libudev
            BUILD_COMMAND ${MAKE_EXECUTABLE} -j
            INSTALL_COMMAND ${MAKE_EXECUTABLE} install
            TEST_COMMAND ""
            BUILD_BYPRODUCTS ${HWLOC_LIBRARY}
    )


    file(MAKE_DIRECTORY ${HWLOC_LIB_DIR})
    file(MAKE_DIRECTORY ${HWLOC_INCLUDE_DIR})

    add_library(hwloc INTERFACE IMPORTED GLOBAL)
    target_include_directories(hwloc INTERFACE ${HWLOC_INCLUDE_DIR})
    target_link_libraries(hwloc INTERFACE "${HWLOC_LIBRARY}")
    add_dependencies(hwloc hwloc_dep)

endfunction()

include_hwloc(${CMAKE_CURRENT_SOURCE_DIR}/hwloc)
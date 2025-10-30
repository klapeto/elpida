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

    set(ENV_FILE ${CMAKE_CURRENT_BINARY_DIR}/env.sh)
    file(WRITE ${ENV_FILE} "#!/bin/bash\n")

    if (ANDROID)
        set(HWLOC_TRIPLE ${CMAKE_LIBRARY_ARCHITECTURE}${ANDROID_NATIVE_API_LEVEL})
        file(APPEND ${ENV_FILE} "export CC=\"${CMAKE_C_COMPILER} --target=${HWLOC_TRIPLE}\"\n")
        file(APPEND ${ENV_FILE} "export CXX=\"${CMAKE_CXX_COMPILER} --target=${HWLOC_TRIPLE}\"\n")
        file(APPEND ${ENV_FILE} "export STRIP=\"${ANDROID_STRIP}\"\n")
    else ()
        file(APPEND ${ENV_FILE} "export CC=\"${CMAKE_C_COMPILER}\"\n")
        file(APPEND ${ENV_FILE} "export CXX=\"${CMAKE_CXX_COMPILER}\"\n")
    endif ()

    if (NOT HWLOC_TRIPLE)
        execute_process(COMMAND ${CMAKE_C_COMPILER} -dumpmachine OUTPUT_STRIP_TRAILING_WHITESPACE OUTPUT_VARIABLE HWLOC_TRIPLE)
    endif ()

    file(APPEND ${ENV_FILE} "export AR=\"${CMAKE_AR}\"\n")
    file(APPEND ${ENV_FILE} "export RANLIB=\"${CMAKE_RANLIB}\"\n")
    file(APPEND ${ENV_FILE} "export CFLAGS=\"${CMAKE_C_FLAGS}\"\n")
    file(APPEND ${ENV_FILE} "export CXXFLAGS=\"${CMAKE_CXX_FLAGS}\"\n")

    file(APPEND ${ENV_FILE} "${sourceDir}/autogen.sh && ${sourceDir}/configure --host=${HWLOC_TRIPLE} --with-sysroot=${CMAKE_SYSROOT} --prefix=${ELPIDA_LOCAL_INSTALL_DIR} --enable-static --disable-shared --enable-plugins=no --disable-readme --disable-cairo --disable-libxml2 --disable-io --disable-pci --disable-opencl --disable-cuda --disable-nvml --disable-rsmi --disable-levelzero --disable-gl --disable-libudev")
    file(CHMOD ${ENV_FILE} PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_WRITE GROUP_EXECUTE WORLD_READ WORLD_WRITE WORLD_EXECUTE)

    ExternalProject_Add(hwloc_dep
            SOURCE_DIR ${sourceDir}
            CONFIGURE_HANDLED_BY_BUILD true
            CONFIGURE_COMMAND ${ENV_FILE}
            BUILD_COMMAND ${MAKE_EXECUTABLE} VERBOSE=1 V=1 -j$(nproc)
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
function(include_hwloc sourceDir)

    include(GNUInstallDirs)
    include(ExternalProject)
    include(ProcessorCount)
    ProcessorCount(NUM_JOBS)

    set_property(DIRECTORY PROPERTY EP_UPDATE_DISCONNECTED true)

    set(HWLOC_LIB_DIR ${ELPIDA_LOCAL_INSTALL_DIR}/${CMAKE_INSTALL_LIBDIR})
    set(HWLOC_INCLUDE_DIR ${ELPIDA_LOCAL_INSTALL_DIR}/${CMAKE_INSTALL_INCLUDEDIR})
    set(HWLOC_LIBRARY ${HWLOC_LIB_DIR}/${CMAKE_STATIC_LIBRARY_PREFIX}hwloc${CMAKE_STATIC_LIBRARY_SUFFIX})

    find_program(MAKE_EXECUTABLE NAMES make REQUIRED)
    find_program(SH_EXECUTABLE NAMES sh REQUIRED)

    set(HWLOC_CONFIG_FILE ${CMAKE_CURRENT_BINARY_DIR}/hwloc.config.sh)
    generate_environment_file(${HWLOC_CONFIG_FILE} "${sourceDir}/autogen.sh && ${sourceDir}/configure --host=$TRIPLE --with-sysroot=$SYSROOT --prefix=${ELPIDA_LOCAL_INSTALL_DIR} --enable-static --disable-shared --enable-plugins=no --disable-readme --disable-cairo --disable-libxml2 --disable-io --disable-pci --disable-opencl --disable-cuda --disable-nvml --disable-rsmi --disable-levelzero --disable-gl --disable-libudev")

    ExternalProject_Add(hwloc_dep
            SOURCE_DIR ${sourceDir}
            CONFIGURE_HANDLED_BY_BUILD true
            CONFIGURE_COMMAND ${SH_EXECUTABLE} ${HWLOC_CONFIG_FILE}
            BUILD_COMMAND ${MAKE_EXECUTABLE} -j${NUM_JOBS}
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

include_hwloc(${ELPIDA_EXTERN_PATH}/hwloc)
project("Elpida platform specific" VERSION ${ELPIDA_VERSION}
        DESCRIPTION "Elpida platform specific library"
        HOMEPAGE_URL ${ELPIDA_WEBSITE}
        LANGUAGES CXX C)

add_library(elpida-platform
        TopologyLoader.cpp

        NumaAllocator.Unix.cpp
        NumaAllocator.Windows.cpp

        OsUtilities.Unix.cpp
        OsUtilities.Windows.cpp

        OsInfoLoader.Unix.cpp
        OsInfoLoader.Windows.cpp

        MemoryInfoLoader.Unix.cpp
        MemoryInfoLoader.Windows.cpp

        CpuInfoLoader.X86.cpp
        CpuInfoLoader.Arm.Unix.cpp
        CpuInfoLoader.Arm64.Windows.cpp
        CpuInfoLoader.RiscV64.Unix.cpp

        Process.cpp
        Process.Unix.cpp
        Process.Windows.cpp

        AnonymousPipe.cpp
        AnonymousPipe.Unix.cpp
        AnonymousPipe.Windows.cpp
        AsyncPipeReader.cpp
        NumaAllocatorFactory.cpp
)

add_library(elpida-jep106 jep106.c)

target_link_libraries(elpida-platform PUBLIC elpida-core hwloc elpida-jep106)

if (UNIX)
    include(${ELPIDA_ROOT_DIRECTORY}/cmake-modules/FindNUMA.cmake)

    if (NUMA_FOUND)
        set(ELPIDA_HAVE_NUMA 1 CACHE BOOL "Whether numa is available")
        target_link_libraries(elpida-platform PUBLIC numa)
    endif ()

endif ()

if (ELPIDA_INSTALL_CORE_LIBRARIES)
    install(TARGETS elpida-platform elpida-jep106 ARCHIVE)
endif ()
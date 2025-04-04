project("Elpida platform specific" VERSION ${ELPIDA_VERSION}
        DESCRIPTION "Elpida platform specific library"
        HOMEPAGE_URL ${ELPIDA_WEBSITE}
        LANGUAGES CXX C)

set(ELPIDA_PLATFORM_SOURCES
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

        Process.cpp
        Process.Unix.cpp
        Process.Windows.cpp

        AnonymousPipe.cpp
        AnonymousPipe.Unix.cpp
        AnonymousPipe.Windows.cpp
        AsyncPipeReader.cpp
        NumaAllocatorFactory.cpp
        CpuInfoLoader.Arm64.Windows.cpp
)

add_library(elpida-platform ${ELPIDA_PLATFORM_SOURCES})

target_link_libraries(elpida-platform PUBLIC elpida-core)

if (MSVC)
    include(FindHWLOC)
    target_include_directories(elpida-platform PUBLIC ${HWLOC_INCLUDE_DIRS})
    target_link_directories(elpida-platform PUBLIC ${HWLOC_LIBRARY_DIRS})
    target_link_libraries(elpida-platform PUBLIC libhwloc)
else()
    target_link_libraries(elpida-platform PUBLIC hwloc)
endif()

if (UNIX)
    target_link_libraries(elpida-platform PUBLIC numa)
endif ()
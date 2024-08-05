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

target_link_libraries(elpida-platform elpida-core)

if (MSVC)
    include(FindHWLOC)
    target_include_directories(elpida-platform PRIVATE ${HWLOC_INCLUDE_DIRS})
    target_link_directories(elpida-platform PRIVATE ${HWLOC_LIBRARY_DIRS})
    target_link_libraries(elpida-platform libhwloc)
else()
    target_link_libraries(elpida-platform hwloc)
endif()

if (UNIX)
    target_link_libraries(elpida-platform numa)
endif ()
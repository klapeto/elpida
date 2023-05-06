project("Elpida platform specific" VERSION ${ELPIDA_VERSION}
        DESCRIPTION "Elpida platform specific library"
        HOMEPAGE_URL ${ELPIDA_WEBSITE}
        LANGUAGES CXX C)

set(ELPIDA_PLATFORM_SOURCES
        TopologyLoader.cpp

        BenchmarkGroupModule.cpp
        BenchmarkGroupModule.Unix.cpp
        BenchmarkGroupModule.Windows.cpp

        NumaAllocator.Unix.cpp
        NumaAllocator.Windows.cpp

        OsUtilities.Unix.cpp
        OsUtilities.Windows.cpp

        OsInfoLoader.Unix.cpp
        OsInfoLoader.Windows.cpp

        MemoryInfoLoader.Unix.cpp
        MemoryInfoLoader.Windows.cpp

        CpuInfoLoader.X86.cpp
        CpuInfoLoader.Arm.Unix.cpp Process.cpp Process.hpp Process.Unix.cpp AnonymousPipe.cpp AnonymousPipe.hpp AnonymousPipe.Unix.cpp AnonymousPipe.Windows.cpp Process.Windows.cpp)

add_library(elpida-platform ${ELPIDA_PLATFORM_SOURCES})

target_link_libraries(elpida-platform elpida-core hwloc numa)
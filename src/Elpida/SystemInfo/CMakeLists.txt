project(Elpida-SystemInfo VERSION ${ELPIDA_VERSION}
        DESCRIPTION "Elpida system info"
        HOMEPAGE_URL ${ELPIDA_WEBSITE}
        LANGUAGES CXX C)

add_library(elpida-systemInfo
        ArmInfo.Unix.cpp
        ArmInfo.Windows.cpp
        CpuCache.cpp
        CpuFeature.cpp
        CpuInfo.cpp
        CpuKind.cpp
        MemoryInfo.cpp
        MemoryInfo.Unix.cpp
        MemoryInfo.Windows.cpp
        OsInfo.cpp
        OsInfo.Unix.cpp
        OsInfo.Windows.cpp
        ProcessorNode.cpp
        ProcessorNode.Unix.cpp
        ProcessorNode.Windows.cpp
        SystemTopology.cpp
        TargetTimeCalculator.cpp
        TimingInfo.cpp
        X86Info.cpp
        )

target_link_libraries(elpida-systemInfo elpida-common elpida-utilities)
target_include_directories(elpida-systemInfo PUBLIC ${CMAKE_SOURCE_DIR}/include )
target_include_directories(elpida-systemInfo PRIVATE ${ELPIDA_BINARY_INCLUDE_DIR})
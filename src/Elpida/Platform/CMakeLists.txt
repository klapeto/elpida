project("Elpida platform specific" VERSION ${ELPIDA_VERSION}
        DESCRIPTION "Elpida system information library"
        HOMEPAGE_URL ${ELPIDA_WEBSITE}
        LANGUAGES CXX C)

set(ELPIDA_PLATFORM_SOURCES
        TopologyLoader.cpp
        NumaAllocator.Unix.cpp)

add_library(elpida-platform ${ELPIDA_PLATFORM_SOURCES})

target_link_libraries(elpida-platform elpida-core hwloc numa)
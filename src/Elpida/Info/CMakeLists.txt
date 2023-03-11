project("Elpida Info" VERSION ${ELPIDA_VERSION}
        DESCRIPTION "Elpida system information library"
        HOMEPAGE_URL ${ELPIDA_WEBSITE}
        LANGUAGES CXX C)

set(ELPIDA_INFO_SOURCES
        TopologyLoader.cpp
        )

add_library(elpida-info ${ELPIDA_INFO_SOURCES})

target_link_libraries(elpida-info elpida-core hwloc)

#set_property(TARGET elpida-info PROPERTY POSITION_INDEPENDENT_CODE ON)
project("Elpida Backend Information Dumper" VERSION ${ELPIDA_VERSION}
        DESCRIPTION "Dumps information for Elpida web backend"
        HOMEPAGE_URL ${ELPIDA_WEBSITE}
        LANGUAGES CXX C)

set(ELPIDA_BACKEND_INFO_DUMPER_SOURCES
        Main.cpp)

add_executable(elpida-backend-info-dumper ${ELPIDA_BACKEND_INFO_DUMPER_SOURCES})
target_link_libraries(elpida-backend-info-dumper PRIVATE elpida-core elpida-application elpida-platform elpida-shared)
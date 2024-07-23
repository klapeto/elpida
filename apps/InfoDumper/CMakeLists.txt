project("Elpida Information Dumper" VERSION ${ELPIDA_VERSION}
        DESCRIPTION "Dumps information for Elpida"
        HOMEPAGE_URL ${ELPIDA_WEBSITE}
        LANGUAGES CXX C)

set(ELPIDA_INFO_DUMPER_SOURCES
        Main.cpp)


add_executable(elpida-info-dumper ${ELPIDA_INFO_DUMPER_SOURCES})
target_link_libraries(elpida-info-dumper elpida-core overheads-calculator elpida-platform elpida-shared)

install(TARGETS elpida-info-dumper RUNTIME DESTINATION bin)
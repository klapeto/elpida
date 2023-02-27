project("Elpida test program" VERSION ${ELPIDA_VERSION}
        DESCRIPTION
        HOMEPAGE_URL ${ELPIDA_WEBSITE}
        LANGUAGES CXX C)


add_executable(elpida-test Main.cpp)

target_link_libraries(elpida-test elpida)
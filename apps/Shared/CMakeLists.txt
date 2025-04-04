project("Elpida Shared" VERSION ${ELPIDA_VERSION}
        DESCRIPTION "Elpida Application Shared"
        HOMEPAGE_URL ${ELPIDA_WEBSITE}
        LANGUAGES CXX C)

add_library(elpida-shared
        JsonSerializer.cpp
        InfoGetter.cpp)

target_link_libraries(elpida-shared PUBLIC elpida-core elpida-platform)

target_include_directories(elpida-shared PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
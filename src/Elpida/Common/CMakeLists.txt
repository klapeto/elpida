project(Elpida-Common VERSION ${ELPIDA_VERSION}
        DESCRIPTION "Elpida system info"
        HOMEPAGE_URL ${ELPIDA_WEBSITE}
        LANGUAGES CXX C)

add_library(elpida-common
        ElpidaException.cpp
        OperationCanceledException.cpp
        )

target_include_directories(elpida-common PUBLIC ${CMAKE_SOURCE_DIR}/include )
target_include_directories(elpida-common PRIVATE ${ELPIDA_BINARY_INCLUDE_DIR})
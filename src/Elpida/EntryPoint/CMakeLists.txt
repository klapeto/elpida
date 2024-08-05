set(ELPIDA_ENTRY_POINT_SOURCES
        Main.cpp
        ArgumentsHelper.cpp
        JsonFormatter.cpp
        DefaultFormatter.cpp
        ResultFormatter.cpp
)

add_library(elpida-entry-point ${ELPIDA_ENTRY_POINT_SOURCES})

target_link_libraries(elpida-entry-point elpida-core elpida-platform)
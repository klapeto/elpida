project("Elpida common tasks" VERSION ${ELPIDA_VERSION}
        DESCRIPTION "Elpida common tasks"
        HOMEPAGE_URL ${ELPIDA_WEBSITE}
        LANGUAGES CXX C)

set(ELPIDA_COMMON_TASKS_SOURCES
        FileReadTask.cpp
        FileWriteTask.cpp
        )

add_library(elpida-common-tasks ${ELPIDA_COMMON_TASKS_SOURCES})

target_link_libraries(elpida-common-tasks PRIVATE elpida-core)

set_property(TARGET elpida-common-tasks PROPERTY POSITION_INDEPENDENT_CODE ON)

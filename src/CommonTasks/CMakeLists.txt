project("Elpida common tasks" VERSION ${ELPIDA_VERSION}
        DESCRIPTION "Elpida common tasks"
        HOMEPAGE_URL ${ELPIDA_WEBSITE}
        LANGUAGES CXX C)

set(ELPIDA_COMMON_TASKS_SOURCES
        FileReadTask.cpp
        FileWriteTask.cpp
        ParseXmlTask.cpp
        AllocateMemoryTask.cpp
        GenerateRandomDataTask.cpp
        GenerateLoremIpsum.cpp
)

add_library(elpida-common-tasks ${ELPIDA_COMMON_TASKS_SOURCES})

target_link_libraries(elpida-common-tasks PUBLIC elpida-core elpida-xml)

install(TARGETS elpida-common-tasks)

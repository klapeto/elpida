set(ELPIDA_COMMON_TASKS_SOURCES
        FileReadTask.cpp
        FileWriteTask.cpp
        ParseXmlTask.cpp
        AllocateMemoryTask.cpp
)

add_library(elpida-common-tasks ${ELPIDA_COMMON_TASKS_SOURCES})

target_link_libraries(elpida-common-tasks elpida-core elpida-xml)
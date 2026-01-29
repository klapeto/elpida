project("Elpida common tasks" VERSION ${ELPIDA_VERSION}
        DESCRIPTION "Elpida common tasks"
        HOMEPAGE_URL ${ELPIDA_WEBSITE}
        LANGUAGES CXX C)

add_library(elpida-common-tasks
        FileReadTask.cpp
        FileWriteTask.cpp
        ParseXmlTask.cpp
        AllocateMemoryTask.cpp
        GenerateRandomDataTask.cpp
        GenerateLoremIpsum.cpp)

target_include_directories(elpida-common-tasks PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(elpida-common-tasks PUBLIC elpida-core elpida-xml)

if (ELPIDA_INSTALL_COMMON_TASKS_LIBRARIES)
    install(TARGETS elpida-common-tasks ARCHIVE)
endif ()
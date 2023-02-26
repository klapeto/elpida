project(Elpida-CommonTasks VERSION ${ELPIDA_VERSION}
        DESCRIPTION "Elpida common tasks"
        HOMEPAGE_URL ${ELPIDA_WEBSITE}
        LANGUAGES CXX C)

add_library(elpida-commonTasks
        AllocateMemory/AllocateMemorySpecification.cpp
        AllocateMemory/AllocateMemory.cpp
        ReadFile/ReadFileSpecification.cpp
        ReadFile/ReadFile.cpp
        WriteFile/WriteFileSpecification.cpp
        WriteFile/WriteFile.cpp
        )

target_link_libraries(elpida-commonTasks elpida-engine)
target_include_directories(elpida-commonTasks PUBLIC ${CMAKE_SOURCE_DIR}/include )
target_include_directories(elpida-commonTasks PRIVATE ${ELPIDA_BINARY_INCLUDE_DIR})
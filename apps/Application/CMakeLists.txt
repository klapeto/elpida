project(elpida-application VERSION ${ELPIDA_VERSION}
        DESCRIPTION "Elpida Application Core"
        HOMEPAGE_URL ${ELPIDA_WEBSITE}
        LANGUAGES CXX C)

set(ELPIDA_APPLICATION_SOURCES

        Event.cpp
        EventSubscription.cpp
        DataUploader.cpp

        Controllers/CustomBenchmarkController.cpp
        Controllers/Controller.cpp
        Controllers/BenchmarkConfigurationInstanceController.cpp

        Models/Abstractions/CollectionModel.cpp
        Models/Abstractions/CollectionItem.cpp
        Models/Abstractions/AssociativeModel/AssociativeModel.cpp
        Models/Abstractions/ListModel/ListModel.cpp
        Models/Abstractions/ListModel/ListModelItem.cpp
        Models/Abstractions/AssociativeModel/AssociativeItem.cpp

        Models/SystemInfo/TopologyModel.cpp
        Models/SystemInfo/TopologyNodeModel.cpp
        Models/Abstractions/Model.cpp

        Models/SystemInfo/CpuInfoModel.cpp
        Models/SystemInfo/MemoryInfoModel.cpp
        Models/SystemInfo/OsInfoModel.cpp
        Models/SystemInfo/TimingModel.cpp
        Models/SystemInfo/OsInfoModel.cpp
        Models/Custom/CustomBenchmarkModel.cpp
        Models/Benchmark/BenchmarkGroupModel.cpp
        Models/Benchmark/BenchmarkModel.cpp
        Models/Benchmark/BenchmarkConfigurationModel.cpp
        Models/Benchmark/BenchmarkResultModel.cpp
        Models/Custom/CustomBenchmarkResultsModel.cpp
        Models/Benchmark/TaskResultModel.cpp
        Models/Benchmark/TaskModel.cpp

        Core/MessageService.cpp
        Core/BenchmarkExecutionService.cpp
        Core/Promise.cpp
        Core/ThreadQueue.cpp
        Core/SettingsService.cpp
        Core/BenchmarkUploaderService.cpp
        Core/BenchmarkUploaderService.hpp
        Models/BenchmarkRunConfigurationModel.cpp
        Models/BenchmarkRunConfigurationModel.hpp
        Controllers/BenchmarkRunConfigurationController.cpp
        Controllers/BenchmarkRunConfigurationController.hpp
        Models/Full/FullBenchmarkResultModel.cpp
        Models/Full/FullBenchmarkResultModel.hpp
        Controllers/FullBenchmarkController.cpp
        Controllers/FullBenchmarkController.hpp
        Models/Full/FullBenchmarkModel.cpp
        Models/Full/FullBenchmarkModel.hpp
)


if (MSYS OR MINGW)
    generate_product_version(
            ELPIDA_APPLICATION_SOURCES
            NAME ${PROJECT_NAME}
            ICON ${ELPIDA_ROOT_DIRECTORY}/images/Elpida-icon.ico
            VERSION_MAJOR ${ELPIDA_VERSION_MAJOR}
            VERSION_MINOR ${ELPIDA_VERSION_MINOR}
            VERSION_PATCH ${ELPIDA_VERSION_REVISION}
            VERSION_REVISION ${ELPIDA_VERSION_BUILD}
            COMPANY_NAME ${ELPIDA_COMPANY_NAME}
            FILE_DESCRIPTION ${PROJECT_DESCRIPTION}
    )
endif()

add_library(elpida-application ${ELPIDA_APPLICATION_SOURCES})

add_definitions(-DCPPHTTPLIB_OPENSSL_SUPPORT)

include(FindOpenSSL)

if (ELPIDA_WEB_API_KEY)
    add_definitions(-DELPIDA_WEB_API_KEY="${ELPIDA_WEB_API_KEY}")
endif()

if (ELPIDA_WEB_API_URL)
    add_definitions(-DELPIDA_WEB_API_URL="${ELPIDA_WEB_API_URL}")
endif()

if(MSYS OR MINGW)
    target_link_libraries(elpida-application ws2_32 crypt32)
else()
    target_link_libraries(elpida-application)
endif()

target_link_libraries(elpida-application elpida-core elpida-platform OpenSSL::SSL)

if (UNIX OR MSYS OR MINGW)
    if (${CMAKE_CXX_COMPILER_VERSION} STRLESS 9.3)
        target_link_libraries(elpida-application stdc++fs)
    endif()

    target_compile_options(elpida-application PUBLIC -fcoroutines)
endif()

set_property(TARGET elpida-application PROPERTY CXX_STANDARD 20)

target_include_directories(elpida-application PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})


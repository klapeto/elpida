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
        ModelBuilderJson.cpp
        ModelBuilderJson.hpp
        ResultSerializer.cpp
        ResultSerializer.hpp
        Controllers/FullBenchmarkInstance.cpp
        Controllers/FullBenchmarkInstance.hpp
        Core/BenchmarkScore.hpp
        Core/ResultsHTMLReporter.cpp
        Core/ResultsHTMLReporter.hpp
        Core/PathsService.cpp
        Core/PathsService.hpp
        Core/DesktopService.cpp
        Core/DesktopService.hpp
        Core/BenchmarkStatisticsService.cpp
        Core/BenchmarkStatisticsService.hpp
        Models/MemoryBenchmark/MemoryBenchmarkModel.cpp
        Models/MemoryBenchmark/MemoryBenchmarkModel.hpp
        Models/MemoryBenchmark/MemoryBenchmarkResultModel.cpp
        Models/MemoryBenchmark/MemoryBenchmarkResultModel.hpp
        Controllers/MemoryBenchmarkController.cpp
        Controllers/MemoryBenchmarkController.hpp
        FullBenchmarkInstancesLoader.cpp
        FullBenchmarkInstancesLoader.hpp
        FullBenchmarkInstances/CppCompilation.cpp
        FullBenchmarkInstances/CppCompilation.hpp
)

file(GLOB ELPIDA_BENCHMARK_INSTANCES FullBenchmarkInstances/*.cpp)

add_library(elpida-application ${ELPIDA_APPLICATION_SOURCES} ${ELPIDA_BENCHMARK_INSTANCES})

if (ELPIDA_WEB_API_KEY)
    target_compile_definitions(elpida-application PRIVATE -DELPIDA_WEB_API_KEY="${ELPIDA_WEB_API_KEY}")
endif()

if (ELPIDA_WEB_API_URL)
    target_compile_definitions(elpida-application PRIVATE -DELPIDA_WEB_API_URL="${ELPIDA_WEB_API_URL}")
endif()

target_compile_definitions(elpida-application PRIVATE -DCPPHTTPLIB_OPENSSL_SUPPORT)

if(MSYS OR MINGW OR MSVC)
    target_link_libraries(elpida-application PUBLIC ssl crypto ws2_32 crypt32 zlibstatic)
else()
    target_link_libraries(elpida-application PUBLIC ssl crypto zlibstatic)
endif()

target_link_libraries(elpida-application PUBLIC elpida-core elpida-platform elpida-shared)

if (UNIX OR MSYS OR MINGW)
    if (${CMAKE_CXX_COMPILER_VERSION} STRLESS 9.3 AND ${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
        target_link_libraries(elpida-application PUBLIC stdc++fs)
    endif()

    if (CXX_STANDARD GREATER_EQUAL 20)
        target_compile_options(elpida-application PUBLIC -fcoroutines)
    endif ()
endif()

target_include_directories(elpida-application PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

install(FILES Assets/placeholder DESTINATION bin/assets)
install(FILES Assets/custom-report-template.html DESTINATION bin/assets)
install(FILES Assets/full-report-template.html DESTINATION bin/assets)
install(FILES Assets/Elpida_Background.svg DESTINATION bin/assets)
install(FILES Assets/Elpida_Icon.svg DESTINATION bin/assets)


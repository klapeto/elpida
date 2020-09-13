project(elpidaCore VERSION 0.0.1
        DESCRIPTION "Elpida Application Core"
        LANGUAGES CXX C)

set(ELPIDA_CORE_SOURCES
        Core/ElpidaMediator.cpp
        Core/DataUploader.cpp
        Core/Abstractions/ResultFormatter.cpp
        Core/JsonResultFormatter.cpp

        Models/Benchmarks/BenchmarksModel.cpp
        Models/Abstractions/CollectionModel.cpp
        Models/Abstractions/CollectionItem.cpp
        Models/Abstractions/EventArgs/CollectionChangedEventArgs.cpp
        Models/Abstractions/CollectionModelObserver.cpp
        Models/BenchmarkResultsModel.cpp
        Models/BenchmarkRunnerModel.cpp
        Models/BenchmarkConfigurationsCollectionModel.cpp
        Models/Abstractions/AssociativeModel/AssociativeModel.cpp
        Models/Abstractions/ListModel/ListModel.cpp
        Models/Abstractions/ListModel/ListModelItem.cpp
        Models/Abstractions/AssociativeModel/AssociativeItem.cpp
        Models/BenchmarkConfigurationModel.cpp
        Models/Benchmarks/BenchmarkGroup.cpp
        Models/GlobalConfigurationModel.cpp

        Controllers/BenchmarksController.cpp
        Controllers/BenchmarkRunnerController.cpp
        Controllers/BenchmarkConfigurationController.cpp
        Controllers/UploadController.cpp

        Utilities/OsUtilities.cpp
        Utilities/xdg-open.c)

add_library(elpidaCore ${ELPIDA_CORE_SOURCES})

add_definitions(-DCPPHTTPLIB_OPENSSL_SUPPORT)

include(FindOpenSSL)

target_link_libraries(elpidaCore PRIVATE elpida OpenSSL::SSL)

if (ELPIDA_WEB_API_KEY)
    add_definitions(-DELPIDA_WEB_API_KEY="${ELPIDA_WEB_API_KEY}")
endif()

if(MSYS)
    target_link_libraries(elpidaCore PRIVATE elpida OpenSSL::SSL ws2_32 crypt32)
endif()

target_include_directories(elpidaCore PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_include_directories(elpida PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_BINARY_DIR}")


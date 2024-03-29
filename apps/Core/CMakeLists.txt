project(elpidaCore VERSION ${ELPIDA_VERSION}
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
        Models/Benchmarks/BenchmarkGroup.cpp
        Models/GlobalConfigurationModel.cpp
        Models/AffinityModel.cpp
        Models/SelectedBenchmarksModel.cpp

        Controllers/BenchmarksController.cpp
        Controllers/BenchmarkRunnerController.cpp
        Controllers/UploadController.cpp

        Utilities/OsUtilities.cpp
        Utilities/xdg-open.c)


if (MSYS OR MINGW)
    generate_product_version(
            ELPIDA_CORE_SOURCES
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

add_library(elpidaCore ${ELPIDA_CORE_SOURCES})

add_definitions(-DCPPHTTPLIB_OPENSSL_SUPPORT)

include(FindOpenSSL)

if (ELPIDA_WEB_API_KEY)
    add_definitions(-DELPIDA_WEB_API_KEY="${ELPIDA_WEB_API_KEY}")
endif()

if (ELPIDA_WEB_API_URL)
    add_definitions(-DELPIDA_WEB_API_URL="${ELPIDA_WEB_API_URL}")
endif()

if(MSYS OR MINGW)
    target_link_libraries(elpidaCore ws2_32 crypt32)
else()
    target_link_libraries(elpidaCore)
endif()

target_link_libraries(elpidaCore elpida OpenSSL::SSL)

if (UNIX OR MSYS OR MINGW)
    if (${CMAKE_CXX_COMPILER_VERSION} STRLESS 9.3)
        target_link_libraries(elpidaCore stdc++fs)
    endif()
endif()


target_include_directories(elpidaCore PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_include_directories(elpida PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_BINARY_DIR}")


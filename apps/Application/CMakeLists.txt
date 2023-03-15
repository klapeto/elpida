project(elpida-application VERSION ${ELPIDA_VERSION}
        DESCRIPTION "Elpida Application Core"
        HOMEPAGE_URL ${ELPIDA_WEBSITE}
        LANGUAGES CXX C)

set(ELPIDA_APPLICATION_SOURCES

        Models/Abstractions/CollectionModel.cpp
        Models/Abstractions/CollectionItem.cpp
        Models/Abstractions/EventArgs/CollectionChangedEventArgs.cpp
        Models/Abstractions/CollectionModelObserver.cpp
        Models/Abstractions/AssociativeModel/AssociativeModel.cpp
        Models/Abstractions/ListModel/ListModel.cpp
        Models/Abstractions/ListModel/ListModelItem.cpp
        Models/Abstractions/AssociativeModel/AssociativeItem.cpp

        Models/AvailableBenchmarksModel.cpp

        Event.cpp
        EventSubscription.cpp)


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

target_link_libraries(elpida-application elpida-core OpenSSL::SSL)

if (UNIX OR MSYS OR MINGW)
    if (${CMAKE_CXX_COMPILER_VERSION} STRLESS 9.3)
        target_link_libraries(elpida-application stdc++fs)
    endif()
endif()

target_include_directories(elpida-application PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})


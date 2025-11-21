project("Elpida Backend Information Dumper" VERSION ${ELPIDA_VERSION}
        DESCRIPTION "Dumps information for Elpida web backend"
        HOMEPAGE_URL ${ELPIDA_WEBSITE}
        LANGUAGES CXX C)

set(ELPIDA_BACKEND_INFO_DUMPER_SOURCES
        Main.cpp)

if (MSYS OR MINGW OR MSVC)
    generate_product_version(
            ELPIDA_BACKEND_INFO_DUMPER_SOURCES
            NAME ${PROJECT_NAME}
            ICON ${ELPIDA_ROOT_DIRECTORY}/images/Elpida-icon.ico
            VERSION_MAJOR ${ELPIDA_VERSION_MAJOR}
            VERSION_MINOR ${ELPIDA_VERSION_MINOR}
            VERSION_PATCH ${ELPIDA_VERSION_REVISION}
            VERSION_REVISION ${ELPIDA_VERSION_BUILD}
            COMPANY_NAME ${ELPIDA_COMPANY_NAME}
            FILE_DESCRIPTION "Information dumper Elpida Web Backend"
    )
endif()

add_executable(elpida-backend-info-dumper ${ELPIDA_BACKEND_INFO_DUMPER_SOURCES})

target_link_libraries(elpida-backend-info-dumper PUBLIC elpida-core elpida-application elpida-platform elpida-shared)
project("Elpida CLI"
        DESCRIPTION "Elpida CLI Front End"
        LANGUAGES CXX C)

set(ELPIDA_CLI_SOURCES
        Main.cpp
        ArgumentsHelper.cpp
        CliSettingsService.cpp
        CliPathsService.cpp
        CliDesktopService.cpp
        CliMessageService.cpp
        DefaultOutputFormatter.cpp)

if (MSYS OR MINGW OR MSVC)
    generate_product_version(
            ELPIDA_CLI_SOURCES
            NAME ${PROJECT_NAME}
            ICON ${ELPIDA_ROOT_DIRECTORY}/images/Elpida-icon.ico
            VERSION_MAJOR ${ELPIDA_VERSION_MAJOR}
            VERSION_MINOR ${ELPIDA_VERSION_MINOR}
            VERSION_PATCH ${ELPIDA_VERSION_REVISION}
            VERSION_REVISION ${ELPIDA_VERSION_BUILD}
            COMPANY_NAME ${ELPIDA_COMPANY_NAME}
            FILE_DESCRIPTION "The CLI of Elpida for running benchmarks"
    )
endif()

add_executable(elpida-cli ${ELPIDA_CLI_SOURCES})

target_link_libraries(elpida-cli PUBLIC elpida-core elpida-platform elpida-application)

install(TARGETS elpida-cli RUNTIME DESTINATION bin)
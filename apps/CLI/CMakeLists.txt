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

add_executable(elpida-cli ${ELPIDA_CLI_SOURCES})

target_link_libraries(elpida-cli PRIVATE elpida-core elpida-platform elpida-application)

install(TARGETS elpida-cli RUNTIME DESTINATION bin)
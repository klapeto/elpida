set(ELPIDA_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")

configure_file("${ELPIDA_INCLUDE_DIR}/Elpida/Config.hpp.in" "Elpida/Config.hpp")
set(ELPIDA_BINARY_INCLUDE_DIR "${CMAKE_CURRENT_BINARY_DIR}")

if (MSYS OR MINGW)
    generate_product_version(
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

add_subdirectory(Common)
add_subdirectory(Utilities)
add_subdirectory(SystemInfo)
add_subdirectory(Engine)
add_subdirectory(CommonTasks)

#if (UNIX)
#    include(FindThreads)
#    include(${CMAKE_SOURCE_DIR}/cmake-modules/FindNUMA.cmake)
##    find_library(numa REQUIRED)
#    find_package(Threads)
#    target_link_libraries(elpida numa ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT})
#endif()
#
#include(FindHWLOC)
#
#find_package(HWLOC REQUIRED)
#
#target_link_libraries(elpida hwloc)
#
#target_include_directories(elpida PUBLIC "${ELPIDA_INCLUDE_DIR}" "${CMAKE_CURRENT_BINARY_DIR}")
#
#install(DIRECTORY ${ELPIDA_INCLUDE_DIR}/Elpida DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} FILES_MATCHING PATTERN "*.h*")
#install(TARGETS elpida
#        PUBLIC_HEADER DESTINATION ${ELPIDA_HEADER_INSTALL_DIR}
#        LIBRARY DESTINATION ${ELPIDA_LIB_INSTALL_DIR}
#        ARCHIVE DESTINATION ${ELPIDA_LIB_INSTALL_DIR}
#        )
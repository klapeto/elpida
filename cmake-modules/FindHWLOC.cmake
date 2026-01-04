# Module for locating hwloc

include(FindPackageHandleStandardArgs)

find_path(HWLOC_ROOT_DIR
        NAMES include/hwloc.h
        PATHS ENV HWLOC_ROOT
        DOC "HWLOC root directory")

find_path(HWLOC_INCLUDE_DIR
        NAMES hwloc.h
        HINTS ${HWLOC_ROOT_DIR}
        PATH_SUFFIXES include
        DOC "HWLOC include directory")

find_library(HWLOC_LIBRARY
        NAMES hwloc
        HINTS ${HWLOC_ROOT_DIR}
        DOC "HWLOC library")

if (HWLOC_LIBRARY)
    get_filename_component(HWLOC_LIBRARY_DIR ${HWLOC_LIBRARY} PATH)
endif()

mark_as_advanced(HWLOC_INCLUDE_DIR HWLOC_LIBRARY_DIR HWLOC_LIBRARY)

find_package_handle_standard_args(HWLOC REQUIRED_VARS HWLOC_ROOT_DIR HWLOC_INCLUDE_DIR HWLOC_LIBRARY)

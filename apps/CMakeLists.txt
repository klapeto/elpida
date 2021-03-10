add_subdirectory(Core)
add_subdirectory(Qt)

if (ELPIDA_DEBUG_BUILD EQUAL 1)
    add_subdirectory(MetadataDumper)
endif()
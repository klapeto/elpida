set(ELPIDA_PLUGIN_INSTALL_PATH "${CMAKE_INSTALL_PREFIX}/Elpida")

set(TASK_BATCHES_CONFIG_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR})

configure_file("${PROJECT_SOURCE_DIR}/include/TaskBatches/Config.hpp.in" "Config.hpp")

add_subdirectory(General)
add_subdirectory(Image)
add_subdirectory(Memory)
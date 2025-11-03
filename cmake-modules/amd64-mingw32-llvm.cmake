set(CMAKE_SYSTEM_NAME Windows)

set(CMAKE_SYSTEM_PROCESSOR x86_64)

set(CMAKE_C_COMPILER x86_64-w64-mingw32-clang)
set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-clang++)
set(CROSS_TRIPLE x86_64-w64-mingw32.static)

# where is the target environment located
#set(CMAKE_FIND_ROOT_PATH /usr/i586-mingw32msvc
        #        /home/alex/mingw-install
#)

# adjust the default behavior of the FIND_XXX() commands:
# search programs in the host environment
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# search headers and libraries in the target environment
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

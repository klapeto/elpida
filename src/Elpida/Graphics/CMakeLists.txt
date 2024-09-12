project("Elpida Graphics")

add_library(elpida-graphics
        Matrix.cpp
)

target_link_libraries(elpida-graphics)

install(TARGETS elpida-graphics)
project("Elpida Math")

add_library(elpida-math
        Matrix.cpp
)

target_link_libraries(elpida-math)

install(TARGETS elpida-math)
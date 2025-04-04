project("Elpida Math")

add_library(elpida-math
        Matrix.cpp
        DynamicMatrix.cpp
)

target_link_libraries(elpida-math)
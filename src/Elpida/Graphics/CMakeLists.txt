project("Elpida Graphics")

add_library(elpida-graphics
        Color.cpp
        Light.cpp
        ShaderColor.cpp
        Triangle.cpp
        Vector3.cpp
        Shader.cpp
        Scene.cpp
        Camera.cpp
        Ray.cpp
        FrameBuffer.cpp
)

target_link_libraries(elpida-graphics PRIVATE elpida-math)

install(TARGETS elpida-graphics)
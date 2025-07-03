cmake_minimum_required(VERSION 3.31)
project(openGL_transport_simulation)

set(CMAKE_CXX_STANDARD 17)

include_directories(src/app)
include_directories(src/model)
include_directories(src/view)
link_directories(include/imgui)

# dependencies
include_directories(include)
find_package(glfw3 CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)

# to be able to import glfw whenever we want
add_definitions(-DGLFW_INCLUDE_NONE)




# executable
add_executable(app
        src/glad/glad.c
        src/app/app.cpp
        src/view/Shader.cpp
        src/view/Texture.cpp
        src/view/Camera.cpp
        src/view/LightSource.cpp
        src/view/Skybox.cpp
        src/model/RailroadMap.cpp
        src/model/Mesh.cpp
        src/model/Model.cpp
        src/model/Train.cpp
        src/model/Train.h
        src/model/TrainManager.cpp
        src/main.cpp
        include/imgui/imgui.cpp
        include/imgui/imgui_demo.cpp
        include/imgui/imgui_draw.cpp
        include/imgui/imgui_tables.cpp
        include/imgui/imgui_widgets.cpp
        include/imgui/backends/imgui_impl_glfw.cpp
        include/imgui/backends/imgui_impl_opengl3.cpp
        src/model/Tunnel.cpp
        src/model/Tunnel.h
        "src/app/stations.h"
        src/app/stones.h
)

target_link_libraries(app PRIVATE glfw assimp::assimp)
target_include_directories(app PRIVATE include/imgui)
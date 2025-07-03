cmake_minimum_required(VERSION 3.31)
project(openGL_transport_simulation)

set(CMAKE_CXX_STANDARD 17)

include_directories(src/app)
include_directories(src/model)
include_directories(src/view)

# dependencies
include_directories(include)
find_package(glfw3 CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)

# to be able to import glfw whenever we want
add_definitions(-DGLFW_INCLUDE_NONE)

# executable
add_executable(app
        src/app/app.cpp
        src/view/Shader.cpp
        src/view/Texture.cpp
        src/view/Camera.cpp
        src/model/Mesh.cpp
        src/model/Model.cpp
        src/glad/glad.c
        src/main.cpp
        src/view/LightSource.cpp
        src/model/RailroadMap.cpp
        src/model/Train.cpp
        src/model/Train.h
        src/model/TrainManager.cpp
        src/model/TrainManager.h
)

target_link_libraries(app PRIVATE glfw assimp::assimp)
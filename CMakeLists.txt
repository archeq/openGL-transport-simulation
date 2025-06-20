cmake_minimum_required(VERSION 3.31)
project(openGL_transport_simulation)

set(CMAKE_CXX_STANDARD 17)

include_directories(src/app)

# dependencies
include_directories(include)
include_directories(dependencies/GLFW/include)
link_directories(dependencies/GLFW/lib)

# to be able to import glfw whenever we want
add_definitions(-DGLFW_INCLUDE_NONE)

# executable
add_executable(app
        src/app/app.cpp
        src/app/app.h
        src/glad/glad.c
        src/main.cpp)

target_link_libraries(app glfw3)
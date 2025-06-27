//
// Created by KUCIA on 20.06.2025.
//

#include "app.h"

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <Shader.h>
#include <Texture.h>
#include <Mesh.h>

#define WINDOW_WIDTH 1600
#define WINDOW_HEIGHT 900
#define WINDOW_TITLE "Simulation"


void App::run() {
    // initialization
    glfw_init();
    window = window_init(WINDOW_WIDTH, WINDOW_HEIGHT);
    glad_init();

    // viewport
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    // running the loop
    loop();
}

void App::terminate() {
    glfwTerminate();
    std::cout << "App was terminated" << std::endl;
}

void App::glfw_init() {
    if (is_glfw_initialized) return;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    is_glfw_initialized = true;
}

void App::glad_init() {
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        terminate();
        return;
    }

    is_glad_initialized = true;
}

GLFWwindow *App::window_init(int width, int height) {
    GLFWwindow *window = glfwCreateWindow(width, height, WINDOW_TITLE, nullptr, nullptr);
    if (!window) {
        std::cout << "Failed to create GLFW window" << std::endl;
        terminate();
    }
    glfwMakeContextCurrent(window);
    return window;
}

Shader shader;

void test_setup() {
    // Temporal function for TESTING only
    shader = Shader("../shaders/simple_shader.vert", "../shaders/simple_shader.frag");
}
void render(GLFWwindow *window) {
    // Temporal function for TESTING only
    glClearColor(0.05f, 0.0f, 0.05f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(window);
}

void App::loop() {
    test_setup();
    while (!glfwWindowShouldClose(window)) {

        // rendering
        render(window);

        // updating
        glfwPollEvents();
    }
}

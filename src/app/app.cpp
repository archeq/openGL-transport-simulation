//
// Created by KUCIA on 20.06.2025.
//

#include "app.h"

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <Shader.h>
#include <Texture.h>

#define GLM_ENABLE_EXPERIMENTAL
#include "Camera.h"
#include "LightSource.h"
#include "Model.h"
#include "RailroadMap.h"
#include "Skybox.h"
#include "TrainManager.h"
#include "glm/glm.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "stb/stb_image.h"
#include <vector>
#include <string>


#define WINDOW_WIDTH 1600
#define WINDOW_HEIGHT 900
#define WINDOW_RATIO ((float) WINDOW_WIDTH / (float) WINDOW_HEIGHT)
#define WINDOW_TITLE "Simulation"

// these must be global because callbacks must be either functions or static methods
float mouse_last_x;
float mouse_last_y;
bool firstMouse = true;

bool error = false;

void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
    error = true;
    std::cout << "[OpenGL Error](" << type << ") " << message << std::endl;
}

void App::run() {
    // glfw initialization
    glfw_init();
    window = window_init(WINDOW_WIDTH, WINDOW_HEIGHT);

    // cursor
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad (openGL) initialization
    glad_init();

    stbi_set_flip_vertically_on_load(true);

    // depth
    glEnable(GL_DEPTH_TEST);

    // viewport
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    // Enable debug output
    // glEnable(GL_DEBUG_OUTPUT);
    // glDebugMessageCallback(MessageCallback, 0);

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

    // mouse
    mouse_last_x = width / 2.0f;
    mouse_last_y = height / 2.0f;
    return window;
}

Shader simpleShader, lightSourceShader, modelShader;
unsigned int VBO, cubeVAO, lightCubeVAO;

Texture boxTexture, boxSpecularMap, railTexture, stationTexture;
Camera camera;
Model trainModel;
LightSource lightSource;
Skybox skybox;

// Объявляем глобальную переменную railroadMap и routes
RailroadMap railroadMap;
TrainManager trainManager(railroadMap);
std::shared_ptr<Model> trainModelPtr;


std::vector<std::string> faces = {
    "../textures/right.jpg",
    "../textures/left.jpg",
    "../textures/top.jpg",
    "../textures/bottom.jpg",
    "../textures/front.jpg",
    "../textures/back.jpg"
};

std::vector<glm::vec3> allPoints = {
    // Индексы 0-6: точки первого маршрута
    glm::vec3(-60.0f, 0.0f, 51.0f),   // A (0)
    glm::vec3(-30.0f, 0.0f, 45.0f),   // B (1)
    glm::vec3(-5.0f, 0.0f, 30.0f),    // C (2)
    glm::vec3(35.0f, 0.0f, -10.0f),    // D (3)
    glm::vec3(110.0f, 0.0f, -15.0f),    // E (4)
    glm::vec3(150.0f, 0.0f, 40.0f),    // F (5)



    glm::vec3(-115.0f, 0.0f, -45.0f),    // G (6)
    glm::vec3(-70.0f, 0.0f, -55.0f),  // H (7)
    glm::vec3(-20.0f, 0.0f, -25.0f),  // I (8)
    glm::vec3(75.0f, 0.0f, 45.0f),   // J (9)
    glm::vec3(130.0f, 0.0f, 75.0f),   // K (10)
    glm::vec3(60.0f, 0.0f, 125.0f),   // L (11)
    glm::vec3(30.0f, 0.0f, 85.0f),    // M (12)
    glm::vec3(-15.0f, 0.0f, -105.0f),    // N (13)
    glm::vec3(-60.0f, 0.0f, -160.0f),    // O (14)
};

// Определяем маршруты по индексам
std::vector<std::vector<int>> routeIndices = {
    {0, 1, 2, 3, 4, 5},      // route 1
    {6, 7, 8, 3, 9, 10,},    // route 2
    {11, 12, 2, 8, 13, 14,},    // route 3
};



void test_setup() {
    // Temporal function for TESTING only
    simpleShader = Shader("../shaders/specular_map.vert", "../shaders/specular_map.frag");
    lightSourceShader = Shader("../shaders/light_source.vert", "../shaders/light_source.frag");
    modelShader = Shader("../shaders/model_shader.vert", "../shaders/model_shader.frag");
    boxTexture = Texture(std::string("../textures/container.png"));
    boxSpecularMap = Texture(std::string("../textures/container2_specular.png"));
    camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f));
    lightSource = LightSource(glm::vec3(0.0f, 50.0f, 0.0f),
                        glm::vec3(0.5f, 0.5f, 0.5f),
                         glm::vec3(0.5f, 0.5f, 0.5f),
                        glm::vec3(1.0f, 1.0f, 1.0f));
    skybox.prepare(faces);

    // СНАЧАЛА инициализируем RailroadMap
    railroadMap.initialize(allPoints, routeIndices);
    railroadMap.loadTextures("../textures/rail.png", "../textures/station.png");
    railroadMap.loadStationBoxTextures("../textures/container.png", "../textures/container2_specular.png");

    std::cout << "Routes created: " << railroadMap.getRouteCount() << std::endl;

    // ЗАТЕМ создаем модель поезда
    // TODO: TRAIN MODEL LOAD
    // trainModelPtr = std::make_shared<Model>("../models/train/Intercity 125 Executive Livery With Buffers.obj");
    trainModelPtr = std::make_shared<Model>("../models/train_minimalistic/mini_train.obj");
    if (!trainModelPtr->is_loaded()) {
        std::cout << "ERROR:: Train model failed to load!" << std::endl;
    } else {
        trainModelPtr->scale = glm::vec3(0.12);
        trainModelPtr->rotation_deg = glm::vec3(0, 90, 0);
    }

    // ТОЛЬКО ПОСЛЕ этого добавляем поезда
    // Добавляем поезда на разные маршруты
    trainManager.addTrain(trainModelPtr, 0, 15.0f);  // Маршрут 0, скорость 15
    trainManager.addTrain(trainModelPtr, 1, 15.0f);  // Маршрут 1, скорость 15
    trainManager.addTrain(trainModelPtr, 2, 15.0f);  // Маршрут 2, скорость 15

    // Настройки замедления у станций
    trainManager.setStationSlowdownSettings(8.0f, 0.99f);

}

void App::process_input(GLFWwindow *window) const {

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime_s);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime_s);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime_s);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime_s);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, deltaTime_s);
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, deltaTime_s);
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        camera.ProcessKeyboard(KEY_1, deltaTime_s);
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        camera.ProcessKeyboard(KEY_2, deltaTime_s);
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
        camera.ProcessKeyboard(KEY_3, deltaTime_s);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        mouse_last_x = xpos;
        mouse_last_y = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - mouse_last_x;
    float yoffset = mouse_last_y - ypos; // reversed since y-coordinates go from bottom to top

    mouse_last_x = xpos;
    mouse_last_y = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void draw_test_box(const glm::vec3 &light_pos, const glm::mat4 &projection, const glm::mat4 &view) {
    // ======================================
    //              test box
    // ======================================
    simpleShader.use();
    simpleShader.setVec3("light.position", light_pos);
    simpleShader.setVec3("viewPos", camera.position);

    // light properties
    simpleShader.setVec3("light.ambient", glm::vec3(0.3f, 0.3f, 0.3f));
    simpleShader.setVec3("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
    simpleShader.setVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));

    // material properties
    // shader.setVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));  // for shader without specular mapping
    simpleShader.setFloat("material.shininess", 80.0f);

    simpleShader.setMat4("projection", projection);
    simpleShader.setMat4("view", view);

    auto model = glm::mat4(1.0f);
    simpleShader.setMat4("model", model);

    // bind diffuse map
    simpleShader.setInt("material.diffuse", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, boxTexture.id);

    // bind specular map
    simpleShader.setInt("material.specular", 1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, boxSpecularMap.id);

    // render the cube
    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void render(GLFWwindow *window) {
    // 1) temporal function for TESTING only
    // 2) there's nothing more permanent than a temporary solution
    glClearColor(0.00f, 0.0f, 0.05f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // skybox
    skybox.draw(camera);

    // light
    lightSource.draw_as_cube(camera, 3);

    // Добавляем отрисовку поездов через менеджер
    trainManager.draw(modelShader, camera, lightSource);

    // railroad
    railroadMap.draw(simpleShader, camera, lightSource);




    glUseProgram(0);
    glfwSwapBuffers(window);
}

void App::loop() {
    test_setup();
    while (!glfwWindowShouldClose(window)) {

        // time
        const auto currentFrame = static_cast<float>(glfwGetTime());
        deltaTime_s = currentFrame - lastFrame_s;
        lastFrame_s = currentFrame;

        // input
        process_input(window);

        // train
        trainManager.update(deltaTime_s);

        // rendering
        render(window);

        // updating
        glfwPollEvents();
    }
}

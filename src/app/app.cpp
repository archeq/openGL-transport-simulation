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

#define GLM_ENABLE_EXPERIMENTAL
#include "Camera.h"
#include "Model.h"
#include "glm/glm.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/gtx/transform.hpp"
#include "stb/stb_image.h"

#define WINDOW_WIDTH 1600
#define WINDOW_HEIGHT 900
#define WINDOW_RATIO ((float) WINDOW_WIDTH / (float) WINDOW_HEIGHT)
#define WINDOW_TITLE "Simulation"

// these must be global because callbacks must be either functions or static methods
float mouse_last_x;
float mouse_last_y;
bool firstMouse = true;

// cube
float cube_vertices_pnt[] = {
       // positions          // normals           // texture coords
       -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
       -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
       -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
       -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
       -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
       -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
       -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
       -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
       -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
       -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
       -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
       -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
       -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
       -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
       -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
       -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
       -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
       -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };

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
Texture boxTexture, boxSpecularMap;
Camera camera;
Model backpackModel;

void test_setup() {
    // Temporal function for TESTING only
    simpleShader = Shader("../shaders/specular_map.vert", "../shaders/specular_map.frag");
    lightSourceShader = Shader("../shaders/light_source.vert", "../shaders/light_source.frag");
    modelShader = Shader("../shaders/model_shader.vert", "../shaders/model_shader.frag");
    boxTexture = Texture(std::string("../textures/container.png"));
    boxSpecularMap = Texture(std::string("../textures/container2_specular.png"));
    camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f));
    backpackModel = Model("../models/backpack/backpack.obj");

    // test box
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices_pnt), cube_vertices_pnt, GL_STATIC_DRAW);

    glBindVertexArray(cubeVAO);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // texture attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void *>(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // light cube
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);


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
    simpleShader.setVec3("viewPos", camera.Position);

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

void draw_light_cube(const glm::vec3 &light_pos,const glm::mat4 &projection,const glm::mat4 &view) {
    // ======================================
    //              light cube
    // ======================================
    lightSourceShader.use();
    lightSourceShader.setMat4("projection", projection);
    lightSourceShader.setMat4("view", view);
    auto model = glm::mat4(1.0f);
    model = glm::translate(model, light_pos);
    model = glm::scale(model, glm::vec3(0.1f));
    lightSourceShader.setMat4("model", model);

    // render the cube
    glBindVertexArray(lightCubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void render(GLFWwindow *window) {
    // 1) temporal function for TESTING only
    // 2) there's nothing more permanent than a temporary solution
    glClearColor(0.00f, 0.0f, 0.05f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto light_pos = glm::vec3(1.7, 2.0, 3.0f);
    auto projection = glm::perspective(glm::radians(camera.Zoom), WINDOW_RATIO, 0.1f, 100.0f);
    auto view = camera.GetViewMatrix();

    // test box
    // draw_test_box(light_pos, projection, view);

    // test model
    // TODO: я это все скоро красиво организую в class Model
    auto model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
    modelShader.use();
    modelShader.setMat4("model", model);
    modelShader.setMat4("view", view);
    modelShader.setMat4("projection", projection);
    modelShader.setVec3("lightPos", light_pos);
    modelShader.setVec3("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
    modelShader.setVec3("light.diffuse", glm::vec3(0.6f, 0.6f, 0.6f));
    modelShader.setVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
    modelShader.setVec3("viewPos", camera.Position);
    modelShader.setFloat("material.shininess", 25.0f);


    backpackModel.draw(modelShader);

    // light cube
    draw_light_cube(light_pos, projection, view);

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

        // rendering
        render(window);

        // updating
        glfwPollEvents();
    }
}

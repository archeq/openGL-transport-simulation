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
#include "glm/gtx/transform.hpp"
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "stations.h"
#include "stones.h"

#define WINDOW_WIDTH 1600
#define WINDOW_HEIGHT 900
#define WINDOW_RATIO ((float) WINDOW_WIDTH / (float) WINDOW_HEIGHT)
#define WINDOW_TITLE "Simulation"

// these must be global because callbacks must be either functions or static methods
float mouse_last_x;
float mouse_last_y;
bool firstMouse = true;
bool cursor_visible = true;

bool error = false;
bool pauseUpdate = true;

// Добавьте в начало файла app.cpp после включений
struct City {
    std::string name;
    std::vector<glm::vec3> points;
    std::vector<std::vector<int>> routeIndices;
    std::vector<std::string> stationNames;

    City(const std::string& cityName,
         const std::vector<glm::vec3>& cityPoints,
         const std::vector<std::vector<int>>& cityRoutes,
         const std::vector<std::string>& cityStations)
        : name(cityName), points(cityPoints), routeIndices(cityRoutes), stationNames(cityStations) {}
};

std::vector<City> cities;
size_t selectedCityIndex = 0;
bool showMainMenu = true;
bool citySelected = false;

void initializeCities() {


    cities.emplace_back("Simple City",
        parisPoints,
        parisRoutes,
        parisStations);
    cities.emplace_back("Warsaw",
        warsawPoints,
        warsawRoutes,
        warsawStations);
    cities.emplace_back("Minsk",
        minskPoints,
        minskRoutes,
        minskStations);
    cities.emplace_back("Metro City",
        berlinPoints,
        berlinRoutes,
        berlinStations);
    cities.emplace_back("Gavnar",
        gavnarPoints,
        gavnarRoutes,
        gavnarStations);
    cities.emplace_back("Subway City",
        metroPoints,
        metroRoutes,
        metroStations);
    cities.emplace_back("Valley Network",
        valleyPoints,
        valleyRoutes,
        valleyStations);




}

void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
    error = true;
    std::cout << "[OpenGL Error](" << type << ") " << message << std::endl;
}

Shader simpleShader, lightSourceShader, modelShader;
unsigned int VBO, cubeVAO, lightCubeVAO;

Texture boxTexture, boxSpecularMap, railTexture, stationTexture;
Camera camera;
Model trainModel, stone1, stone2, stone3;
LightSource lightSource;
Skybox skybox;

// Объявляем глобальную переменную railroadMap и routes
RailroadMap railroadMap;
TrainManager trainManager(railroadMap);
std::shared_ptr<Model> trainModelPtr;

Texture groundTexture;
unsigned int groundVAO, groundVBO;

size_t selectedTrainIndex = 0;
bool freeCameraMode = false;

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    if (camera.mode == FOLLOW) {
        camera.follow_distance += static_cast<float>(yoffset);
        camera.follow_distance = std::max(0.0f, camera.follow_distance);
    }
}

void App::run() {
    // glfw initialization
    glfw_init();

    // Проверяем, что GLFW инициализирован
    if (!is_glfw_initialized) {
        std::cout << "GLFW initialization failed" << std::endl;
        return;
    }

    window = window_init(WINDOW_WIDTH, WINDOW_HEIGHT);

    // cursor
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, cursor_visible ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);

    // glad (openGL) initialization
    glad_init();

    stbi_set_flip_vertically_on_load(true);

    // depth
    glEnable(GL_DEPTH_TEST);

    // MSAA
    glEnable(GL_MULTISAMPLE);

    // viewport
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    // Enable debug output
    // glEnable(GL_DEBUG_OUTPUT);
    // glDebugMessageCallback(MessageCallback, 0);

    // running the loop
    loop();
}

void App::terminate() {
    // Очистка ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    std::cout << "App was terminated" << std::endl;
}

void App::glfw_init() {
    if (is_glfw_initialized) return;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 8);

    // Дополнительные подсказки для совместимости
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

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

std::vector<std::string> faces = {
    "../textures/right.jpg",
    "../textures/left.jpg",
    "../textures/top.jpg",
    "../textures/bottom.jpg",
    "../textures/front.jpg",
    "../textures/back.jpg"
};


void loadSelectedCity(GLFWwindow* window) {
    if (selectedCityIndex >= cities.size()) return;

    const City& city = cities[selectedCityIndex];

    // Очищаем все данные предыдущего города
    railroadMap.clear();

    // Инициализируем RailroadMap с данными выбранного города
    railroadMap.initialize(city.points, city.routeIndices);
    railroadMap.loadTextures("../textures/rail.png", "../textures/station.png");
    railroadMap.loadStationBoxTextures("../textures/container.png", "../textures/container2_specular.png");
    railroadMap.setStationNames(city.stationNames);

    std::cout << "Loaded city: " << city.name << std::endl;
    std::cout << "Routes created: " << railroadMap.getRouteCount() << std::endl;

    // Очищаем существующие поезда и добавляем новые
    trainManager.~TrainManager(); // Явно вызываем деструктор
    new (&trainManager) TrainManager(railroadMap); // Placement new для пересоздания объекта

    // Добавляем поезда для каждого маршрута (если есть)
    size_t routeCount = railroadMap.getRouteCount();
    for (size_t i = 0; i < routeCount; ++i) {
        trainManager.addTrain(trainModelPtr, i, 25.0f);
        std::cout << "Added train for route " << i << std::endl;
    }

    citySelected = true;
    showMainMenu = false;

    // Устанавливаем курсор в зависимости от состояния паузы
    cursor_visible = pauseUpdate;
    glfwSetInputMode(window, GLFW_CURSOR, cursor_visible ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
}

// добавьте эту функцию после функции draw_test_box
void draw_ground(const Camera& camera, const LightSource& lightSource) {

    // Включаем polygon offset для земли
    glEnable(GL_POLYGON_OFFSET_FILL);
    // glPolygonOffset(1.0f, 1.0f); // Отодвигаем землю назад


    // Используем тот же шейдер, что и для других объектов
    simpleShader.use();

    simpleShader.setBool("use_texture", true);
    simpleShader.setVec3("light.position", lightSource.position);
    simpleShader.setVec3("viewPos", camera.position);
    simpleShader.setVec3("light.ambient", lightSource.ambient);
    simpleShader.setVec3("light.diffuse", lightSource.diffuse);
    simpleShader.setVec3("light.specular", lightSource.specular);
    simpleShader.setFloat("material.shininess", 25.0f);

    // Матрицы трансформации
    glm::mat4 projection = camera.getProjectionMatrix();
    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 model = glm::mat4(1.0f);

    // Масштабируем и позиционируем землю
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // Опускаем немного вниз
    model = glm::scale(model, glm::vec3(1000.0f, 1.0f, 1000.0f)); // Делаем очень большой

    simpleShader.setMat4("projection", projection);
    simpleShader.setMat4("view", view);
    simpleShader.setMat4("model", model);

    // Привязываем текстуру земли
    simpleShader.setInt("material.diffuse", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, groundTexture.id);

    // Используем ту же текстуру для specular map (можно заменить на отдельную)
    simpleShader.setInt("material.specular", 1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, groundTexture.id);

    // Отрисовываем плоскость
    glBindVertexArray(groundVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // Отключаем polygon offset после отрисовки
    glDisable(GL_POLYGON_OFFSET_FILL);
}

// добавьте эту функцию для инициализации геометрии земли
void setup_ground() {
    // Вершины для горизонтальной плоскости
    float groundVertices[] = {
        // Позиции          // Нормали         // Текстурные координаты
        -0.5f,  0.0f, -0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 100.0f, // Повторяем текстуру много раз
        -0.5f,  0.0f,  0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
         0.5f,  0.0f,  0.5f,  0.0f, 1.0f, 0.0f,  100.0f, 0.0f,

        -0.5f,  0.0f, -0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 100.0f,
         0.5f,  0.0f,  0.5f,  0.0f, 1.0f, 0.0f,  100.0f, 0.0f,
         0.5f,  0.0f, -0.5f,  0.0f, 1.0f, 0.0f,  100.0f, 100.0f
    };

    // Создаем VBO и VAO для земли
    glGenVertexArrays(1, &groundVAO);
    glGenBuffers(1, &groundVBO);

    glBindVertexArray(groundVAO);

    glBindBuffer(GL_ARRAY_BUFFER, groundVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(groundVertices), groundVertices, GL_STATIC_DRAW);

    // Позиции
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Нормали
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Текстурные координаты
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Загружаем текстуру земли
    groundTexture = Texture(std::string("../textures/dirt.jpg"));
}


void test_setup(GLFWwindow* window) {
    // Temporal function for TESTING only
    simpleShader = Shader("../shaders/specular_map.vert", "../shaders/specular_map.frag");
    lightSourceShader = Shader("../shaders/light_source.vert", "../shaders/light_source.frag");
    modelShader = Shader("../shaders/model_shader.vert", "../shaders/model_shader.frag");
    stone1 = Model("../models/stones/stone1.obj");
    stone2 = Model("../models/stones/stone2.obj");
    stone3 = Model("../models/stones/stone3.obj");

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 460");

    boxTexture = Texture(std::string("../textures/container.png"));
    boxSpecularMap = Texture(std::string("../textures/container2_specular.png"));
    camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f));
    camera.mode = FOLLOW;
    lightSource = LightSource(glm::vec3(0.0f, 50.0f, 0.0f),
                        glm::vec3(0.5f, 0.5f, 0.5f),
                         glm::vec3(0.5f, 0.5f, 0.5f),
                        glm::vec3(1.0f, 1.0f, 1.0f));
    skybox.prepare(faces);

    // Инициализируем города
    initializeCities();

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

    setup_ground();
}

bool tab_previous = false;
void App::process_input(GLFWwindow *window) const {

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (!cursor_visible) {
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
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
            pauseUpdate = !pauseUpdate;
            cursor_visible = pauseUpdate;
            glfwSetInputMode(window, GLFW_CURSOR, cursor_visible ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
        }
    }
    if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS) {
        if (!tab_previous) {
            cursor_visible = !cursor_visible;
            tab_previous = true;
        }
        glfwSetInputMode(window, GLFW_CURSOR, cursor_visible ? GLFW_CURSOR_NORMAL: GLFW_CURSOR_DISABLED);
    }
    else {
        tab_previous = false;
    }
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
    if (cursor_visible) return;

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

void draw_stones(const Shader &shader, const Camera &camera, const LightSource &lightSource) {
    for (size_t i = 0; i < positions1.size(); ++i) {
        stone1.position = positions1[i];
        stone1.scale = glm::vec3(scales1[i]);
        stone1.rotation_deg.y = rotations1[i];
        stone1.draw(shader, camera, lightSource);
    }
    for (size_t i = 0; i < positions2.size(); ++i) {
        stone2.position = positions2[i];
        stone2.scale = glm::vec3(scales2[i]);
        stone2.rotation_deg.y = rotations2[i];
        stone2.draw(shader, camera, lightSource);
    }
    for (size_t i = 0; i < positions3.size(); ++i) {
        stone3.position = positions3[i];
        stone3.scale = glm::vec3(scales3[i]);
        stone3.rotation_deg.y = rotations3[i];
        stone3.draw(shader, camera, lightSource);
    }
}


void render(GLFWwindow *window) {
    glClearColor(0.00f, 0.0f, 0.05f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Главное меню выбора города
    if (showMainMenu) {
        // Центрируем окно
        ImGui::SetNextWindowPos(ImVec2(WINDOW_WIDTH * 0.5f, WINDOW_HEIGHT * 0.5f),
                               ImGuiCond_Always, ImVec2(0.5f, 0.5f));
        ImGui::SetNextWindowSize(ImVec2(400, 300));

        if (ImGui::Begin("Main Menu", nullptr,
                        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                        ImGuiWindowFlags_NoCollapse)) {
            ImGui::Text("Welcome to Metro Systems Simulator");
            ImGui::Separator();
            ImGui::Text("Please, choose a city to explore:");
            ImGui::Separator();

            for (size_t i = 0; i < cities.size(); ++i) {
                if (ImGui::RadioButton(cities[i].name.c_str(), selectedCityIndex == i)) {
                    selectedCityIndex = i;
                }
                ImGui::Spacing();
            }

            ImGui::Separator();

            if (ImGui::Button("Start Simulation", ImVec2(380, 40))) {
                loadSelectedCity(window);
            }
        }
        ImGui::End();
    } else {
        // Обычная отрисовка сцены
        skybox.draw(camera);
        draw_ground(camera, lightSource);
        lightSource.draw_as_cube(camera, 3);

        simpleShader.use();
        simpleShader.setMat4("projection", camera.getProjectionMatrix());
        simpleShader.setMat4("view", camera.getViewMatrix());
        simpleShader.setMat4("model", glm::mat4(1.0f));
        simpleShader.setVec3("light.position", lightSource.position);
        simpleShader.setVec3("viewPos", camera.position);
        simpleShader.setVec3("light.ambient", lightSource.ambient);
        simpleShader.setVec3("light.diffuse", lightSource.diffuse);
        simpleShader.setVec3("light.specular", lightSource.specular);

        bool transparentTunnels = freeCameraMode;
        railroadMap.drawTunnels(simpleShader, transparentTunnels);
        trainManager.draw(modelShader, camera, lightSource);
        draw_stones(simpleShader, camera, lightSource);
        railroadMap.draw(simpleShader, camera, lightSource);
        railroadMap.drawStationLabels(camera, WINDOW_WIDTH, WINDOW_HEIGHT);

        // Панель управления (существующий код)
        if (ImGui::Begin("Camera Control")) {
            ImGui::Text("Update core");
            if (ImGui::Button("Pause/Unpause")) {
                pauseUpdate = !pauseUpdate;
                cursor_visible = pauseUpdate;
                glfwSetInputMode(window, GLFW_CURSOR, cursor_visible ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
            }

            ImGui::Text("Label Display Mode");

            if (ImGui::RadioButton("Static", railroadMap.getLabelDisplayMode() == LabelDisplayMode::STATIC && railroadMap.showStationLabels)) {
                railroadMap.setLabelDisplayMode(LabelDisplayMode::STATIC);
                railroadMap.showStationLabels = true;
            }

            if (ImGui::RadioButton("Dynamic", railroadMap.getLabelDisplayMode() == LabelDisplayMode::DYNAMIC && railroadMap.showStationLabels)) {
                railroadMap.setLabelDisplayMode(LabelDisplayMode::DYNAMIC);
                railroadMap.showStationLabels = true;
            }

            if (ImGui::RadioButton("Off", !railroadMap.showStationLabels)) {
                railroadMap.showStationLabels = false;
            }

            ImGui::Separator();

            // Добавляем контроль туннелей
            bool showTunnels = railroadMap.getShowTunnels();
            if (ImGui::Checkbox("Show Tunnels", &showTunnels)) {
                railroadMap.setShowTunnels(showTunnels);
            }

            ImGui::Separator();
            ImGui::Text("Camera Mode");

            if (ImGui::RadioButton("Free Camera", freeCameraMode)) {
                freeCameraMode = true;
                camera.mode = FREE;
            }

            if (ImGui::RadioButton("Follow Train", !freeCameraMode)) {
                freeCameraMode = false;
                camera.mode = FOLLOW;
            }

            if (!freeCameraMode) {
                ImGui::Text("Select Train to Follow:");
                size_t trainCount = trainManager.getTrainCount();
                for (size_t i = 0; i < trainCount; i++) {
                    std::string trainLabel = "Train " + std::to_string(i + 1);
                    if (ImGui::RadioButton(trainLabel.c_str(), selectedTrainIndex == i)) {
                        selectedTrainIndex = i;
                    }
                }
            }
            ImGui::Separator();
            if (ImGui::Button("Back to Main Menu", ImVec2(200, 30))) {
                showMainMenu = true;
                citySelected = false;
                pauseUpdate = true;
            }

        }
        ImGui::End();
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glUseProgram(0);
    glfwSwapBuffers(window);
}

void App::loop() {
    test_setup(window);
    while (!glfwWindowShouldClose(window)) {

        // time
        const auto currentFrame = static_cast<float>(glfwGetTime());
        deltaTime_s = currentFrame - lastFrame_s;
        lastFrame_s = currentFrame;

        // input
        process_input(window);

        // Обновляем камеру и поезда только если город выбран
        if (citySelected && !showMainMenu) {
            if (!freeCameraMode && camera.mode == FOLLOW) {
                if (static_cast<size_t>(selectedTrainIndex) < trainManager.getTrainCount()) {
                    glm::vec3 target = trainManager.getTrain(selectedTrainIndex)->getPosition();
                    camera.target_position = target;
                }
            }
            camera.update();

            if (!pauseUpdate)
                trainManager.update(deltaTime_s);
        }

        // rendering
        render(window);

        // updating
        glfwPollEvents();
    }
}
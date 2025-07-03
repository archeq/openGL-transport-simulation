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
    // Город 1 - существующий Gavnar
    std::vector<glm::vec3> gavnarPoints = {
        glm::vec3(-60.0f, 0.0f, 51.0f),   // A (0)
        glm::vec3(-30.0f, 0.0f, 45.0f),   // B (1)
        glm::vec3(-5.0f, 0.0f, 30.0f),    // C (2)
        glm::vec3(35.0f, 0.0f, -10.0f),   // D (3)
        glm::vec3(110.0f, 0.0f, -15.0f),  // E (4)
        glm::vec3(150.0f, 0.0f, 40.0f),   // F (5)
        glm::vec3(-115.0f, 0.0f, -45.0f), // G (6)
        glm::vec3(-70.0f, 0.0f, -55.0f),  // H (7)
        glm::vec3(-20.0f, 0.0f, -25.0f),  // I (8)
        glm::vec3(75.0f, 0.0f, 45.0f),    // J (9)
        glm::vec3(130.0f, 0.0f, 75.0f),   // K (10)
        glm::vec3(60.0f, 0.0f, 125.0f),   // L (11)
        glm::vec3(30.0f, 0.0f, 85.0f),    // M (12)
        glm::vec3(-15.0f, 0.0f, -105.0f), // N (13)
        glm::vec3(-60.0f, 0.0f, -160.0f), // O (14)
    };

    std::vector<std::vector<int>> gavnarRoutes = {
        {0, 1, 2, 3, 4, 5},
        {6, 7, 8, 3, 9, 10},
        {11, 12, 2, 8, 13, 14},
    };

    std::vector<std::string> gavnarStations = {
        "Gavnar", "North Terminal", "East Junction", "South Gate", "West End",
        "Final Stop", "Industrial Park", "Riverside", "Mountain View",
        "Green Park", "UAM (kazn perdezhom)", "Sports Complex",
        "Theater District", "Museum Quarter", "Airport Terminal"
    };

    // Город 2 - новый Metro City
    std::vector<glm::vec3> metroPoints = {
        glm::vec3(-80.0f, 0.0f, 80.0f),   // 0
        glm::vec3(0.0f, 0.0f, 90.0f),     // 1
        glm::vec3(80.0f, 0.0f, 60.0f),    // 2
        glm::vec3(100.0f, 0.0f, 0.0f),    // 3
        glm::vec3(60.0f, 0.0f, -80.0f),   // 4
        glm::vec3(-20.0f, 0.0f, -100.0f), // 5
        glm::vec3(-90.0f, 0.0f, -40.0f),  // 6
        glm::vec3(-70.0f, 0.0f, 20.0f),   // 7
        glm::vec3(20.0f, 0.0f, 40.0f),    // 8
        glm::vec3(40.0f, 0.0f, -20.0f),   // 9
    };

    std::vector<std::vector<int>> metroRoutes = {
        {0, 1, 2, 3},
        {4, 5, 6, 7},
        {8, 9, 3, 5},
    };

    std::vector<std::string> metroStations = {
        "Metro Center", "North Plaza", "East Tower", "Commerce Hub",
        "South Port", "Industrial Zone", "West Quarter", "Park Avenue",
        "City Square", "Business District"
    };

    // Город 3 - Valley Network
    std::vector<glm::vec3> valleyPoints = {
        glm::vec3(-120.0f, 0.0f, 0.0f),   // 0
        glm::vec3(-60.0f, 0.0f, 40.0f),   // 1
        glm::vec3(0.0f, 0.0f, 20.0f),     // 2
        glm::vec3(60.0f, 0.0f, 60.0f),    // 3
        glm::vec3(120.0f, 0.0f, 20.0f),   // 4
        glm::vec3(80.0f, 0.0f, -40.0f),   // 5
        glm::vec3(20.0f, 0.0f, -80.0f),   // 6
        glm::vec3(-40.0f, 0.0f, -60.0f),  // 7
    };

    std::vector<std::vector<int>> valleyRoutes = {
        {0, 1, 2, 3, 4},
        {5, 6, 7, 0},
    };

    std::vector<std::string> valleyStations = {
        "Valley Station", "Mountain View", "Central Valley",
        "Highland Park", "East Ridge", "Canyon Point",
        "River Crossing", "West Valley"
    };

    cities.emplace_back("Gavnar", gavnarPoints, gavnarRoutes, gavnarStations);
    cities.emplace_back("Metro City", metroPoints, metroRoutes, metroStations);
    cities.emplace_back("Valley Network", valleyPoints, valleyRoutes, valleyStations);
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

// Добавляем названия станций
std::vector<std::string> stationNames = {
    "Gavnar",                // Станция 0 (вместо "Central Station")
    "North Terminal",        // Станция 1 (вместо "Северная")
    "East Junction",         // Станция 2 (вместо "Восточная")
    "South Gate",            // Станция 3 (вместо "Южная")
    "West End",              // Станция 4 (вместо "Западная")
    "Final Stop",            // Станция 5 (вместо "Конечная")
    "Industrial Park",       // Станция 6 (вместо "Промышленная")
    "Riverside",             // Станция 7 (вместо "Речная")
    "Mountain View",         // Станция 8 (вместо "Горная")
    "Green Park",            // Станция 9 (вместо "Парковая")
    "UAM (kazn perdezhom)",  // Станция 10 (вместо "Университетская")
    "Sports Complex",        // Станция 11 (вместо "Спортивная")
    "Theater District",      // Станция 12 (вместо "Театральная")
    "Museum Quarter",        // Станция 13 (вместо "Музейная")
    "Airport Terminal"       // Станция 14 (вместо "Аэропорт")
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

    // // СНАЧАЛА инициализируем RailroadMap
    // railroadMap.initialize(allPoints, routeIndices);
    // railroadMap.loadTextures("../textures/rail.png", "../textures/station.png");
    // railroadMap.loadStationBoxTextures("../textures/container.png", "../textures/container2_specular.png");
    //
    //
    //
    // railroadMap.setStationNames(stationNames);
    //
    // std::cout << "Routes created: " << railroadMap.getRouteCount() << std::endl;

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

    trainManager.addTrain(trainModelPtr, 0, 25.0f);
    trainManager.addTrain(trainModelPtr, 1, 25.0f);
    trainManager.addTrain(trainModelPtr, 2, 25.0f);

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

std::vector<glm::vec3> positions1 = {
    glm::vec3(129.0f, 0.0f, 37.0f),
glm::vec3(-149.0f, 0.0f, 59.0f),
glm::vec3(-74.0f, 0.0f, 109.0f),
glm::vec3(-24.0f, 0.0f, -34.0f),
glm::vec3(21.0f, 0.0f, 85.0f),
glm::vec3(-29.0f, 0.0f, -92.0f),
glm::vec3(137.0f, 0.0f, 41.0f),
glm::vec3(-94.0f, 0.0f, 36.0f),
glm::vec3(-63.0f, 0.0f, -34.0f),
glm::vec3(74.0f, 0.0f, 21.0f),
glm::vec3(10.0f, 0.0f, 75.0f),
glm::vec3(134.0f, 0.0f, 123.0f),
glm::vec3(106.0f, 0.0f, -27.0f),
glm::vec3(-78.0f, 0.0f, -148.0f),
glm::vec3(-53.0f, 0.0f, -93.0f),
glm::vec3(-39.0f, 0.0f, -81.0f),
glm::vec3(136.0f, 0.0f, -1.0f),
glm::vec3(-37.0f, 0.0f, 74.0f),
glm::vec3(-47.0f, 0.0f, -11.0f),
glm::vec3(-20.0f, 0.0f, 72.0f),
glm::vec3(67.0f, 0.0f, 74.0f),
glm::vec3(113.0f, 0.0f, 134.0f),
glm::vec3(-56.0f, 0.0f, -112.0f),
glm::vec3(104.0f, 0.0f, 137.0f),
glm::vec3(-118.0f, 0.0f, 124.0f),
glm::vec3(-129.0f, 0.0f, -83.0f),
glm::vec3(72.0f, 0.0f, 105.0f),
glm::vec3(-11.0f, 0.0f, 92.0f),
glm::vec3(44.0f, 0.0f, 145.0f),
glm::vec3(-101.0f, 0.0f, 78.0f),
glm::vec3(62.0f, 0.0f, -98.0f),
glm::vec3(125.0f, 0.0f, -8.0f),
glm::vec3(30.0f, 0.0f, -83.0f),
glm::vec3(-114.0f, 0.0f, 35.0f),
glm::vec3(65.0f, 0.0f, -36.0f),
glm::vec3(-126.0f, 0.0f, 28.0f),
glm::vec3(-11.0f, 0.0f, -47.0f),
glm::vec3(65.0f, 0.0f, -3.0f),
glm::vec3(109.0f, 0.0f, 23.0f),
glm::vec3(-143.0f, 0.0f, -9.0f),
glm::vec3(136.0f, 0.0f, 138.0f),
glm::vec3(93.0f, 0.0f, 37.0f),
glm::vec3(-21.0f, 0.0f, 135.0f),
glm::vec3(108.0f, 0.0f, -110.0f),
glm::vec3(32.0f, 0.0f, -129.0f),
glm::vec3(-113.0f, 0.0f, 141.0f),
glm::vec3(5.0f, 0.0f, 131.0f),
glm::vec3(-48.0f, 0.0f, -2.0f),
glm::vec3(-50.0f, 0.0f, 16.0f),
glm::vec3(92.0f, 0.0f, 122.0f),
};
std::vector scales1 = {
    3.6f, 0.7f, 2.1f, 1.9f, 2.7f, 1.0f, 1.8f, 3.0f, 2.2f, 1.1f, 2.7f, 1.7f, 3.9f, 4.3f, 1.5f, 2.3f, 0.6f, 3.3f, 1.7f, 3.0f, 3.1f, 0.8f, 1.8f, 3.6f, 3.6f, 4.3f, 3.0f, 0.8f, 4.0f, 2.0f, 3.1f, 2.5f, 4.5f, 0.9f, 4.3f, 2.3f, 0.9f, 0.8f, 3.7f, 2.4f, 1.2f, 3.8f, 4.0f, 3.6f, 1.0f, 4.0f, 1.7f, 2.4f, 4.0f, 0.6f
};
std::vector rotations1 = { 130, 213, 69, 140, 330, 298, 155, 135, 22, 270, 191, 252, 19, 205, 120, 225, 103, 244, 324, 301, 112, 168, 225, 92, 57, 5, 185, 124, 32, 239, 5, 151, 153, 145, 84, 80, 218, 4, 183, 159, 205, 264, 134, 271, 360, 333, 46, 71, 68, 133
};
std::vector positions2 = {glm::vec3(-116.0f, 0.0f, 56.0f),
glm::vec3(-64.0f, 0.0f, -108.0f),
glm::vec3(57.0f, 0.0f, 53.0f),
glm::vec3(-124.0f, 0.0f, -103.0f),
glm::vec3(5.0f, 0.0f, -89.0f),
glm::vec3(104.0f, 0.0f, -67.0f),
glm::vec3(-100.0f, 0.0f, -6.0f),
glm::vec3(32.0f, 0.0f, -129.0f),
glm::vec3(-137.0f, 0.0f, -31.0f),
glm::vec3(26.0f, 0.0f, -22.0f),
glm::vec3(112.0f, 0.0f, -43.0f),
glm::vec3(-115.0f, 0.0f, 56.0f),
glm::vec3(-81.0f, 0.0f, 34.0f),
glm::vec3(-89.0f, 0.0f, -19.0f),
glm::vec3(74.0f, 0.0f, 21.0f),
glm::vec3(15.0f, 0.0f, 129.0f),
glm::vec3(105.0f, 0.0f, -131.0f),
glm::vec3(-65.0f, 0.0f, 81.0f),
glm::vec3(-92.0f, 0.0f, 64.0f),
glm::vec3(120.0f, 0.0f, 12.0f),
glm::vec3(136.0f, 0.0f, -114.0f),
glm::vec3(-100.0f, 0.0f, -135.0f),
glm::vec3(131.0f, 0.0f, -62.0f),
glm::vec3(108.0f, 0.0f, -87.0f),
glm::vec3(-87.0f, 0.0f, 13.0f),
glm::vec3(-64.0f, 0.0f, 31.0f),
glm::vec3(33.0f, 0.0f, 20.0f),
glm::vec3(-125.0f, 0.0f, 102.0f),
glm::vec3(87.0f, 0.0f, 42.0f),
glm::vec3(13.0f, 0.0f, 16.0f),
glm::vec3(70.0f, 0.0f, -34.0f),
glm::vec3(46.0f, 0.0f, -75.0f),
glm::vec3(148.0f, 0.0f, 3.0f),
glm::vec3(40.0f, 0.0f, 23.0f),
glm::vec3(115.0f, 0.0f, 91.0f),
glm::vec3(50.0f, 0.0f, -117.0f),
glm::vec3(52.0f, 0.0f, -109.0f),
glm::vec3(44.0f, 0.0f, -62.0f),
glm::vec3(38.0f, 0.0f, -86.0f),
glm::vec3(-133.0f, 0.0f, 130.0f),
glm::vec3(-64.0f, 0.0f, 44.0f),
glm::vec3(-114.0f, 0.0f, 52.0f),
glm::vec3(-26.0f, 0.0f, -130.0f),
glm::vec3(94.0f, 0.0f, 20.0f),
glm::vec3(-118.0f, 0.0f, 23.0f),
glm::vec3(-91.0f, 0.0f, 8.0f),
glm::vec3(-59.0f, 0.0f, -47.0f),
glm::vec3(-43.0f, 0.0f, 108.0f),
glm::vec3(-55.0f, 0.0f, -100.0f),
glm::vec3(-6.0f, 0.0f, -148.0f),};
std::vector scales2 = {3.3f, 1.7f, 3.5f, 1.1f, 4.4f, 0.7f, 0.8f, 1.7f, 4.4f, 4.2f, 4.6f, 4.3f, 3.4f, 0.6f, 4.1f, 2.7f, 3.0f, 3.7f, 0.7f, 1.8f, 3.8f, 1.3f, 2.7f, 4.9f, 5.0f, 0.6f, 3.3f, 4.9f, 2.9f, 3.4f, 4.6f, 4.2f, 4.3f, 4.1f, 1.7f, 1.1f, 4.5f, 4.0f, 0.6f, 1.7f, 0.7f, 3.9f, 2.4f, 1.2f, 1.8f, 4.8f, 0.6f, 1.3f, 2.9f, 1.3f, };
std::vector rotations2 = {117, 356, 139, 119, 17, 31, 241, 334, 183, 63, 262, 92, 7, 196, 233, 128, 339, 205, 316, 98, 148, 269, 18, 62, 287, 321, 18, 341, 255, 336, 53, 58, 317, 213, 130, 295, 67, 286, 292, 321, 276, 54, 165, 212, 297, 133, 174, 129, 45, 318};
std::vector positions3 = {glm::vec3(115.0f, 0.0f, 74.0f),
glm::vec3(-46.0f, 0.0f, 72.0f),
glm::vec3(104.0f, 0.0f, 121.0f),
glm::vec3(31.0f, 0.0f, 136.0f),
glm::vec3(-67.0f, 0.0f, -131.0f),
glm::vec3(121.0f, 0.0f, -36.0f),
glm::vec3(-8.0f, 0.0f, 69.0f),
glm::vec3(114.0f, 0.0f, -18.0f),
glm::vec3(117.0f, 0.0f, 126.0f),
glm::vec3(65.0f, 0.0f, 0.0f),
glm::vec3(134.0f, 0.0f, 73.0f),
glm::vec3(86.0f, 0.0f, -129.0f),
glm::vec3(25.0f, 0.0f, 103.0f),
glm::vec3(-95.0f, 0.0f, -27.0f),
glm::vec3(109.0f, 0.0f, -34.0f),
glm::vec3(-123.0f, 0.0f, -27.0f),
glm::vec3(135.0f, 0.0f, 117.0f),
glm::vec3(-28.0f, 0.0f, -103.0f),
glm::vec3(47.0f, 0.0f, -136.0f),
glm::vec3(149.0f, 0.0f, 22.0f),
glm::vec3(32.0f, 0.0f, -118.0f),
glm::vec3(140.0f, 0.0f, 131.0f),
glm::vec3(40.0f, 0.0f, -34.0f),
glm::vec3(53.0f, 0.0f, 91.0f),
glm::vec3(-7.0f, 0.0f, -14.0f),
glm::vec3(-90.0f, 0.0f, 34.0f),
glm::vec3(-6.0f, 0.0f, 140.0f),
glm::vec3(80.0f, 0.0f, 85.0f),
glm::vec3(-83.0f, 0.0f, -6.0f),
glm::vec3(15.0f, 0.0f, -122.0f),
glm::vec3(-64.0f, 0.0f, -134.0f),
glm::vec3(54.0f, 0.0f, -84.0f),
glm::vec3(-76.0f, 0.0f, 130.0f),
glm::vec3(81.0f, 0.0f, 137.0f),
glm::vec3(115.0f, 0.0f, 77.0f),
glm::vec3(46.0f, 0.0f, 101.0f),
glm::vec3(25.0f, 0.0f, -46.0f),
glm::vec3(-63.0f, 0.0f, 137.0f),
glm::vec3(-26.0f, 0.0f, -61.0f),
glm::vec3(-107.0f, 0.0f, -1.0f),
glm::vec3(-106.0f, 0.0f, 74.0f),
glm::vec3(55.0f, 0.0f, -95.0f),
glm::vec3(25.0f, 0.0f, -124.0f),
glm::vec3(1.0f, 0.0f, -4.0f),
glm::vec3(9.0f, 0.0f, -38.0f),
glm::vec3(16.0f, 0.0f, 80.0f),
glm::vec3(48.0f, 0.0f, -76.0f),
glm::vec3(111.0f, 0.0f, 3.0f),
glm::vec3(-19.0f, 0.0f, -131.0f),
glm::vec3(89.0f, 0.0f, -5.0f),};
std::vector scales3 = {3.0f, 3.8f, 2.9f, 0.8f, 3.8f, 4.8f, 1.4f, 1.3f, 3.6f, 4.3f, 2.8f, 3.4f, 1.4f, 0.8f, 0.5f, 4.8f, 3.5f, 2.1f, 0.5f, 0.6f, 0.8f, 4.6f, 0.7f, 2.6f, 2.4f, 3.3f, 2.5f, 0.9f, 3.7f, 4.7f, 1.1f, 3.1f, 2.0f, 3.0f, 0.9f, 2.9f, 2.6f, 3.9f, 3.0f, 1.8f, 5.0f, 4.2f, 2.6f, 3.6f, 4.2f, 4.8f, 2.9f, 4.2f, 3.8f, 1.6f, };
std::vector rotations3 = {67, 192, 347, 88, 300, 82, 70, 207, 192, 270, 318, 275, 132, 182, 283, 178, 216, 185, 18, 225, 229, 288, 308, 111, 23, 106, 157, 223, 302, 38, 152, 222, 2, 96, 44, 146, 192, 77, 245, 97, 270, 129, 359, 289, 37, 299, 330, 130, 248, 17};
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

// void render(GLFWwindow *window) {
//     // 1) temporal function for TESTING only
//     // 2) there's nothing more permanent than a temporary solution
//     glClearColor(0.00f, 0.0f, 0.05f, 1.0f);
//     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//     // ImGui frame
//     ImGui_ImplOpenGL3_NewFrame();
//     ImGui_ImplGlfw_NewFrame();
//     ImGui::NewFrame();
//
//     // skybox
//     skybox.draw(camera);
//
//     // ground
//     draw_ground(camera, lightSource);
//
//     // light
//     lightSource.draw_as_cube(camera, 3);
//
//     // Настройка шейдера для освещения
//     simpleShader.use();
//     simpleShader.setMat4("projection", camera.getProjectionMatrix());
//     simpleShader.setMat4("view", camera.getViewMatrix());
//     simpleShader.setMat4("model", glm::mat4(1.0f));
//     simpleShader.setVec3("light.position", lightSource.position);
//     simpleShader.setVec3("viewPos", camera.position);
//     simpleShader.setVec3("light.ambient", lightSource.ambient);
//     simpleShader.setVec3("light.diffuse", lightSource.diffuse);
//     simpleShader.setVec3("light.specular", lightSource.specular);
//
//     // Туннели (прозрачные в free camera режиме)
//     bool transparentTunnels = freeCameraMode;
//     railroadMap.drawTunnels(simpleShader, transparentTunnels);
//
//     // Добавляем отрисовку поездов через менеджер
//     trainManager.draw(modelShader, camera, lightSource);
//
//     // stones
//     draw_stones(simpleShader, camera, lightSource);
//
//     // railroad
//     railroadMap.draw(simpleShader, camera, lightSource);
//
//
//
//     // Этикетки станций
//     railroadMap.drawStationLabels(camera, WINDOW_WIDTH, WINDOW_HEIGHT);
//
//     // GUI панель управления
//     // GUI панель управления (замените существующий блок ImGui)
//
//
//     // Рендеринг ImGui
//     ImGui::Render();
//     ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
//
//
//     glUseProgram(0);
//     glfwSwapBuffers(window);
// }

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
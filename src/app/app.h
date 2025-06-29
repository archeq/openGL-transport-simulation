//
// Created by KUCIA on 20.06.2025.
//

#ifndef APP_H
#define APP_H
#include "GLFW/glfw3.h"

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);

class App {
public:
    GLFWwindow *window = nullptr;

    void run(); // initializes the app and starts its main loop
    static void terminate(); // kills the app
    void glfw_init(); // pretty much self-explanatory
    void glad_init();

private:
    bool is_glfw_initialized = false;
    bool is_glad_initialized = false;
    float deltaTime_s = 0.0f;
    float lastFrame_s = 0.0f;

    GLFWwindow *window_init(int width, int height);
    void process_input(GLFWwindow *window) const;

    void loop();
};

#endif //APP_H

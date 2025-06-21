//
// Created by KUCIA on 20.06.2025.
//

#ifndef APP_H
#define APP_H
#include "GLFW/glfw3.h"


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

    GLFWwindow *window_init(int width, int height);

    void loop();
};

#endif //APP_H

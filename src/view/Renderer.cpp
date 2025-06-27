//
// Created by KUCIA on 22.06.2025.
//

#include "Renderer.h"
#include <glad/glad.h>

void Renderer::render(Scene *, Camera *) {

    // clearing the screen
    glClearColor(background_color.x, background_color.y, background_color.z, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

}


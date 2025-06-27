//
// Created by KUCIA on 22.06.2025.
//

#ifndef RENDERER_H
#define RENDERER_H

#include "Camera.h"
#include "Scene.h"
#include <glm/vec3.hpp>


class Renderer {
public:
    void render(Scene *, Camera *);

    glm::vec3 background_color = glm::vec3(0.0f); // black
};



#endif //RENDERER_H

//
// Created by KUCIA on 01.07.2025.
//

#ifndef LIGHTSOURCE_H
#define LIGHTSOURCE_H

#include "Shader.h"
#include "Camera.h"

#include "glm/vec3.hpp"


class LightSource {
public:
    glm::vec3 position;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    LightSource() = default;
    LightSource(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);

    void draw_as_cube(const Camera &camera, float scale = 1.0f) const;

private:
    Shader shader;
    unsigned int VAO, VBO;

};



#endif //LIGHTSOURCE_H

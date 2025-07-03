//
// Created by KUCIA on 03.07.2025.
//

#ifndef SKYBOX_H
#define SKYBOX_H

#include <string>
#include <vector>

#include "Shader.h"
#include "Camera.h"


class Skybox {
public:
    Skybox() = default;
    void prepare(const std::vector<std::string> &faces);
    void draw(const Camera &camera) const;

private:
    unsigned int texture;
    unsigned int VAO, VBO;
    Shader shader;

};


#endif //SKYBOX_H

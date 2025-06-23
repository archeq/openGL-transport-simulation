//
// Created by KUCIA on 23.06.2025.
//

#ifndef MESH_H
#define MESH_H

#include <vector>
#include <Texture.h>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

#include "Shader.h"

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};


class Mesh {
public:
    // mesh data
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    // constructor
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

    void draw(Shader &shader);

private:
    unsigned int VAO, VBO, EBO;
    void setupMesh();
};



#endif //MESH_H

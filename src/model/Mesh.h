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
    glm::vec3 Tangent;
    glm::vec3 Bitangent;
};

struct MaterialProperties {
    glm::vec3 diffuseColor = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 specularColor = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 ambientColor = glm::vec3(0.1f, 0.1f, 0.1f);
    float shininess = 32.0f;
};

class Mesh {
public:
    // mesh data
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;
    MaterialProperties materialProps;

    // constructor
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures, MaterialProperties matProps = {});

    void draw(const Shader &shader) const;

private:
    unsigned int VAO, VBO, EBO;
    void setupMesh();
};

#endif //MESH_H
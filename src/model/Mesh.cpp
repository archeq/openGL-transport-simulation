//
// Created by KUCIA on 23.06.2025.
//

#include "Mesh.h"

#include <iostream>
#include <ostream>
#include <glad/glad.h>
#include <string>

using std::string;

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures) {
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;

    setupMesh();
}

void Mesh::setupMesh() {
    // generating buffers
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // binding Buffers
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

        // sending data to buffers
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
        // vertex positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
        // vertex normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(offsetof(Vertex, Normal)));
        // vertex texture coords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(offsetof(Vertex, TexCoords)));

    // unbinding buffers
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::draw(const Shader &shader) const {
    unsigned int diffuseNr = 1, specularNr = 1;

    for(unsigned int i=0; i < textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);

        // retrieve texture number
        // assuming that we have material.diffuseN and material.specularN in our shaders N >= 1
        const Texture texture = textures[i];
        string type_name;
        switch(texture.type) {
            case TextureType::DIFFUSE:
                type_name = "diffuse";
                break;
            case TextureType::SPECULAR:
                type_name = "specular";
                break;
            default:
                std::cout << "Texture type not supported" << std::endl;
                return;
        }
        string uniform_texture_name = "material." + type_name + std::to_string(specularNr++);

        shader.setInt(uniform_texture_name, i);  // I'm really unsure about it
        glBindTexture(GL_TEXTURE_2D, texture.id);
    }
    glActiveTexture(GL_TEXTURE0);

    // draw mesh
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);

}


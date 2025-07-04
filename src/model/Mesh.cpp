//
// Created by KUCIA on 23.06.2025.
//

#include "Mesh.h"

#include <iostream>
#include <ostream>
#include <glad/glad.h>
#include <string>

using std::string;

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures, MaterialProperties matProps) {
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;
    this->materialProps = matProps;

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
        // vertex tangent
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(offsetof(Vertex, Tangent)));
        // vertex bitangent
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(offsetof(Vertex, Bitangent)));

    // unbinding buffers
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::draw(const Shader &shader) const {
    unsigned int diffuseNr = 1, specularNr = 1, normalNr = 1, heightNr = 1;

    // Set material properties
    shader.setVec3("materialColor.diffuse", materialProps.diffuseColor);
    shader.setVec3("materialColor.specular", materialProps.specularColor);
    shader.setVec3("materialColor.ambient", materialProps.ambientColor);
    shader.setFloat("materialColor.shininess", materialProps.shininess);

    for(unsigned int i=0; i < textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);

        // retrieve texture number
        // assuming that we have material.diffuseN and material.specularN in our shaders N >= 1
        const Texture texture = textures[i];
        string type_name;
        string uniform_texture_name;
        switch(texture.type) {
            case TextureType::DIFFUSE:
                uniform_texture_name = "material.diffuse" + std::to_string(diffuseNr++);
                break;
            case TextureType::SPECULAR:
                uniform_texture_name = "material.specular" + std::to_string(specularNr++);
                break;
            case TextureType::NORMAL:
                uniform_texture_name = "material.normal" + std::to_string(normalNr++);
                break;
            case TextureType::HEIGHT:
                uniform_texture_name = "material.height" + std::to_string(heightNr++);
                break;
            default:
                std::cout << "Texture type not supported" << std::endl;
                continue;
        }

        shader.setInt(uniform_texture_name, i);
        glBindTexture(GL_TEXTURE_2D, texture.id);
    }
    
    // Set flag to indicate if we're using textures or material colors
    shader.setBool("useTextures", !textures.empty() && textures[0].path != "default_white");
    
    glActiveTexture(GL_TEXTURE0);

    // draw mesh
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}
//
// Created by KUCIA on 22.06.2025.
//

#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include <string>

#include "assimp/scene.h"
#include "assimp/material.h"
#include "assimp/mesh.h"

#include "LightSource.h"
#include "Camera.h"
#include "Texture.h"
#include "Mesh.h"

class Model {
public:
    Model() = default;
    Model(const char *path);

    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 rotation_deg = glm::vec3(0.0f);
    glm::vec3 scale = glm::vec3(1.0f);

    float material_shininess = 32.0f;

    void draw(const Shader &shader, const Camera &camera, const LightSource &lightSource) const;

private:
    std::vector<Texture> loaded_textures;
    std::vector<Mesh> meshes;
    std::string directory;
    std::string path;

    glm::mat4 getModelMatrix() const;

    void processNode(const aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> loadMaterialTextures(const aiMaterial *mat, aiTextureType assimp_type, TextureType texture_type);

};

#endif //OBJECT_H

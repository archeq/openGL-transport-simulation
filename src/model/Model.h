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

#include "Texture.h"
#include "Mesh.h"

class Model {
public:
    Model() = default;
    Model(const char *path);

    void draw(const Shader &shader);

private:
    std::vector<Texture> loaded_textures;
    std::vector<Mesh> meshes;
    std::string directory;
    std::string path;

    void processNode(const aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> loadMaterialTextures(const aiMaterial *mat, aiTextureType assimp_type, TextureType texture_type);

};

#endif //OBJECT_H


//
// Created by KUCIA on 22.06.2025.
//

#include "Model.h"

#include <iostream>

#include "Camera.h"
#include "assimp/Importer.hpp"
#include "assimp/mesh.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "LightSource.h"
#include "glm/trigonometric.hpp"
#include "glm/gtx/transform.hpp"
#include <glad/glad.h>


Model::Model(const char *path) {
    this->path = std::string(path);  // меня за это сожгут в аду :))))))) MAKAVELIGODD

    // reading the file
    Assimp::Importer importer;
    auto flags = aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace;
    const aiScene* scene = importer.ReadFile(path, flags);
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
        _is_loaded = false;
        return;
    }

    // retrieve the directory path of the filepath
    directory = this->path.substr(0, this->path.find_last_of('/'));

    // process ASSIMP's root node recursively
    processNode(scene->mRootNode, scene);
    _is_loaded = true;
}

void Model::draw(const Shader &shader, const Camera &camera, const LightSource &lightSource) const {
    if (!is_loaded() || meshes.empty()) return;
    shader.use();

    // uniforms
    shader.setMat4("model", getModelMatrix());
    shader.setMat4("view", camera.getViewMatrix());
    shader.setMat4("projection", camera.getProjectionMatrix());
    shader.setVec3("viewPos", camera.position);
    shader.setFloat("material.shininess", material_shininess);
    shader.setVec3("lightPos", lightSource.position);
    shader.setVec3("light.diffuse", lightSource.diffuse);
    shader.setVec3("light.ambient", lightSource.ambient);
    shader.setVec3("light.specular", lightSource.specular);

    // merely drawing all meshes the model consists of
    for (auto &mesh : meshes) {
        mesh.draw(shader);
    }
}

void Model::processNode(const aiNode *node, const aiScene *scene) {
    // process all the node's meshes (if any)
    for(unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }

    // then do the same for each of its children
    for(unsigned int i = 0; i < node->mNumChildren; i++)
        processNode(node->mChildren[i], scene);
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene) {

    // data to fill
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    for (int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;
        glm::vec3 vector;

        // positions
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;

        // normals
        if (mesh->HasNormals())
        {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;
        }
        // texture coordinates
        if(mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            glm::vec2 vec;
            // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't
            // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;

            // tangent
            vector.x = mesh->mTangents[i].x;
            vector.y = mesh->mTangents[i].y;
            vector.z = mesh->mTangents[i].z;
            vertex.Tangent = vector;

            // bitangent
            vector.x = mesh->mBitangents[i].x;
            vector.y = mesh->mBitangents[i].y;
            vector.z = mesh->mBitangents[i].z;
            vertex.Bitangent = vector;
        }
        else
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);

        vertices.push_back(vertex);
    }

    // indices
    for(unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for(unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    // process materials
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

    // Store material properties
    MaterialProperties matProps;

    // Get material colors
    aiColor3D color;
    if (material->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS) {
        matProps.diffuseColor = glm::vec3(color.r, color.g, color.b);
    }
    if (material->Get(AI_MATKEY_COLOR_SPECULAR, color) == AI_SUCCESS) {
        matProps.specularColor = glm::vec3(color.r, color.g, color.b);
    }
    if (material->Get(AI_MATKEY_COLOR_AMBIENT, color) == AI_SUCCESS) {
        matProps.ambientColor = glm::vec3(color.r, color.g, color.b);
    }

    float shininess;
    if (material->Get(AI_MATKEY_SHININESS, shininess) == AI_SUCCESS) {
        matProps.shininess = shininess;
    }

    // 1. diffuse maps
    std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, TextureType::DIFFUSE);
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    // 2. specular maps
    std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, TextureType::SPECULAR);
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    // 3. normal maps
    std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, TextureType::NORMAL);
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    // 4. height maps
    std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, TextureType::HEIGHT);
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

    // If no textures are found, create a default white texture for compatibility
    if (textures.empty()) {
        // Create default white texture if none exists
        if (defaultWhiteTexture.id == 0) {
            createDefaultTexture();
        }

        Texture defaultTex = defaultWhiteTexture;
        defaultTex.type = TextureType::DIFFUSE;
        textures.push_back(defaultTex);
    }

    // return a mesh object created from the extracted mesh data
    return {vertices, indices, textures, matProps};
}

std::vector<Texture> Model::loadMaterialTextures(const aiMaterial *mat, const aiTextureType assimp_type, const TextureType texture_type) {
    std::vector<Texture> textures;
    // std::cout << "Loading " << mat->GetTextureCount(assimp_type) << " textures of type " << (int)assimp_type << std::endl;

    for (int i = 0; i < mat->GetTextureCount(assimp_type); i++) {
        aiString str;
        mat->GetTexture(assimp_type, i, &str);
        std::string path = directory + "/" + str.C_Str();

        // checking if the texture is already loaded
        bool is_loaded = false;
        for(auto & loaded_texture : loaded_textures) {
            if(path == loaded_texture.path) {
                textures.push_back(loaded_texture);
                is_loaded = true;
                break;
            }
        }

        // loading if not
        if (!is_loaded) {
            try {
                // std::cout << "Creating new texture: " << path << std::endl;
                Texture texture(path);
                texture.type = texture_type;
                textures.push_back(texture);
                loaded_textures.push_back(texture);
                // std::cout << "Successfully loaded texture: " << path << std::endl;
            } catch (const std::exception& e) {
                std::cout << "Failed to load texture: " << path << " - " << e.what() << std::endl;
            } catch (...) {
                std::cout << "Failed to load texture: " << path << " - Unknown error" << std::endl;
            }
        }
    }

    return textures;
}

glm::mat4 Model::getModelMatrix() const {
    auto model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::rotate(model, glm::radians(rotation_deg.z), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::rotate(model, glm::radians(rotation_deg.y), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotation_deg.x), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::scale(model, scale);
    return model;
}

void Model::createDefaultTexture() {
    glGenTextures(1, &defaultWhiteTexture.id);
    glBindTexture(GL_TEXTURE_2D, defaultWhiteTexture.id);

    // Create 1x1 white pixel
    unsigned char white[] = {255, 255, 255, 255};
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, white);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    defaultWhiteTexture.type = TextureType::DIFFUSE;
    defaultWhiteTexture.path = "default_white";
}

bool Model::is_loaded() const {
    return _is_loaded;
}
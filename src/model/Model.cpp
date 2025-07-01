//
// Created by KUCIA on 22.06.2025.
//

#include "Model.h"

#include <iostream>

#include "assimp/Importer.hpp"
#include "assimp/mesh.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

Model::Model(const char *path) {
    this->path = std::string(path);  // меня за это сожгут в аду :))))))) MAKAVELIGODD

    // reading the file
    Assimp::Importer importer;
    auto flags = aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs;
    const aiScene* scene = importer.ReadFile(path, flags);
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
        return;
    }

    // retrieve the directory path of the filepath
    directory = this->path.substr(0, this->path.find_last_of('/'));

    // process ASSIMP's root node recursively
    processNode(scene->mRootNode, scene);
}

void Model::draw(const Shader &shader) {
    // merely drawing all meshes the model consists of
    for (auto &mesh : meshes) {
        mesh.draw(shader);
    }
}

void Model::processNode(const aiNode *node, const aiScene *scene) {
    // process all the node’s meshes (if any)
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

            // // tangent
            // vector.x = mesh->mTangents[i].x;
            // vector.y = mesh->mTangents[i].y;
            // vector.z = mesh->mTangents[i].z;
            // vertex.Tangent = vector;

            // // bitangent
            // vector.x = mesh->mBitangents[i].x;
            // vector.y = mesh->mBitangents[i].y;
            // vector.z = mesh->mBitangents[i].z;
            // vertex.Bitangent = vector;
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

    // 1. diffuse maps
    std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, TextureType::DIFFUSE);
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    // 2. specular maps
    std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, TextureType::SPECULAR);
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    // // 3. normal maps // TODO: use normal maps
    // std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, TextureType::Normal);
    // textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    // // 4. height maps
    // std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, TextureType::Height);
    // textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

    // return a mesh object created from the extracted mesh data
    return {vertices, indices, textures};
}

std::vector<Texture> Model::loadMaterialTextures(const aiMaterial *mat, const aiTextureType assimp_type, const TextureType texture_type) {
    std::vector<Texture> textures;
    for (int i = 0; i < mat->GetTextureCount(assimp_type); i++) {
        aiString str;
        mat->GetTexture(assimp_type, i, &str);
        std::string path = directory + "/" + str.C_Str();

        Texture texture(path);
        texture.type = texture_type;
        textures.push_back(texture);

    }

    return textures;
}
//
// Created by KUCIA on 22.06.2025.
//

#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

enum TextureType {
    DIFFUSE,
    SPECULAR,
    NORMAL,
    HEIGHT,
    METALNESS,
    ROUGHNESS,
    AMBIENT,
    EMISSIVE,
};

class Texture {
public:

    Texture() {};
    Texture(const std::string &path, int width, int height);

    unsigned int id = NULL;
    TextureType type = TextureType::DIFFUSE;
};

#endif //TEXTURE_H

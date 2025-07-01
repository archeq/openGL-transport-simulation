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
};

class Texture {
public:

    Texture() = default;
    explicit Texture(const std::string &path);

    unsigned int id = NULL;
    TextureType type = TextureType::DIFFUSE;
    std::string path;
};

#endif //TEXTURE_H

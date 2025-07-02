//
// Created by KUCIA on 22.06.2025.
//

#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <iostream>
#include <ostream>
#include <stb/stb_image.h>
#include <glad/glad.h>

Texture::Texture(const std::string &path) {

    // loading the image
    int width, height, channels;
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &channels, 0);

    if (!data) {
        std::cout << "ERROR::TEXTURE:: Failed to load the texture " << path << std::endl;
        return;
    }

    // color format
    GLenum format;
    if (channels == 1)
        format = GL_RED;
    else if (channels == 3)
        format = GL_RGB;
    else if (channels == 4)
        format = GL_RGBA;

    // generating new texture
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    // setting the texture wrapping/filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // binding the texture
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    // saving the path
    this->path = path;

    // freeing the memory
    stbi_image_free(data);

}

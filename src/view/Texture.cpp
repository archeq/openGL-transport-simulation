//
// Created by KUCIA on 22.06.2025.
//

#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <iostream>
#include <ostream>
#include <stb/stb_image.h>
#include <glad/glad.h>

Texture::Texture(const std::string &path, int width, int height) {

    // loading the image
    unsigned char *data = stbi_load(path.c_str(), &width, &height, nullptr, 3);

    if (!data) {
        std::cout << "Failed to load the texture" << std::endl;
        return;
    }

    // generating new texture
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    // setting the texture wrapping/filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // binding the texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    // freeing the memory
    stbi_image_free(data);

}

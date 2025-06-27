//
// Created by KUCIA on 22.06.2025.
//

#ifndef SHADER_H
#define SHADER_H

#include <stddef.h>
#include <string>


class Shader {
public:

    unsigned int id = NULL;
    Shader(const char *vertexPath, const char *fragmentPath);
    void use() const;

    // uniform management
    void setBool(const std::string &name, const bool value) const;
    void setInt(const std::string &name, const int value) const;
    void setFloat(const std::string &name, const float value) const;

};



#endif //SHADER_H

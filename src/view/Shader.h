//
// Created by KUCIA on 22.06.2025.
//

#ifndef SHADER_H
#define SHADER_H
#include <stddef.h>


class Shader {
public:

    unsigned int id = NULL;
    Shader(const char *vertexPath, const char *fragmentPath);
    void use() const;

    // uniform management
    void setBool(const char *name, bool value);
    void setInt(const char *name, int value);
    void setFloat(const char *name, float value);

};



#endif //SHADER_H

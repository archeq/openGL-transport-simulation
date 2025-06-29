//
// Created by KUCIA on 22.06.2025.
//

#ifndef SHADER_H
#define SHADER_H

#include <stddef.h>
#include <string>

#include "glm/fwd.hpp"


class Shader {
public:

    Shader() {};  // default constructor for declarations
    Shader(const char *vertexPath, const char *fragmentPath);
    void use() const;

    // uniform management
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setVec2(const std::string &name, const glm::vec2 &value) const;
    void setVec3(const std::string &name, const glm::vec3 &value ) const;
    void setMat4(const std::string &name, const glm::mat4 &value) const;

private:
    unsigned int id = NULL;
};



#endif //SHADER_H

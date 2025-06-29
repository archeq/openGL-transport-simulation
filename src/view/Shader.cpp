//
// Created by KUCIA on 22.06.2025.
//

#include "Shader.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include "glad/glad.h"

Shader::Shader(const char *vertexPath, const char *fragmentPath) {

    // loading shaders' data
    std::ifstream vShaderFile, fShaderFile;
    std::string vertexCode, fragmentCode;
    vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try {
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);

        std::stringstream vShaderStream, fShaderStream;
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        vShaderFile.close();
        fShaderFile.close();

        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure e) {
        std::cout << "ERROR::SHADER failed to read shaders:\n";
        std::cout << e.what() << std::endl;
        return;
    }

    // compiling shaders
    const char *vShaderCode = vertexCode.c_str();
    const char *fShaderCode = fragmentCode.c_str();
    unsigned int vertex, fragment;
    int success;

    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, nullptr);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[1024];
        glGetShaderInfoLog(vertex, 1024, nullptr, infoLog);
        std::cout << "ERROR::SHADER::VERTEX:: failed to compile vertex shader:\n";
        std::cout << infoLog << std::endl;
        return;
    }

    // fragment shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, nullptr);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[1024];
        glGetShaderInfoLog(fragment, 1024, nullptr, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT:: failed to compile fragment shader:\n";
        std::cout << infoLog << std::endl;
        return;
    }

    // shader program
    id = glCreateProgram();
    glAttachShader(id, vertex);
    glAttachShader(id, fragment);
    glLinkProgram(id);
    glDeleteShader(vertex);  // removing shader after they were linked
    glDeleteShader(fragment);
    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[1024];
        glGetProgramInfoLog(id, 1024, nullptr, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM:: failed to link shader program:\n";
        std::cout << infoLog << std::endl;
        return;
    }

}

void Shader::use() const {
    glUseProgram(id);
}

void Shader::setBool(const std::string &name, const bool value) const {
    glUniform1i(glGetUniformLocation(id, name.c_str()), static_cast<int>(value));
}

void Shader::setInt(const std::string &name, const int value) const {
    glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, const float value) const {
    glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::setVec2(const std::string &name, const glm::vec2 &value) const {
    glUniform2fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
}

void Shader::setVec3(const std::string &name, const glm::vec3 &value) const {
    glUniform3fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
}


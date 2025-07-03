//
// Created by arche on 3.07.2025.
//

#ifndef TUNNEL_H
#define TUNNEL_H

// Tunnel.h
#pragma once
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "LightSource.h"
#include "Texture.h"

class Tunnel {
private:
    std::unique_ptr<Mesh> tunnelMesh;
    std::vector<glm::vec3> pathPoints;
    float radius;
    int segments;

    void generateTunnelGeometry();

public:
    Tunnel();
    ~Tunnel() = default;

    // Запрещаем копирование
    Tunnel(const Tunnel&) = delete;
    Tunnel& operator=(const Tunnel&) = delete;

    // Разрешаем перемещение
    Tunnel(Tunnel&& other) noexcept;
    Tunnel& operator=(Tunnel&& other) noexcept;

    void initialize(const std::vector<glm::vec3>& pathPoints, float radius = 5.0f, int segments = 12);
    void draw(const Shader& shader, bool transparent = false);
    void loadTexture(const std::string& texturePath);
};
#endif //TUNNEL_H

//
// Created by arche on 3.07.2025.
//

#define GLM_ENABLE_EXPERIMENTAL
#include "Tunnel.h"
#include <iostream>
#include <cmath>
#include <glm/gtc/constants.hpp>



Tunnel::Tunnel() : radius(5.0f), segments(12) {}

// Добавьте после существующего конструктора
Tunnel::Tunnel(Tunnel&& other) noexcept
    : tunnelMesh(std::move(other.tunnelMesh)),
      pathPoints(std::move(other.pathPoints)),
      radius(other.radius),
      segments(other.segments) {
}

Tunnel& Tunnel::operator=(Tunnel&& other) noexcept {
    if (this != &other) {
        tunnelMesh = std::move(other.tunnelMesh);
        pathPoints = std::move(other.pathPoints);
        radius = other.radius;
        segments = other.segments;
    }
    return *this;
}

void Tunnel::generateTunnelGeometry() {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    if (pathPoints.size() < 2) return;

    // Генерируем вершины туннеля
    for (size_t i = 0; i < pathPoints.size(); i++) {
        glm::vec3 forward;

        // Вычисляем направление движения
        if (i == 0) {
            forward = glm::normalize(pathPoints[i + 1] - pathPoints[i]);
        } else if (i == pathPoints.size() - 1) {
            forward = glm::normalize(pathPoints[i] - pathPoints[i - 1]);
        } else {
            forward = glm::normalize(pathPoints[i + 1] - pathPoints[i - 1]);
        }

        // Создаем локальную систему координат
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 right = glm::normalize(glm::cross(forward, up));
        up = glm::cross(right, forward);

        // Генерируем кольцо вершин
        for (int j = 0; j < segments; j++) {
            float angle = 2.0f * glm::pi<double>() * j / segments;
            glm::vec3 circlePos = cos(angle) * right + sin(angle) * up;
            glm::vec3 position = pathPoints[i] + radius * circlePos;

            Vertex vertex;
            vertex.Position = position;
            vertex.Normal = -circlePos; // Нормаль направлена внутрь туннеля
            vertex.TexCoords = glm::vec2((float)j / segments, (float)i / (pathPoints.size() - 1));
            vertex.Tangent = glm::vec3(0.0f); // Можно вычислить, если нужно
            vertex.Bitangent = glm::vec3(0.0f); // Можно вычислить, если нужно

            vertices.push_back(vertex);
        }
    }

    // Генерируем индексы для соединения колец
    for (size_t i = 0; i < pathPoints.size() - 1; i++) {
        for (int j = 0; j < segments; j++) {
            int current = i * segments + j;
            int next = i * segments + (j + 1) % segments;
            int currentNext = (i + 1) * segments + j;
            int nextNext = (i + 1) * segments + (j + 1) % segments;

            // Два треугольника для каждого квада
            indices.push_back(current);
            indices.push_back(next);
            indices.push_back(currentNext);

            indices.push_back(next);
            indices.push_back(nextNext);
            indices.push_back(currentNext);
        }
    }

    // Создаем пустой вектор текстур (текстуры загружаются отдельно)
    std::vector<Texture> textures;

    // Создаем материал для туннеля
    MaterialProperties material;
    material.diffuseColor = glm::vec3(0.7f, 0.7f, 0.7f);  // Серый цвет
    material.specularColor = glm::vec3(0.2f, 0.2f, 0.2f); // Слабый блеск
    material.ambientColor = glm::vec3(0.1f, 0.1f, 0.1f);  // Темный ambient
    material.shininess = 16.0f;

    // Создаем меш
    tunnelMesh = std::make_unique<Mesh>(vertices, indices, textures, material);
}

void Tunnel::initialize(const std::vector<glm::vec3>& pathPoints, float radius, int segments) {
    this->pathPoints = pathPoints;
    this->radius = radius;
    this->segments = segments;

    generateTunnelGeometry();

    std::cout << "Tunnel initialized with " << pathPoints.size() << " points, radius: "
              << radius << ", segments: " << segments << std::endl;
}

void Tunnel::draw(const Shader& shader, bool transparent) {
    if (!tunnelMesh) {
        std::cout << "Tunnel mesh is not initialized!" << std::endl;
        return;
    }

    // Настройка прозрачности
    if (transparent) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDepthMask(GL_FALSE);

        // Устанавливаем альфа для прозрачности
        shader.setFloat("alpha", 0.3f);
    } else {
        shader.setFloat("alpha", 1.0f);
    }

    // Рисуем туннель
    tunnelMesh->draw(shader);

    // Восстанавливаем состояние OpenGL
    if (transparent) {
        glDepthMask(GL_TRUE);
        glDisable(GL_BLEND);
    }
}

void Tunnel::loadTexture(const std::string& texturePath) {
    if (!tunnelMesh) {
        std::cout << "Cannot load texture: tunnel mesh is not initialized!" << std::endl;
        return;
    }

    try {
        // Создаем новую текстуру
        Texture tunnelTexture(texturePath);
        tunnelTexture.type = TextureType::DIFFUSE;

        // Добавляем текстуру к мешу
        tunnelMesh->textures.clear(); // Очищаем старые текстуры
        tunnelMesh->textures.push_back(tunnelTexture);

        std::cout << "Tunnel texture loaded: " << texturePath << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Failed to load tunnel texture: " << e.what() << std::endl;
    }
}

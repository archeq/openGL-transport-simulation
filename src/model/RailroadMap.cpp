//
// Created by arche on 1.07.2025.
//
#define GLM_ENABLE_EXPERIMENTAL
#include "RailroadMap.h"
#include <glad/glad.h>
#include <iostream>
#include <glm/gtx/vector_angle.hpp>

RailroadMap::RailroadMap(const std::vector<std::vector<glm::vec3>>& routePoints) {
    for (const auto& route : routePoints) {
        // Проверяем, достаточно ли точек для сплайна
        if (route.size() >= 2) {
            // Создаем сплайн для маршрута
            routes.emplace_back(route);

            // Сохраняем станции
            for (const auto& point : route) {
                stations.push_back(point);
            }
        }
    }

    createRailsMesh();
    createStationsMesh();
}

RailroadMap::~RailroadMap() {
    glDeleteVertexArrays(1, &railVAO);
    glDeleteVertexArrays(1, &stationVAO);
}

bool RailroadMap::loadTextures(const std::string& railTexturePath, const std::string& stationTexturePath) {
    try {
        Texture railTex(railTexturePath);
        Texture stationTex(stationTexturePath);

        railTextureID = railTex.id;
        stationTextureID = stationTex.id;
        std::cout << "Rail texture ID: " << railTextureID << std::endl;
        std::cout << "Station texture ID: " << stationTextureID << std::endl;
        if (railTextureID == 0) {
            std::cout << "Failed to load rail texture: " << railTexturePath << std::endl;
            return false;
        }

        if (stationTextureID == 0) {
            std::cout << "Failed to load station texture: " << stationTexturePath << std::endl;
            return false;
        }

        return true;
    } catch (const std::exception& e) {
        std::cout << "Exception during texture loading: " << e.what() << std::endl;
        return false;
    }

}

void RailroadMap::createRailsMesh() {
    std::vector<float> vertices;
    const float railWidth = 0.2f;

    // Для каждого маршрута
    for (size_t routeIdx = 0; routeIdx < routes.size(); ++routeIdx) {
        const auto& route = routes[routeIdx];

        // Для каждого сегмента сплайна
        for (int segment = 0; segment < route.getSegmentCount(); ++segment) {
            // Создаем точки вдоль сегмента сплайна
            const int steps = 50;
            for (int i = 0; i < steps; ++i) {
                float t = i / (float)steps;
                float nextT = (i + 1) / (float)steps;

                glm::vec3 pos = route.getPoint(segment, t);
                glm::vec3 nextPos = route.getPoint(segment, nextT);

                // Получаем направление для создания ширины рельса
                glm::vec3 direction = route.getDirection(segment, t);
                glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

                // Если направление почти вертикально, используем другой вектор для cross
                if (glm::length(glm::cross(direction, up)) < 0.001f) {
                    up = glm::vec3(0.0f, 0.0f, 1.0f);
                }

                glm::vec3 right = glm::normalize(glm::cross(direction, up)) * railWidth;

                // Левая сторона рельса - ПОДНИМАЕМ НА 0.01f
                glm::vec3 left = pos - right;
                left.y += 0.01f; // Поднимаем рельсы над поверхностью

                // Правая сторона рельса - ПОДНИМАЕМ НА 0.01f
                glm::vec3 right_pos = pos + right;
                right_pos.y += 0.01f;

                // Получаем направление для следующей точки
                glm::vec3 nextDirection = route.getDirection(segment, nextT);
                if (glm::length(glm::cross(nextDirection, up)) < 0.001f) {
                    up = glm::vec3(0.0f, 0.0f, 1.0f);
                }

                glm::vec3 nextRight = glm::normalize(glm::cross(nextDirection, up)) * railWidth;
                glm::vec3 nextLeft = nextPos - nextRight;
                nextLeft.y += 0.01f; // Поднимаем рельсы над поверхностью
                glm::vec3 nextRightPos = nextPos + nextRight;
                nextRightPos.y += 0.01f;

                // Нормаль направлена вверх для всех вершин рельса
                glm::vec3 normal = glm::vec3(0.0f, 1.0f, 0.0f);

                // Добавляем два треугольника для квада
                // Треугольник 1
                vertices.push_back(left.x);
                vertices.push_back(left.y);
                vertices.push_back(left.z);
                vertices.push_back(normal.x);
                vertices.push_back(normal.y);
                vertices.push_back(normal.z);
                vertices.push_back(t * 5.0f);
                vertices.push_back(0.0f);

                vertices.push_back(right_pos.x);
                vertices.push_back(right_pos.y);
                vertices.push_back(right_pos.z);
                vertices.push_back(normal.x);
                vertices.push_back(normal.y);
                vertices.push_back(normal.z);
                vertices.push_back(t * 5.0f);
                vertices.push_back(1.0f);

                vertices.push_back(nextLeft.x);
                vertices.push_back(nextLeft.y);
                vertices.push_back(nextLeft.z);
                vertices.push_back(normal.x);
                vertices.push_back(normal.y);
                vertices.push_back(normal.z);
                vertices.push_back(nextT * 5.0f);
                vertices.push_back(0.0f);

                // Треугольник 2
                vertices.push_back(right_pos.x);
                vertices.push_back(right_pos.y);
                vertices.push_back(right_pos.z);
                vertices.push_back(normal.x);
                vertices.push_back(normal.y);
                vertices.push_back(normal.z);
                vertices.push_back(t * 5.0f);
                vertices.push_back(1.0f);

                vertices.push_back(nextRightPos.x);
                vertices.push_back(nextRightPos.y);
                vertices.push_back(nextRightPos.z);
                vertices.push_back(normal.x);
                vertices.push_back(normal.y);
                vertices.push_back(normal.z);
                vertices.push_back(nextT * 5.0f);
                vertices.push_back(1.0f);

                vertices.push_back(nextLeft.x);
                vertices.push_back(nextLeft.y);
                vertices.push_back(nextLeft.z);
                vertices.push_back(normal.x);
                vertices.push_back(normal.y);
                vertices.push_back(normal.z);
                vertices.push_back(nextT * 5.0f);
                vertices.push_back(0.0f);
            }
        }
    }

    railVerticesCount = vertices.size() / 8; // Изменено с 5 на 8 из-за добавления нормалей

    // OpenGL буферы
    unsigned int VBO;
    glGenVertexArrays(1, &railVAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(railVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // Позиция
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Нормаль
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Текстурные координаты
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    std::cout << "Rail vertices count: " << railVerticesCount << ", VAO: " << railVAO << std::endl;

    glBindVertexArray(0);
}



void RailroadMap::draw_station_boxes(const Shader& shader, unsigned int cubeVAO, unsigned int diffuseTextureID, unsigned int specularTextureID) {
    // Настраиваем шейдер
    shader.use();

    // Привязываем текстуры
    shader.setInt("material.diffuse", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuseTextureID);

    shader.setInt("material.specular", 1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, specularTextureID);

    // Отрисовываем куб для каждой станции
    for (const auto& station : stations) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, station);
        model = glm::translate(model, glm::vec3(0.0f, 0.2f, 0.0f));
        model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f)); // Размер коробки

        shader.setMat4("model", model);

        // Отрисовка куба (используем существующий VAO из app.cpp)
        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}


void RailroadMap::createStationsMesh() {
    std::vector<float> vertices;

    const float stationSize = 0.2f;

    for (const auto& station : stations) {
        // Верхний левый
        vertices.push_back(station.x - stationSize);
        vertices.push_back(station.y);
        vertices.push_back(station.z - stationSize);
        vertices.push_back(0.0f); // u-текстурная координата
        vertices.push_back(1.0f); // v-текстурная координата

        // Нижний левый
        vertices.push_back(station.x - stationSize);
        vertices.push_back(station.y);
        vertices.push_back(station.z + stationSize);
        vertices.push_back(0.0f); // u-текстурная координата
        vertices.push_back(0.0f); // v-текстурная координата

        // Нижний правый
        vertices.push_back(station.x + stationSize);
        vertices.push_back(station.y);
        vertices.push_back(station.z + stationSize);
        vertices.push_back(1.0f); // u-текстурная координата
        vertices.push_back(0.0f); // v-текстурная координата

        // Верхний левый
        vertices.push_back(station.x - stationSize);
        vertices.push_back(station.y);
        vertices.push_back(station.z - stationSize);
        vertices.push_back(0.0f); // u-текстурная координата
        vertices.push_back(1.0f); // v-текстурная координата

        // Нижний правый
        vertices.push_back(station.x + stationSize);
        vertices.push_back(station.y);
        vertices.push_back(station.z + stationSize);
        vertices.push_back(1.0f); // u-текстурная координата
        vertices.push_back(0.0f); // v-текстурная координата

        // Верхний правый
        vertices.push_back(station.x + stationSize);
        vertices.push_back(station.y);
        vertices.push_back(station.z - stationSize);
        vertices.push_back(1.0f); // u-текстурная координата
        vertices.push_back(1.0f); // v-текстурная координата
    }

    stationVerticesCount = vertices.size() / 5;

    unsigned int VBO;
    glGenVertexArrays(1, &stationVAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(stationVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // Позиция
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Текстурные координаты
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void RailroadMap::draw_rails() {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, railTextureID);

    glBindVertexArray(railVAO);
    glDrawArrays(GL_TRIANGLES, 0, railVerticesCount);
    glBindVertexArray(0);
}

void RailroadMap::draw_stations() {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, stationTextureID);

    glBindVertexArray(stationVAO);
    glDrawArrays(GL_TRIANGLES, 0, stationVerticesCount);
    glBindVertexArray(0);
}


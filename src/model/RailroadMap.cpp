//
// Created by arche on 1.07.2025.
//
#define GLM_ENABLE_EXPERIMENTAL
#include "RailroadMap.h"
#include <glad/glad.h>
#include <iostream>
#include <glm/gtx/vector_angle.hpp>


#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

double piD = glm::pi<double>();



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


void RailroadMap::setPoints(const std::vector<glm::vec3>& points) {
    allPoints = points;
}




void RailroadMap::addRoute(const std::vector<int>& pointIndices) {
    // Проверяем валидность индексов
    for (int index : pointIndices) {
        if (index < 0 || index >= static_cast<int>(allPoints.size())) {
            std::cout << "Invalid point index: " << index << " (total points: " << allPoints.size() << ")" << std::endl;
            return;
        }
    }

    routeIndices.push_back(pointIndices);

    // Создаем сплайн из точек по индексам
    std::vector<glm::vec3> routePoints;
    for (int index : pointIndices) {
        routePoints.push_back(allPoints[index]);
    }

    if (routePoints.size() >= 2) {
        routes.emplace_back(routePoints);
        std::cout << "Route " << (routes.size() - 1) << " created with " << routePoints.size() << " points" << std::endl;
    } else {
        std::cout << "ERROR: Route needs at least 2 points, got " << routePoints.size() << std::endl;
    }
}




void RailroadMap::initialize(const std::vector<glm::vec3>& points, const std::vector<std::vector<int>>& routeIndexArrays) {
    routes.clear();
    stations.clear();
    routeIndices.clear();

    setPoints(points);
    std::cout << "Initialized with " << points.size() << " points" << std::endl;

    for (size_t i = 0; i < routeIndexArrays.size(); ++i) {
        const auto& routeIdx = routeIndexArrays[i];
        std::cout << "Adding route " << i << " with " << routeIdx.size() << " point indices" << std::endl;

        addRoute(routeIdx);

        // Добавляем станции из этого маршрута
        for (int index : routeIdx) {
            if (index >= 0 && index < static_cast<int>(allPoints.size())) {
                stations.push_back(allPoints[index]);
            }
        }
    }

    std::cout << "Total routes created: " << routes.size() << std::endl;

    createRailsMesh();
    createStationsMesh();
}




RailroadMap::~RailroadMap() = default;

bool RailroadMap::loadTextures(const std::string& railTexturePath, const std::string& stationTexturePath) {
    try {
        Texture railTex(railTexturePath);
        Texture stationTex(stationTexturePath);
        railTextureID = railTex.id;
        stationTextureID = stationTex.id;
        std::cout << "Rail texture ID: " << railTextureID << std::endl;
        std::cout << "Station texture ID: " << stationTextureID << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cout << "Exception during texture loading: " << e.what() << std::endl;
        return false;
    }

}

void RailroadMap::createRailsMesh() {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
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

                // Добавляем вершины
                Vertex v1, v2, v3, v4;

                // Первый треугольник
                v1.Position = left;
                v1.Normal = normal;
                v1.TexCoords = glm::vec2(t * 5.0f, 0.0f);

                v2.Position = right_pos;
                v2.Normal = normal;
                v2.TexCoords = glm::vec2(t * 5.0f, 1.0f);

                v3.Position = nextLeft;
                v3.Normal = normal;
                v3.TexCoords = glm::vec2(nextT * 5.0f, 0.0f);

                // Второй треугольник
                v4.Position = nextRightPos;
                v4.Normal = normal;
                v4.TexCoords = glm::vec2(nextT * 5.0f, 1.0f);

                // Добавляем вершины
                unsigned int indexOffset = vertices.size();

                vertices.push_back(v1);
                vertices.push_back(v2);
                vertices.push_back(v3);
                vertices.push_back(v2);
                vertices.push_back(v4);
                vertices.push_back(v3);

                // Добавляем индексы
                indices.push_back(indexOffset);
                indices.push_back(indexOffset + 1);
                indices.push_back(indexOffset + 2);
                indices.push_back(indexOffset + 3);
                indices.push_back(indexOffset + 4);
                indices.push_back(indexOffset + 5);
            }
        }
    }

    // Создаем текстуру для рельсов
    std::vector<Texture> textures;
    Texture railTexture;

    // Создаем меш из вершин, индексов и текстур
    railMesh = std::make_unique<Mesh>(vertices, indices, textures);
    railVerticesCount = vertices.size();

}



void RailroadMap::createStationsMesh() {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    const float stationSize = 0.2f;

    for (const auto& station : stations) {
        // Вершины квадрата для станции
        Vertex topLeft, bottomLeft, bottomRight, topRight;

        // Верхний левый угол
        topLeft.Position = glm::vec3(station.x - stationSize, station.y + 0.01f, station.z - stationSize);
        topLeft.Normal = glm::vec3(0.0f, 1.0f, 0.0f);
        topLeft.TexCoords = glm::vec2(0.0f, 1.0f);

        // Нижний левый угол
        bottomLeft.Position = glm::vec3(station.x - stationSize, station.y + 0.01f, station.z + stationSize);
        bottomLeft.Normal = glm::vec3(0.0f, 1.0f, 0.0f);
        bottomLeft.TexCoords = glm::vec2(0.0f, 0.0f);

        // Нижний правый угол
        bottomRight.Position = glm::vec3(station.x + stationSize, station.y + 0.01f, station.z + stationSize);
        bottomRight.Normal = glm::vec3(0.0f, 1.0f, 0.0f);
        bottomRight.TexCoords = glm::vec2(1.0f, 0.0f);

        // Верхний правый угол
        topRight.Position = glm::vec3(station.x + stationSize, station.y + 0.01f, station.z - stationSize);
        topRight.Normal = glm::vec3(0.0f, 1.0f, 0.0f);
        topRight.TexCoords = glm::vec2(1.0f, 1.0f);

        // Индексы для добавляемых вершин
        unsigned int indexOffset = vertices.size();

        // Добавляем вершины
        vertices.push_back(topLeft);
        vertices.push_back(bottomLeft);
        vertices.push_back(bottomRight);
        vertices.push_back(topRight);

        // Добавляем индексы для двух треугольников (квад)
        // Первый треугольник
        indices.push_back(indexOffset);
        indices.push_back(indexOffset + 1);
        indices.push_back(indexOffset + 2);

        // Второй треугольник
        indices.push_back(indexOffset);
        indices.push_back(indexOffset + 2);
        indices.push_back(indexOffset + 3);
    }

    // Создаем текстуру для станций
    std::vector<Texture> textures;
    Texture stationTexture;
    stationTexture.id = stationTextureID;
    stationTexture.type = TextureType::DIFFUSE;
    textures.push_back(stationTexture);

    // Создаем меш для станций
    stationMesh = std::make_unique<Mesh>(vertices, indices, textures);
    stationVerticesCount = vertices.size();

    std::cout << "Station vertices count: " << stationVerticesCount << std::endl;
}





void RailroadMap::createStationBoxMesh() {
    // Используем вершины куба из массива с 36 вершинами (как в app.cpp)
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    // Создаем вершины куба


    // Преобразуем массив в вектор вершин
    for (int i = 0; i < 36; i++) {
        Vertex vertex;
        vertex.Position = glm::vec3(
            cube_vertices_pnt[i * 8],
            cube_vertices_pnt[i * 8 + 1],
            cube_vertices_pnt[i * 8 + 2]
        );
        vertex.Normal = glm::vec3(
            cube_vertices_pnt[i * 8 + 3],
            cube_vertices_pnt[i * 8 + 4],
            cube_vertices_pnt[i * 8 + 5]
        );
        vertex.TexCoords = glm::vec2(
            cube_vertices_pnt[i * 8 + 6],
            cube_vertices_pnt[i * 8 + 7]
        );
        vertices.push_back(vertex);
        indices.push_back(i);
    }

    // Создаем текстуры для коробок станций
    std::vector<Texture> textures;

    // Диффузная текстура
    Texture diffuseTexture;
    diffuseTexture.id = boxDiffuseTextureID;
    diffuseTexture.type = TextureType::DIFFUSE;
    textures.push_back(diffuseTexture);

    // Спекулярная текстура
    Texture specularTexture;
    specularTexture.id = boxSpecularTextureID;
    specularTexture.type = TextureType::SPECULAR;
    textures.push_back(specularTexture);

    // Создаем меш
    stationBoxMesh = std::make_unique<Mesh>(vertices, indices, textures);
}


void RailroadMap::createStationSpheresMesh() {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    const int latitudeSegments = 16;  // Количество сегментов по широте
    const int longitudeSegments = 32; // Количество сегментов по долготе
    const float radius = 0.3f;        // Радиус сферы

    // Создаем вершины сферы
    for (int lat = 0; lat <= latitudeSegments; ++lat) {
        float theta = lat * piD / latitudeSegments; // От 0 до π
        float sinTheta = sin(theta);
        float cosTheta = cos(theta);

        for (int lon = 0; lon <= longitudeSegments; ++lon) {
            float phi = lon * 2 * piD / longitudeSegments; // От 0 до 2π
            float sinPhi = sin(phi);
            float cosPhi = cos(phi);

            Vertex vertex;
            // Позиция на единичной сфере
            vertex.Position = glm::vec3(
                radius * sinTheta * cosPhi,
                radius * cosTheta,
                radius * sinTheta * sinPhi
            );

            // Нормаль для сферы = нормализованная позиция
            vertex.Normal = glm::normalize(vertex.Position / radius);

            // Текстурные координаты (не используются, но нужны для совместимости)
            vertex.TexCoords = glm::vec2(
                (float)lon / longitudeSegments,
                (float)lat / latitudeSegments
            );

            vertices.push_back(vertex);
        }
    }

    // Создаем индексы для треугольников
    for (int lat = 0; lat < latitudeSegments; ++lat) {
        for (int lon = 0; lon < longitudeSegments; ++lon) {
            int current = lat * (longitudeSegments + 1) + lon;
            int next = current + longitudeSegments + 1;

            // Первый треугольник
            indices.push_back(current);
            indices.push_back(next);
            indices.push_back(current + 1);

            // Второй треугольник
            indices.push_back(current + 1);
            indices.push_back(next);
            indices.push_back(next + 1);
        }
    }

    // Создаем меш без текстур
    std::vector<Texture> textures;
    stationSphereMesh = std::make_unique<Mesh>(vertices, indices, textures);
}


void RailroadMap::draw_station_spheres(const Shader& shader) {
    if (!stationSphereMesh) {
        createStationSpheresMesh();
    }

    if (stationSphereMesh) {
        // Устанавливаем красный цвет материала
        shader.setVec3("material.diffuse_color", glm::vec3(1.0f, 0.0f, 0.0f)); // Красный цвет
        shader.setVec3("material.specular_color", glm::vec3(0.3f, 0.3f, 0.3f));
        shader.setFloat("material.shininess", 32.0f);

        // Отключаем использование текстур
        shader.setBool("use_texture", false);

        // Для каждой станции устанавливаем модельную матрицу и рисуем сферу
        for (const auto& station : stations) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, station);
            model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0f)); // Поднимаем над землей

            shader.setMat4("model", model);
            stationSphereMesh->draw(shader);
        }
    } else {
        std::cout << "Station sphere mesh is not initialized!" << std::endl;
    }
}



void RailroadMap::draw_rails(const Shader& shader) {
    if (railMesh) {
        // Включаем использование текстур
        shader.setBool("use_texture", true);

        // Устанавливаем текстуры рельсов
        shader.setInt("material.diffuse", 0);
        shader.setInt("material.specular", 1);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, railTextureID);

        // Если есть specular текстура, используем её, иначе diffuse
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, railTextureID); // Используем ту же текстуру для specular

        // Устанавливаем свойства материала
        shader.setFloat("material.shininess", 32.0f);

        railMesh->draw(shader);
    } else {
        std::cout << "Rail mesh is not initialized!" << std::endl;
    }
}

void RailroadMap::draw_stations(const Shader& shader) {
    if (stationMesh) {
        // Включаем использование текстур
        shader.setBool("use_texture", true);

        shader.setInt("material.diffuse", 0);
        shader.setInt("material.specular", 1);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, stationTextureID);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, stationTextureID);

        shader.setFloat("material.shininess", 32.0f);

        stationMesh->draw(shader);
    } else {
        std::cout << "Station mesh is not initialized!" << std::endl;
    }
}



void RailroadMap::draw_station_boxes(const Shader& shader) {
    if (!stationBoxMesh) {
        // Создаем меш для коробок станций, если он еще не создан
        if (!boxTexturesLoaded) {
            std::cout << "Station box textures are not loaded!" << std::endl;
            return;
        }
        createStationBoxMesh();
    }

    if (stationBoxMesh) {
        // Для каждой станции устанавливаем модельную матрицу и рисуем куб
        for (const auto& station : stations) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, station);
            model = glm::translate(model, glm::vec3(0.0f, 2.f, 0.0f));
            model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));

            shader.setMat4("model", model);
            stationBoxMesh->draw(shader);
        }
    } else {
        std::cout << "Station box mesh is not initialized!" << std::endl;
    }
}



bool RailroadMap::loadStationBoxTextures(const std::string& diffuseTexturePath, const std::string& specularTexturePath) {

    if (boxTexturesLoaded) {
        return true; // Текстуры уже загружены, ничего не делаем
    }

    try {
        Texture diffuseTex(diffuseTexturePath);
        Texture specularTex(specularTexturePath);
        boxDiffuseTextureID = diffuseTex.id;
        boxSpecularTextureID = specularTex.id;
        std::cout << "Box diffuse texture ID: " << boxDiffuseTextureID << std::endl;
        std::cout << "Box specular texture ID: " << boxSpecularTextureID << std::endl;
        boxTexturesLoaded = true; // Устанавливаем флаг в true после успешной загрузки
        return true;
    } catch (const std::exception& e) {
        std::cout << "Exception during box texture loading: " << e.what() << std::endl;
        return false;
    }
}

void RailroadMap::draw(const Shader &shader, const Camera &camera, const LightSource &lightSource) {
    shader.use();

    shader.setMat4("projection", camera.getProjectionMatrix());
    shader.setMat4("view", camera.getViewMatrix());
    shader.setMat4("model", glm::mat4(1.0f));
    // Установка uniform-переменных для освещения
    shader.setVec3("light.position", lightSource.position);
    shader.setVec3("viewPos", camera.position);
    shader.setVec3("light.ambient", lightSource.ambient);
    shader.setVec3("light.diffuse", lightSource.diffuse);
    shader.setVec3("light.specular", lightSource.specular);
    // material properties
    shader.setFloat("material.shininess", 32.0f);
    // Установка текстурного слота
    shader.setInt("material.diffuse", 0);
    // Отрисовка рельсов и станций
    draw_rails(shader);
    // draw_stations(shader);
    // draw_station_boxes(shader);
    draw_station_spheres(shader);
    
}
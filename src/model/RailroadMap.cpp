//
// Created by arche on 1.07.2025.
//
#define GLM_ENABLE_EXPERIMENTAL
#include "RailroadMap.h"
#include <glad/glad.h>
#include <iostream>
#include <glm/gtx/vector_angle.hpp>
#include <set>

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

    // Сначала создаем все маршруты
    for (size_t i = 0; i < routeIndexArrays.size(); ++i) {
        const auto& routeIdx = routeIndexArrays[i];
        addRoute(routeIdx);
    }

    // Затем добавляем уникальные станции (без дублирования)
    std::set<int> uniqueStationIndices;
    for (const auto& routeIdx : routeIndexArrays) {
        for (int index : routeIdx) {
            if (index >= 0 && index < static_cast<int>(allPoints.size())) {
                uniqueStationIndices.insert(index);
            }
        }
    }

    // Добавляем станции в порядке индексов
    for (int index : uniqueStationIndices) {
        stations.push_back(allPoints[index]);
    }

    std::cout << "Total routes created: " << routes.size() << std::endl;
    std::cout << "Total unique stations: " << stations.size() << std::endl;

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
    const float textureRepeat = 2.0f; // Коэффициент повторения текстуры

    // Для каждого маршрута
    for (size_t routeIdx = 0; routeIdx < routes.size(); ++routeIdx) {
        const auto& route = routes[routeIdx];
        float accumulatedDistance = 0.0f; // Накопленное расстояние для текстурных координат

        // Для каждого сегмента сплайна
        for (int segment = 0; segment < route.getSegmentCount(); ++segment) {
            // Создаем точки вдоль сегмента сплайна
            const int steps = 50;
            for (int i = 0; i < steps; ++i) {
                float t = i / (float)steps;
                float nextT = (i + 1) / (float)steps;

                glm::vec3 pos = route.getPoint(segment, t);
                glm::vec3 nextPos = route.getPoint(segment, nextT);

                // Вычисляем расстояние между текущей и следующей точкой
                float segmentLength = glm::length(nextPos - pos);

                // Получаем направление для создания ширины рельса
                glm::vec3 direction = route.getDirection(segment, t);
                glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

                if (glm::length(glm::cross(direction, up)) < 0.001f) {
                    up = glm::vec3(0.0f, 0.0f, 1.0f);
                }

                glm::vec3 right = glm::normalize(glm::cross(direction, up)) * railWidth;

                glm::vec3 left = pos - right;
                left.y += 0.01f;

                glm::vec3 right_pos = pos + right;
                right_pos.y += 0.01f;

                glm::vec3 nextDirection = route.getDirection(segment, nextT);
                if (glm::length(glm::cross(nextDirection, up)) < 0.001f) {
                    up = glm::vec3(0.0f, 0.0f, 1.0f);
                }

                glm::vec3 nextRight = glm::normalize(glm::cross(nextDirection, up)) * railWidth;
                glm::vec3 nextLeft = nextPos - nextRight;
                nextLeft.y += 0.01f;
                glm::vec3 nextRightPos = nextPos + nextRight;
                nextRightPos.y += 0.01f;

                glm::vec3 normal = glm::vec3(0.0f, 1.0f, 0.0f);

                // Вычисляем текстурные координаты на основе накопленного расстояния
                float nextAccumulatedDistance = accumulatedDistance + segmentLength;

                Vertex v1, v2, v3, v4;

                v1.Position = left;
                v1.Normal = normal;
                v1.TexCoords = glm::vec2(accumulatedDistance * textureRepeat, 0.0f);

                v2.Position = right_pos;
                v2.Normal = normal;
                v2.TexCoords = glm::vec2(accumulatedDistance * textureRepeat, 1.0f);

                v3.Position = nextLeft;
                v3.Normal = normal;
                v3.TexCoords = glm::vec2(nextAccumulatedDistance * textureRepeat, 0.0f);

                v4.Position = nextRightPos;
                v4.Normal = normal;
                v4.TexCoords = glm::vec2(nextAccumulatedDistance * textureRepeat, 1.0f);

                // Обновляем накопленное расстояние
                accumulatedDistance = nextAccumulatedDistance;

                unsigned int indexOffset = vertices.size();

                vertices.push_back(v1);
                vertices.push_back(v2);
                vertices.push_back(v3);
                vertices.push_back(v2);
                vertices.push_back(v4);
                vertices.push_back(v3);

                indices.push_back(indexOffset);
                indices.push_back(indexOffset + 1);
                indices.push_back(indexOffset + 2);
                indices.push_back(indexOffset + 3);
                indices.push_back(indexOffset + 4);
                indices.push_back(indexOffset + 5);
            }
        }
    }

    std::vector<Texture> textures;
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
        return true;
    }

    try {
        Texture diffuseTex(diffuseTexturePath);
        Texture specularTex(specularTexturePath);
        boxDiffuseTextureID = diffuseTex.id;
        boxSpecularTextureID = specularTex.id;
        std::cout << "Box diffuse texture ID: " << boxDiffuseTextureID << std::endl;
        std::cout << "Box specular texture ID: " << boxSpecularTextureID << std::endl;
        boxTexturesLoaded = true;
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
    shader.setVec3("light.position", lightSource.position);
    shader.setVec3("viewPos", camera.position);
    shader.setVec3("light.ambient", lightSource.ambient);
    shader.setVec3("light.diffuse", lightSource.diffuse);
    shader.setVec3("light.specular", lightSource.specular);
    shader.setFloat("material.shininess", 32.0f);
    shader.setInt("material.diffuse", 0);

    draw_rails(shader);
    draw_station_spheres(shader);
    
}

void RailroadMap::setStationNames(const std::vector<std::string>& names) {
    stationNames = names;
}

glm::vec2 RailroadMap::worldToScreen(const glm::vec3& worldPos, const Camera& camera, int width, int height) {
    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)width/(float)height, 0.1f, 1000.0f);
    glm::mat4 mvp = projection * view;

    glm::vec4 clipSpace = mvp * glm::vec4(worldPos, 1.0f);
    if (clipSpace.w <= 0) return glm::vec2(-1, -1);

    glm::vec3 ndc = glm::vec3(clipSpace) / clipSpace.w;
    glm::vec2 screenPos;
    screenPos.x = (ndc.x + 1.0f) * 0.5f * width;
    screenPos.y = (1.0f - ndc.y) * 0.5f * height;

    return screenPos;
}

void RailroadMap::drawStationLabels(const Camera& camera, int windowWidth, int windowHeight) {
    if (!showStationLabels || stationNames.empty()) return;

    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

    for (size_t i = 0; i < stations.size() && i < stationNames.size(); ++i) {
        glm::vec2 screenPos = worldToScreen(stations[i], camera, windowWidth, windowHeight);

        // Проверяем режим отображения
        if (labelDisplayMode == LabelDisplayMode::DYNAMIC) {
            float distance = glm::distance(camera.position, stations[i]);
            if (distance > 50.0f) continue; // Лейблы отображаются только если камера близко
        }

        if (screenPos.x >= 0 && screenPos.x <= windowWidth &&
            screenPos.y >= 0 && screenPos.y <= windowHeight) {

            ImGui::SetNextWindowPos(ImVec2(screenPos.x + 10, screenPos.y - 10));
            ImGui::Begin(("Station_" + std::to_string(i)).c_str(), nullptr,
                        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize);

            ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", stationNames[i].c_str());
            ImGui::End();
        }
    }

    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor();
}

void RailroadMap::generateTunnels() {
    if (tunnelsGenerated) return;

    tunnels.clear();

    // 1. Находим все пересечения маршрутов
    std::vector<IntersectionPoint> intersections = findIntersections();

    // 2. Создаем X-образные туннели для пересечений
    for (const auto& intersection : intersections) {
        generateXTunnel(intersection);
    }

    // 3. Создаем обычные туннели для участков без пересечений
    for (size_t i = 0; i < routes.size(); i++) {
        std::vector<glm::vec3> routePoints;
        const auto& route = routes[i];

        for (int segment = 0; segment < route.getSegmentCount(); ++segment) {
            const int steps = 10;
            for (int j = 0; j < steps; ++j) {
                float t = j / (float)steps;
                glm::vec3 point = route.getPoint(segment, t);

                // Проверяем, не находится ли точка рядом с пересечением
                bool nearIntersection = false;
                for (const auto& intersection : intersections) {
                    if (glm::distance(point, intersection.position) < intersection.radius * 1.5f) {
                        nearIntersection = true;
                        break;
                    }
                }

                if (!nearIntersection) {
                    routePoints.push_back(point);
                }
            }
        }

        if (routePoints.size() >= 2) {
            tunnels.emplace_back();
            tunnels.back().initialize(routePoints, 3.0f, 8);
        }
    }

    tunnelsGenerated = true;
    std::cout << "Generated " << tunnels.size() << " tunnels with " << intersections.size() << " X-intersections" << std::endl;
}

std::vector<RailroadMap::IntersectionPoint> RailroadMap::findIntersections() {
    std::vector<IntersectionPoint> intersections;
    const float intersectionThreshold = 2.0f; // Минимальное расстояние для считания пересечением

    // Проверяем все пары маршрутов
    for (size_t i = 0; i < routes.size(); i++) {
        for (size_t j = i + 1; j < routes.size(); j++) {
            glm::vec3 intersectionPoint;
            if (areRoutesIntersecting(i, j, intersectionPoint)) {

                // Проверяем, есть ли уже пересечение рядом
                bool foundExisting = false;
                for (auto& existing : intersections) {
                    if (glm::distance(existing.position, intersectionPoint) < intersectionThreshold) {
                        existing.routeIndices.push_back(j);
                        foundExisting = true;
                        break;
                    }
                }

                if (!foundExisting) {
                    IntersectionPoint newIntersection;
                    newIntersection.position = intersectionPoint;
                    newIntersection.routeIndices = {i, j};
                    newIntersection.radius = 5.0f; // Радиус X-образного туннеля
                    intersections.push_back(newIntersection);
                }
            }
        }
    }

    return intersections;
}

bool RailroadMap::areRoutesIntersecting(size_t route1, size_t route2, glm::vec3& intersectionPoint) {
    const auto& r1 = routes[route1];
    const auto& r2 = routes[route2];
    const float maxDistance = 3.0f; // Максимальное расстояние для считания пересечением

    // Проверяем все точки первого маршрута против всех точек второго
    for (int seg1 = 0; seg1 < r1.getSegmentCount(); ++seg1) {
        for (int i = 0; i < 20; i++) {
            float t1 = i / 20.0f;
            glm::vec3 point1 = r1.getPoint(seg1, t1);

            for (int seg2 = 0; seg2 < r2.getSegmentCount(); ++seg2) {
                for (int j = 0; j < 20; j++) {
                    float t2 = j / 20.0f;
                    glm::vec3 point2 = r2.getPoint(seg2, t2);

                    float distance = glm::distance(point1, point2);
                    if (distance < maxDistance) {
                        intersectionPoint = (point1 + point2) * 0.5f; // Средняя точка
                        return true;
                    }
                }
            }
        }
    }

    return false;
}

void RailroadMap::generateXTunnel(const IntersectionPoint& intersection) {
    // Создаем сферический туннель в точке пересечения
    std::vector<glm::vec3> spherePoints;
    const float radius = intersection.radius;
    const int segments = 16; // Количество точек для создания сферической формы

    glm::vec3 center = intersection.position;

    // Создаем точки вокруг центра пересечения для сферической формы
    for (int i = 0; i < segments; ++i) {
        float angle = (2.0f * glm::pi<float>() * i) / segments;
        glm::vec3 point = center + glm::vec3(
            radius * 0.5f * cos(angle),
            0.0f, // Держим на том же уровне
            radius * 0.5f * sin(angle)
        );
        spherePoints.push_back(point);
    }

    // Добавляем центральную точку для замыкания
    spherePoints.push_back(center);

    if (spherePoints.size() >= 2) {
        tunnels.emplace_back();
        tunnels.back().initialize(spherePoints, radius, 12);
    }
}


void RailroadMap::drawTunnels(const Shader& shader, bool transparent) {
    if (!showTunnels) {
        return; // Не рисуем туннели, если они отключены
    }

    if (!tunnelsGenerated) {
        generateTunnels();
    }

    for (auto& tunnel : tunnels) {
        tunnel.draw(shader, transparent);
    }
}

bool RailroadMap::loadTunnelTextures(const std::string& texturePath) {
    if (!tunnelsGenerated) {
        std::cout << "Tunnels not generated yet!" << std::endl;
        return false;
    }

    bool success = true;
    for (auto& tunnel : tunnels) {
        tunnel.loadTexture(texturePath);
    }

    return success;
}
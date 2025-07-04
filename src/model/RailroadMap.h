//
// Created by arche on 1.07.2025.
//

#ifndef RAILROAD_MAP_H
#define RAILROAD_MAP_H

#include <vector>
#include <stdexcept>
#include <glm/glm.hpp>
#include "Camera.h"
#include <memory>
#include <iostream>
#include "LightSource.h"
#include "Mesh.h"
#include "Shader.h"
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "Tunnel.h"

// Структура для хранения кривой Безье третьего порядка


inline float cube_vertices_pnt[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
        // ... и так далее для остальных 5 граней куба
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };




class CatmullRomSpline {
private:
    std::vector<glm::vec3> controlPoints;

public:
    explicit CatmullRomSpline(const std::vector<glm::vec3>& points) : controlPoints(points) {
        // Добавляем виртуальные точки в начале и конце для корректного расчета
        if (points.size() >= 2) {
            glm::vec3 firstVirtual = points[0] - (points[1] - points[0]);
            glm::vec3 lastVirtual = points.back() + (points.back() - points[points.size() - 2]);

            controlPoints.insert(controlPoints.begin(), firstVirtual);
            controlPoints.push_back(lastVirtual);
        }
    }

    glm::vec3 getPoint(int segment, float t) const {
        if (segment < 0 || segment >= controlPoints.size() - 3 || t < 0.0f || t > 1.0f) {
            return glm::vec3(0.0f);
        }

        // Точки для расчета сегмента сплайна Катмулла-Рома
        const glm::vec3& p0 = controlPoints[segment];
        const glm::vec3& p1 = controlPoints[segment + 1];
        const glm::vec3& p2 = controlPoints[segment + 2];
        const glm::vec3& p3 = controlPoints[segment + 3];

        // Расчет по формуле Катмулла-Рома
        float t2 = t * t;
        float t3 = t2 * t;

        // Матрица Катмулла-Рома
        // [ 0  2  0  0 ]   [ p0 ]
        // [-1  0  1  0 ] * [ p1 ]
        // [ 2 -5  4 -1 ]   [ p2 ]
        // [-1  3 -3  1 ]   [ p3 ]
        glm::vec3 result = 0.5f * (
            (2.0f * p1) +
            (-p0 + p2) * t +
            (2.0f * p0 - 5.0f * p1 + 4.0f * p2 - p3) * t2 +
            (-p0 + 3.0f * p1 - 3.0f * p2 + p3) * t3
        );

        return result;
    }

    glm::vec3 getDirection(int segment, float t) const {
        if (segment < 0 || segment >= controlPoints.size() - 3 || t < 0.0f || t > 1.0f) {
            return glm::vec3(0.0f);
        }

        const glm::vec3& p0 = controlPoints[segment];
        const glm::vec3& p1 = controlPoints[segment + 1];
        const glm::vec3& p2 = controlPoints[segment + 2];
        const glm::vec3& p3 = controlPoints[segment + 3];

        float t2 = t * t;

        // Производная от сплайна Катмулла-Рома
        glm::vec3 result = 0.5f * (
            (-p0 + p2) +
            (2.0f * p0 - 5.0f * p1 + 4.0f * p2 - p3) * (2.0f * t) +
            (-p0 + 3.0f * p1 - 3.0f * p2 + p3) * (3.0f * t2)
        );

        return result;
    }

    int getSegmentCount() const {
        return std::max(0, static_cast<int>(controlPoints.size()) - 3);
    }
};

enum class LabelDisplayMode {
    STATIC,
    DYNAMIC
};

class RailroadMap {
    std::vector<std::vector<int>> routeIndices; // Маршруты как индексы точек
    std::vector<CatmullRomSpline> routes;
    std::vector<std::string> stationNames; // Для хранения названий станций

    std::unique_ptr<Mesh> railMesh;
    std::unique_ptr<Mesh> stationMesh;
    std::unique_ptr<Mesh> stationBoxMesh;
    std::unique_ptr<Mesh> stationSphereMesh;

    LabelDisplayMode labelDisplayMode = LabelDisplayMode::STATIC;


    bool useSimpleAnimation = false;
    std::vector<glm::vec3> routeColors;
    std::unique_ptr<Mesh> simpleLinesMesh;
    void createSimpleLinesMesh();
    void generateRouteColors();


    int railVerticesCount{}, stationVerticesCount{};
    unsigned int railTextureID = 0, stationTextureID = 0;
    unsigned int boxDiffuseTextureID = 0, boxSpecularTextureID = 0;
    bool boxTexturesLoaded = false;

    std::vector<Tunnel> tunnels;
    bool tunnelsGenerated = false;
    // Новые методы
    std::vector<glm::vec3> getAllRoutePoints();
    std::vector<std::vector<glm::vec3>> getSegmentedRoutes();
    bool arePointsClose(const glm::vec3& p1, const glm::vec3& p2, float threshold = 2.0f);
    struct IntersectionPoint {
        glm::vec3 position;
        std::vector<size_t> routeIndices; // Какие маршруты пересекаются
        float radius;
    };

    std::vector<IntersectionPoint> findIntersections();
    void generateXTunnel(const IntersectionPoint& intersection);
    bool areRoutesIntersecting(size_t route1, size_t route2, glm::vec3& intersectionPoint);
    bool showTunnels = false; // Флаг для отображения туннелей

    std::vector<float> trainPositions; // Позиции "поездов" для каждого маршрута
    glm::vec3 getPositionOnRoute(size_t routeIndex, float progress) const;
    std::vector<std::vector<glm::vec3>> completedPaths; // Завершенные пути для каждого маршрута
    std::vector<bool> routeCompleted; // Флаги завершения маршрутов


    void createRailsMesh();
    void createStationsMesh();
    void createStationBoxMesh();

    void draw_rails(const Shader& shader);
    void draw_stations(const Shader& shader);

public:
    std::vector<glm::vec3> allPoints; // Все точки
    bool showStationLabels = true;
    RailroadMap() = default;
    RailroadMap(const std::vector<std::vector<glm::vec3>>& routePoints);
    std::vector<glm::vec3> stations;

    void setStationNames(const std::vector<std::string>& names);
    void drawStationLabels(const Camera& camera, int windowWidth, int windowHeight);
    void toggleStationLabels() { showStationLabels = !showStationLabels; }

    void setLabelDisplayMode(LabelDisplayMode mode) { labelDisplayMode = mode; }
    LabelDisplayMode getLabelDisplayMode() const { return labelDisplayMode; }
    const char* getLabelDisplayModeString() const {
        switch (labelDisplayMode) {
            case LabelDisplayMode::STATIC:
                return "Static";
            case LabelDisplayMode::DYNAMIC:
                return "Dynamic";
            default:
                return showStationLabels ?
                    (labelDisplayMode == LabelDisplayMode::STATIC ? "Static" : "Dynamic") : "Off";
        }
    }

    glm::vec2 worldToScreen(const glm::vec3& worldPos, const Camera& camera, int width, int height);

    void setPoints(const std::vector<glm::vec3>& points);
    void addRoute(const std::vector<int>& pointIndices);
    void initialize(const std::vector<glm::vec3>& points, const std::vector<std::vector<int>>& routes);

    const glm::vec3& getPoint(int index) const { return allPoints[index]; }
    size_t getPointsCount() const { return allPoints.size(); }


    // Обновляем сигнатуру функции - теперь без дополнительных параметров
    void draw_station_boxes(const Shader& shader);

    // В RailroadMap.h добавьте новый метод:
    void createStationSpheresMesh();
    void draw_station_spheres(const Shader& shader);

    ~RailroadMap();

    bool loadTextures(const std::string& railTexturePath, const std::string& stationTexturePath);
    bool loadStationBoxTextures(const std::string& diffuseTexturePath, const std::string& specularTexturePath);

    void draw(const Shader &shader, const Camera &camera, const LightSource &lightSource);

    void generateTunnels();
    void drawTunnels(const Shader& shader, bool transparent = false);
    bool loadTunnelTextures(const std::string& texturePath);
    void setShowTunnels(bool show) { showTunnels = show; }
    bool getShowTunnels() const { return showTunnels; }

    [[nodiscard]] unsigned int getRailTextureID() const { return railTextureID; }
    [[nodiscard]] unsigned int getStationTextureID() const { return stationTextureID; }

    // Добавьте эти методы в публичную секцию RailroadMap
    size_t getRouteCount() const { return routes.size(); }
    const CatmullRomSpline& RailroadMap::getRoute(int index) const {
        if (index < 0 || index >= static_cast<int>(routes.size())) {
            std::cout << "ERROR: Route index " << index << " out of range (0-" << routes.size()-1 << ")" << std::endl;
            throw std::out_of_range("Route index out of range");
        }
        return routes[index];
    }
    void clear();
    void setUseSimpleAnimation(bool useSimple) { useSimpleAnimation = useSimple; }
    bool getUseSimpleAnimation() const { return useSimpleAnimation; }
    void drawSimpleLines(const Shader& shader);
    void updateTrainPositions(float deltaTime);
};


#endif // RAILROAD_MAP_H
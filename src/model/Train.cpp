//
// Created by arche on 3.07.2025.
//

#define GLM_ENABLE_EXPERIMENTAL
#include "Train.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>

void Train::update(float deltaTime) {
    if (speed <= 0.0f) return;

    currentT += speed * deltaTime * 0.01f; // уменьшенный коэффициент

    const auto& route = railroadMap->getRoute(routeIndex);
    if (currentT >= 1.0f) {
        currentT = 0.0f;
        currentSegment++;
        if (currentSegment >= route.getSegmentCount()) {
            currentSegment = 0; // зацикливаем маршрут
        }
    }

    updatePosition();
}


void Train::updatePosition() {
    if (!railroadMap || routeIndex < 0 || routeIndex >= railroadMap->getRouteCount()) {
        return;
    }

    const auto& route = railroadMap->getRoute(routeIndex);

    if (currentSegment >= route.getSegmentCount()) {
        currentSegment = 0;
        currentT = 0.0f;
    }

    // Получаем позицию и направление
    position = route.getPoint(currentSegment, currentT);
    glm::vec3 direction = glm::normalize(route.getDirection(currentSegment, currentT));

    if (glm::length(direction) > 0.001f) {
        float yaw = atan2(direction.x, direction.z);
        rotation = glm::vec3(0.0f, glm::degrees(yaw), 0.0f);
    }

    // Обновляем позицию модели
    if (model) {
        model->position = position;
        model->rotation_deg = rotation;
    }
}

void Train::draw(const Shader& shader, const Camera& camera, const LightSource& lightSource) {
    if (model && model->is_loaded()) {
        model->draw(shader, camera, lightSource);
    }
}

void Train::setSlowdownSettings(float slowdownSpd, float slowdownRad) {
    slowdownSpeed = slowdownSpd;
    slowdownRadius = slowdownRad;
}
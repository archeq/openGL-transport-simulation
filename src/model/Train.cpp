//
// Created by arche on 3.07.2025.
//

#define GLM_ENABLE_EXPERIMENTAL
#include "Train.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>

// void Train::update(float deltaTime) {
//     if (speed <= 0.0f) return;
//
//     // Проверяем близость к станциям и корректируем скорость
//     float currentSpeed = speed;
//     for (const auto& station : railroadMap->stations) {
//         float distance = glm::length(position - station);
//         if (distance < slowdownRadius) {
//             currentSpeed = slowdownSpeed;
//             isSlowingDown = true;
//             break;
//         } else {
//             isSlowingDown = false;
//             currentSpeed = originalSpeed;
//         }
//     }
//
//     // Обновляем позицию по маршруту
//     const auto& route = railroadMap->getRoute(routeIndex);
//
//
//
//     // Увеличиваем параметр t
//     float step = currentSpeed * deltaTime * 0.1f; // Коэффициент для корректировки скорости
//     currentT += step;
//
//
//     // Переход к следующему сегменту
//     if (currentT >= 1.0f) {
//         currentT = 0.0f;
//         currentSegment++;
//
//         // Проверка конца маршрута
//         if (currentSegment >= route.getSegmentCount()) {
//             currentSegment = 0; // Цикличный маршрут
//         }
//     }
//
//     updatePosition();
// }


void Train::update(float deltaTime) {
    if (speed <= 0.0f) return;

    // Проверяем замедление у станций
    checkStationSlowdown();

    // Двигаем поезд по маршруту
    float moveSpeed = isSlowingDown ? slowdownSpeed : originalSpeed;
    currentT += moveSpeed * deltaTime * 0.01f; // уменьшенный коэффициент

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

    // Вычисляем поворот модели
    glm::vec3 forward = glm::vec3(0.0f, 0.0f, 1.0f);

    if (glm::length(direction) > 0.001f) {
        // Вычисляем угол поворота вокруг оси Y
        float yaw = atan2(direction.x, direction.z);
        rotation = glm::vec3(0.0f, glm::degrees(yaw), 0.0f);
    }

    // Обновляем позицию модели
    if (model) {
        model->position = position;
        model->rotation_deg = rotation;
    }
}


void Train::checkStationSlowdown() {
    // Проверяем расстояние до ближайших станций
    bool nearStation = false;
    for (const auto& station : railroadMap->stations) {
        float distance = glm::length(position - station);
        if (distance < slowdownRadius) {
            nearStation = true;
            break;
        }
    }

    if (nearStation && !isSlowingDown) {
        speed = slowdownSpeed;
        isSlowingDown = true;
    } else if (!nearStation && isSlowingDown) {
        speed = originalSpeed;
        isSlowingDown = false;
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
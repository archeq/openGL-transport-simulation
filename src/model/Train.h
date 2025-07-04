//
// Created by arche on 3.07.2025.
//


#ifndef TRAIN_H
#define TRAIN_H

#include <memory>
#include <stdexcept>
#include <glm/glm.hpp>
#include "Model.h"
#include "RailroadMap.h"

class Train {
    std::shared_ptr<Model> model;
    int routeIndex;
    int currentSegment;
    float currentT;
    float speed;
    glm::vec3 position;
    glm::vec3 rotation;

    const RailroadMap* railroadMap;

public:
    Train(std::shared_ptr<Model> trainModel, int route, float trainSpeed, const RailroadMap* map)
        : model(trainModel), routeIndex(route), currentSegment(0), currentT(0.0f),
          speed(trainSpeed),
          position(0.0f), rotation(0.0f), railroadMap(map) {

        if (!railroadMap) {
            throw std::out_of_range("RailroadMap is null");
        }

        if (routeIndex < 0 || routeIndex >= railroadMap->getRouteCount()) {
            throw std::out_of_range("Route index out of range");
        }

        updatePosition();
    }

    void update(float deltaTime);
    void draw(const Shader& shader, const Camera& camera, const LightSource& lightSource);
    void setSlowdownSettings(float slowdownSpeed, float slowdownRadius);
    void Train::checkStationSlowdown();
    void setSpeed(float newSpeed) { speed = newSpeed; }
    float getCurrentSpeed() const { return speed; }
    glm::vec3 getPosition() const { return position; }

private:
    void updatePosition();
    float slowdownSpeed = 5.0f;
    float slowdownRadius = 2.0f;
};

#endif // TRAIN_H
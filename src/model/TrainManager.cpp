//
// Created by arche on 3.07.2025.
//

#define GLM_ENABLE_EXPERIMENTAL
#include "TrainManager.h"
#include <iostream>


TrainManager::TrainManager(const RailroadMap& railroadMap) : railroadMap(railroadMap) {}

void TrainManager::addTrain(std::shared_ptr<Model> model, int routeIndex, float speed) {
    if (!model) {
        std::cout << "ERROR: Model is null" << std::endl;
        return;
    }

    if (routeIndex < 0 || routeIndex >= railroadMap.getRouteCount()) {
        std::cout << "ERROR: Invalid route index " << routeIndex << std::endl;
        return;
    }

    try {
        auto trainModel = std::make_shared<Model>(*model);
        auto train = std::make_unique<Train>(trainModel, routeIndex, speed, &railroadMap);
        train->setSpeed(speed);

        trains.push_back(std::move(train));
        std::cout << "Added train to route " << routeIndex << " with speed " << speed << std::endl;
    } catch (const std::exception& e) {
        std::cout << "ERROR: Failed to create train: " << e.what() << std::endl;
    }
}

void TrainManager::update(float deltaTime) {
    for (auto& train : trains) {
        train->update(deltaTime);
    }
}

void TrainManager::draw(const Shader& shader, const Camera& camera, const LightSource& lightSource) {
    for (auto& train : trains) {
        train->draw(shader, camera, lightSource);
    }
}

void TrainManager::setGlobalSpeed(float speed) {
    globalSpeedMultiplier = speed;
    for (auto& train : trains) {
        train->setSpeed(train->getCurrentSpeed() * speed);
    }
}

Train* TrainManager::getTrain(size_t index) {
    if (index < trains.size()) {
        return trains[index].get();
    }
    return nullptr;
}
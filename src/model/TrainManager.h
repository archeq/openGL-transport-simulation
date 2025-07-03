//
// Created by arche on 3.07.2025.
//

#ifndef TRAIN_MANAGER_H
#define TRAIN_MANAGER_H

#include <vector>
#include <memory>
#include "Train.h"
#include "Model.h"
#include "RailroadMap.h"

class TrainManager {
public:
    explicit TrainManager(const RailroadMap& railroadMap);

    void addTrain(std::shared_ptr<Model> model, int routeIndex, float speed = 10.0f);
    void update(float deltaTime);
    void draw(const Shader& shader, const Camera& camera, const LightSource& lightSource);

    // Управление всеми поездами
    void pauseAll();
    void resumeAll();
    void resetAll();

    // Настройки
    void setGlobalSpeed(float speed);
    void setStationSlowdownSettings(float distance, float factor);

    // Информация
    size_t getTrainCount() const { return trains.size(); }
    Train* getTrain(size_t index);



private:
    const RailroadMap& railroadMap;
    std::vector<std::unique_ptr<Train>> trains;

    float globalSpeedMultiplier = 1.0f;
    float stationSlowdownDistance = 5.0f;
    float stationSlowdownFactor = 0.3f;
};

#endif // TRAIN_MANAGER_H
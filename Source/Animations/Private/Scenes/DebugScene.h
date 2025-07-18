// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef DEBUGSCENE_H
#define DEBUGSCENE_H

#include "Scenes/Scene.h"


class DebugScene final : public Scene
{
public:
    DebugScene(ResourceManager& resourceManager, EventManager& eventManager);
    void Initialize() override;
    void Update(float deltaTime) override;

private:
    int _fpsEntityId{GenerateId()};
    int _ballCountEntityId{GenerateId()};

    int _fps{0};
    int _ballCount{0};
};


#endif

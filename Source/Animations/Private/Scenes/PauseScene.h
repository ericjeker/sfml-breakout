// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef PAUSESCENE_H
#define PAUSESCENE_H

#include "Scenes/Scene.h"

class PauseScene final : public Scene
{
public:
    PauseScene(ResourceManager& resourceManager, EventManager& eventManager);
    void Initialize() override;
};


#endif

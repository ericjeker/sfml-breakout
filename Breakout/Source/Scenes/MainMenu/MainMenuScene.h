// Copyright (c) Eric Jeker 2025.

#pragma once


#include "Core/Scenes/Scene.h"

class MainMenuScene final : public Scene
{
public:
    explicit MainMenuScene(flecs::world& world);

    void Initialize() override;

private:
    void CreateUIEntities(const flecs::world& world);
};

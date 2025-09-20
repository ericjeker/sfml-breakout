// Copyright (c) Eric Jeker 2025.

#pragma once


#include "Core/Scenes/Scene.h"

class MenuScene final : public Scene
{
public:
    explicit MenuScene(flecs::world& world);

    void Initialize() override;

private:
    void CreateUIEntities(const flecs::world& world);
};

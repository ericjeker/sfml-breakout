// Copyright (c) Eric Jeker 2025.

#pragma once

#include "Core/Scenes/Scene.h"


class GameOverScene final : public Scene
{
public:
    explicit GameOverScene(flecs::world& world);
    void Initialize() override;

private:
    void CreateUISystems(const flecs::world& world);
};

// Copyright (c) Eric Jeker 2025.

#pragma once

#include "Core/Scenes/Scene.h"


class PauseScene final : public Scene
{
public:
    explicit PauseScene(flecs::world& world);
    void Initialize() override;
    void CreateEventBindings(const flecs::world& world) const;
};

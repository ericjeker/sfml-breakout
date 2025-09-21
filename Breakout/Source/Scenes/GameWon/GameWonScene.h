// Copyright (c) Eric Jeker 2025.

#pragma once
#include "Core/Scenes/Scene.h"


class GameWonScene final : public Scene
{
public:
    explicit GameWonScene(flecs::world& world);
    void Initialize() override;
    void CreateEventBindings(const flecs::world& world) const;
};

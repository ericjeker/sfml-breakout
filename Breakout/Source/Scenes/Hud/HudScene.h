// Copyright (c) Eric Jeker 2025.

#pragma once

#include "Core/Scenes/Scene.h"


class HudScene final : public Scene
{
public:
    explicit HudScene(flecs::world& world);
    void Initialize() override;
};

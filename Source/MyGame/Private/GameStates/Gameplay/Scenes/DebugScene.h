// Copyright (c) Eric Jeker 2025.

#pragma once



#include "Core/Scenes/Scene.h"


struct FPS
{
};

class DebugScene final : public Scene
{
public:
    explicit DebugScene(flecs::world& world);
    void Initialize() override;
};




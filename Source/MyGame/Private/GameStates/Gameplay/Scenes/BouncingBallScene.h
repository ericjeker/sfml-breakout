// Copyright (c) Eric Jeker 2025.

#pragma once



#include "Core/Scenes/Scene.h"

#include <flecs.h>


class BouncingBallScene final : public Scene
{
public:
    explicit BouncingBallScene(flecs::world& world);
    void Initialize() override;

private:
    void CreateBalls(const flecs::world& world, float zOrder);
};




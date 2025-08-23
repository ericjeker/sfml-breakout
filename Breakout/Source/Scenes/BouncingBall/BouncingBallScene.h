// Copyright (c) Eric Jeker 2025.

#pragma once


#include "Core/Modules/Physics/Components/ColliderShape.h"
#include "Core/Modules/Physics/Components/Velocity.h"
#include "Core/Modules/Render/Components/Radius.h"
#include "Core/Modules/Render/Components/Transform.h"
#include "Core/Scenes/Scene.h"

#include <flecs.h>


class BouncingBallScene final : public Scene
{
public:
    explicit BouncingBallScene(flecs::world& world);
    void Initialize() override;

private:
    void CreateBalls(const flecs::world& world, float zOrder);
    static void ProcessScreenBounce(Transform& t, Velocity& v, const Radius& r, const ColliderShape& c);
};

// Copyright (c) Eric Jeker 2025.

#pragma once

#include "Components/Ball.h"

#include "Core/Modules/Physics/Components/ColliderShape.h"
#include "Core/Modules/Physics/Components/Velocity.h"
#include "Core/Modules/Render/Components/Radius.h"
#include "Core/Modules/Render/Components/Transform.h"
#include "Core/Scenes/Scene.h"


class GameplayScene final : public Scene
{
public:
    explicit GameplayScene(flecs::world& world);
    void Initialize() override;
    void HandleEvent(const std::optional<sf::Event>& event) override;

private:
    void CreateInputBindings(const flecs::world& world);
    void CreateLocalSystems(const flecs::world& world);
    void CreateUISystem(flecs::world& world);
    void CreatePaddle(const flecs::world& world);
    void CreateBlocks(const flecs::world& world);
    void CreateBall(const flecs::world& world);

    static void ProcessScreenBounce(Transform& t, Velocity& v, const ColliderShape& c, const Radius& r, const Ball& b);
    static void ProcessOutOfBounds(flecs::entity ball, const Transform& t, const Ball& b);
};




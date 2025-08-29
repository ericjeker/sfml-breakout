// Copyright (c) Eric Jeker 2025.

#pragma once

#include "Components/AttachedToPaddle.h"
#include "Components/Ball.h"
#include "Components/Paddle.h"
#include "Components/PauseGameIntent.h"

#include "Core/Modules/Physics/Components/ColliderShape.h"
#include "Core/Modules/Physics/Components/Velocity.h"
#include "Core/Modules/Render/Components/Origin.h"
#include "Core/Modules/Render/Components/Radius.h"
#include "Core/Modules/Render/Components/Size.h"
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
    void CreatePaddle(const flecs::world& world, float& zOrder);
    void CreateBlocks(const flecs::world& world, float& zOrder);
    void CreateBall(const flecs::world& world, float& zOrder);
    void CreateBackground(const flecs::world& world, float& zOrder);

    static void ProcessScreenBounce(Transform& t, Velocity& v, const ColliderShape& c, const Radius& r, const Ball& b);
    static void ProcessOutOfBounds(flecs::entity ball, const Transform& t, const Ball& b);
    static void ProcessCollisionDetection(
        const flecs::entity& blockEntity,
        const Transform& transform,
        const Size& size,
        const Origin& origin,
        const ColliderShape& c
    );
    static void ConstrainPaddleToScreen(Transform& t, const Size& s, const Paddle& p);
    static void ApplyPaddlePositionToBall(const flecs::entity& e, Transform& t, const AttachedToPaddle& ap, const Ball& b);
    static sf::Vector2f CalculateReflection(const sf::Vector2f& velocity, const sf::Vector2f& normal);
    void ProcessPauseGameIntent(const flecs::entity& e, const PauseGameIntent& p);
    void PauseGame(const flecs::world& w);
};




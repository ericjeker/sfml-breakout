// Copyright (c) Eric Jeker 2025.

#include "BallEntity.h"

#include "Scenes/Gameplay/Components/AttachedToPaddle.h"
#include "Scenes/Gameplay/Components/Ball.h"

#include "Core/Configuration.h"
#include "Core/Modules/Physics/Components/Velocity.h"
#include "Core/Modules/Render/Factories/Circle.h"

#include <SFML/Graphics/Color.hpp>


flecs::entity BallEntity::Create(const flecs::world& world, const flecs::entity& rootEntity, float zOrder)
{
    constexpr float CENTER_X = Configuration::RESOLUTION.x / 2;
    constexpr float RADIUS = 10.f;

    return Factories::Circle::Create(
        world,
        {
            .radius = RADIUS,
            .color = sf::Color::White,
            .origin = {0.5f, 0.5f},
            .position = {CENTER_X, Configuration::RESOLUTION.y - 125.f},
            .zOrder = ++zOrder,
        }
    )
        .child_of(rootEntity)
        // TODO: maybe just AttachedTo and add a socket information
        .add<AttachedToPaddle>()
        .add<Ball>()
        .set<Velocity>({});
}

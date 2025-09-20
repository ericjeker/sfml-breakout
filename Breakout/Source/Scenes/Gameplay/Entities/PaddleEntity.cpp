// Copyright (c) Eric Jeker 2025.

#include "PaddleEntity.h"

#include "Modules/Breakout/Components/Paddle.h"

#include "Core/Configuration.h"
#include "Core/Modules/Input/Components/PossessedByPlayer.h"
#include "Core/Modules/Physics/Components/Acceleration.h"
#include "Core/Modules/Physics/Components/ColliderShape.h"
#include "Core/Modules/Physics/Components/Friction.h"
#include "Core/Modules/Physics/Components/Velocity.h"
#include "Core/Modules/Render/Factories/Rectangle.h"
#include "Core/Themes/Nord.h"


flecs::entity PaddleEntity::Create(const flecs::world& world, const flecs::entity& rootEntity, float zOrder)
{
    constexpr float CENTER_X = Configuration::RESOLUTION.x / 2;

    return Factories::Rectangle::
        Create(world,
               {
                   .size = {100.f, 20.f},
                   .color = NordTheme::Aurora1,
                   .origin = {0.5f, 0.5f},
                   .position = {CENTER_X, Configuration::RESOLUTION.y - 100.f},
                   .zOrder = 0.f,
               })
            .child_of(rootEntity)
            .add<PossessedByPlayer>()
            .add<Paddle>()
            .set<Acceleration>({})
            .set<Friction>({.friction = 10.f})
            .set<Velocity>({})
            .set<ColliderShape>({Shape::Rectangle});
}

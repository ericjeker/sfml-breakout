// Copyright (c) Eric Jeker 2025.

#include "ResumeGame.h"

#include "Scenes/Gameplay/Components/Ball.h"
#include "Scenes/Gameplay/Components/Paddle.h"

#include "Core/Scenes/Tags/ScenePaused.h"


void ResumeGame::Run(const flecs::world& world, const flecs::entity& rootEntity)
{
    assert(rootEntity.is_valid());
    rootEntity.remove<ScenePaused>();

    // Query for the Paddle and Ball to enable them
    world.query_builder<>()
        .scope_open()
        .with<Ball>()
        .oper(flecs::Or)
        .with<Paddle>()
        .scope_close()
        .with(flecs::Disabled)
        .optional()
        .with(flecs::ChildOf, rootEntity)
        .each([](const flecs::entity& entity) { entity.enable(); });
}

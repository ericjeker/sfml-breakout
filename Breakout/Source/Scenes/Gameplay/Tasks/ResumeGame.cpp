// Copyright (c) Eric Jeker 2025.

#include "ResumeGame.h"

#include "Scenes/Gameplay/Components/Ball.h"
#include "Scenes/Gameplay/Components/Paddle.h"


void ResumeGame::Run(const flecs::world& world)
{
    // Query for the Paddle and Ball to enable them
    world.query_builder<>()
        .scope_open()
        .with<Ball>()
        .oper(flecs::Or)
        .with<Paddle>()
        .scope_close()
        .with(flecs::Disabled)
        .optional()
        .each([](const flecs::entity& entity) { entity.enable(); });
}

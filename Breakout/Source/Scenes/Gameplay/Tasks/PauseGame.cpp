// Copyright (c) Eric Jeker 2025.

#include "PauseGame.h"

#include "Scenes/Gameplay/Components/Ball.h"
#include "Scenes/Gameplay/Components/Paddle.h"

void PauseGame::Run(const flecs::world& world)
{
    // Query for the Paddle and Ball to disable them
    world.query_builder<>()
        .scope_open()
        .with<Ball>()
        .oper(flecs::Or)
        .with<Paddle>()
        .scope_close()
        .each([](const flecs::entity& entity) { entity.disable(); });
}

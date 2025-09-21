// Copyright (c) Eric Jeker 2025.

#include "ResumeGame.h"

#include "Modules/Breakout/Components/Ball.h"
#include "Modules/Breakout/Components/Paddle.h"
#include "Scenes/Gameplay/GameplayScene.h"

#include "Core/GameService.h"
#include "Core/Managers/SceneManager.h"
#include "Core/Modules/Scene/Tags/ScenePaused.h"


void ResumeGame::Run(const flecs::world& world)
{
    // Unpause the scene by removing the ScenePause tag
    auto& sceneManager = GameService::Get<SceneManager>();
    sceneManager.GetScene<GameplayScene>().GetRootEntity().remove<ScenePaused>();

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

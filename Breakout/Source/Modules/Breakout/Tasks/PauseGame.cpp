// Copyright (c) Eric Jeker 2025.

#include "PauseGame.h"

#include "Modules/Breakout/Components/Ball.h"
#include "Modules/Breakout/Components/Paddle.h"
#include "Scenes/Gameplay/GameplayScene.h"

#include "Core/GameService.h"
#include "Core/Managers/SceneManager.h"
#include "Core/Modules/Scene/Tags/ScenePaused.h"

void PauseGame::Run(const flecs::world& world)
{
    // Set the scene as paused by adding a tag to its root entity
    auto& sceneManager = GameService::Get<SceneManager>();
    sceneManager.GetScene<GameplayScene>().GetRootEntity().add<ScenePaused>();

    // Query for the Paddle and Ball to disable them
    world.query_builder<>().scope_open().with<Ball>().oper(flecs::Or).with<Paddle>().scope_close().each(
        [](const flecs::entity& entity) { entity.disable(); }
    );
}

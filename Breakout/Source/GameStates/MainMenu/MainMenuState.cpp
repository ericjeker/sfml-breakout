// Copyright (c) Eric Jeker 2025.

#include "MainMenuState.h"

#include "Scenes/MainMenu/MainMenuScene.h"

#include "Core/GameStates/GameState.h"
#include "Core/Managers/GameService.h"
#include "Core/Managers/SceneManager.h"
#include "Core/Utils/Logger.h"

MainMenuState::MainMenuState(flecs::world& world)
    : GameState(world)
{
}

void MainMenuState::Enter()
{
    // --- Configure the Scene by updating singletons, enabling/disabling systems ---
    LOG_DEBUG("MainMenuState::Enter -> Adding scenes to the SceneManager");
    auto& sceneManager = GameService::Get<SceneManager>();
    sceneManager.AddScene<MainMenuScene>(std::make_unique<MainMenuScene>(GetWorld()));

    LOG_DEBUG("MainMenuState::Enter -> Loading MainMenuScene");
    sceneManager.LoadScene<MainMenuScene>(SceneLoadMode::Single);
}

void MainMenuState::Exit()
{
    LOG_DEBUG("MainMenuState::Exit");
    auto& sceneManager = GameService::Get<SceneManager>();
    sceneManager.RemoveScene<MainMenuScene>();
}

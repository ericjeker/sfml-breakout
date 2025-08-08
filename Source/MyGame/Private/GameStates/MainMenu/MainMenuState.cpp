// Copyright (c) Eric Jeker 2025.

#include "MainMenuState.h"

#include "Core/GameInstance.h"
#include "Core/Logger.h"
#include "Core/Managers/GameService.h"
#include "Core/Managers/SceneManager.h"
#include "Scenes/MainMenuScene.h"

MainMenuState::MainMenuState(flecs::world& world)
    : GameState(world)
{
}

void MainMenuState::Enter()
{
    LOG_DEBUG("(MainMenuState::MainMenuState): Adding scenes to the SceneManager");
    auto& sceneManager = GameService::Get<SceneManager>();
    sceneManager.AddScene<MainMenuScene>(std::make_unique<MainMenuScene>(GetWorld()));

    LOG_DEBUG("(MainMenuState::Enter): LoingBallSceneading MainMenuScene");
    sceneManager.LoadScene<MainMenuScene>(SceneLoadMode::Single);
}

void MainMenuState::Exit()
{
    LOG_DEBUG("(MainMenuState::Exit)");
    auto& sceneManager = GameService::Get<SceneManager>();
    sceneManager.RemoveScene<MainMenuScene>();
}

void MainMenuState::HandleEvent(const std::optional<sf::Event>& event)
{
}

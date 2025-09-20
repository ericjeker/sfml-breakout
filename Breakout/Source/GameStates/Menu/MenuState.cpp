// Copyright (c) Eric Jeker 2025.

#include "MenuState.h"

#include "Scenes/Menu/MenuScene.h"

#include "Core/GameService.h"
#include "Core/GameStates/GameState.h"
#include "Core/Managers/SceneManager.h"
#include "Core/Utils/Logger.h"

MenuState::MenuState(flecs::world& world)
    : GameState(world)
{
}

void MenuState::Enter()
{
    // --- Configure the Scene by updating singletons, enabling/disabling systems ---
    LOG_DEBUG("MenuState::Enter -> Adding scenes to the SceneManager");
    auto& sceneManager = GameService::Get<SceneManager>();
    sceneManager.AddScene<MenuScene>(std::make_unique<MenuScene>(GetWorld()));

    LOG_DEBUG("MenuState::Enter -> Loading MenuScene");
    sceneManager.LoadScene<MenuScene>(SceneLoadMode::Single);
}

void MenuState::Exit()
{
    LOG_DEBUG("MenuState::Exit");
    auto& sceneManager = GameService::Get<SceneManager>();
    sceneManager.RemoveScene<MenuScene>();
}

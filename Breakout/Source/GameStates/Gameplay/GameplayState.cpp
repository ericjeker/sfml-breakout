// Copyright (c) Eric Jeker 2025.

#include "GameplayState.h"

#include "Scenes/Debug/DebugScene.h"
#include "Scenes/GameOver/GameOverScene.h"
#include "Scenes/GameWon/GameWonScene.h"
#include "Scenes/Gameplay/GameplayScene.h"
#include "Scenes/Pause/PauseScene.h"

#include "Core/GameStates/GameState.h"
#include "Core/Managers/GameService.h"
#include "Core/Managers/SceneManager.h"

GameplayState::GameplayState(flecs::world& world)
    : GameState(world)
{
}

void GameplayState::Enter()
{
    auto& sceneManager = GameService::Get<SceneManager>();

    // --- Add scenes to the game state ---
    sceneManager.AddScene<DebugScene>(std::make_unique<DebugScene>(GetWorld()));
    sceneManager.AddScene<GameplayScene>(std::make_unique<GameplayScene>(GetWorld()));
    sceneManager.AddScene<GameOverScene>(std::make_unique<GameOverScene>(GetWorld()));
    sceneManager.AddScene<GameWonScene>(std::make_unique<GameWonScene>(GetWorld()));
    sceneManager.AddScene<PauseScene>(std::make_unique<PauseScene>(GetWorld()));

    // --- Load the default scene ---
    sceneManager.LoadScene<GameplayScene>(SceneLoadMode::Single);
    sceneManager.LoadScene<DebugScene>(SceneLoadMode::Additive);
}

void GameplayState::Exit()
{
    // --- Clean-up scenes ---
    auto& sceneManager = GameService::Get<SceneManager>();
    sceneManager.RemoveScene<GameplayScene>();
    sceneManager.RemoveScene<PauseScene>();
    sceneManager.RemoveScene<DebugScene>();
}

void GameplayState::HandleEvent(const std::optional<sf::Event>& event)
{
    auto& sceneManager = GameService::Get<SceneManager>();

    if (event->is<sf::Event::FocusLost>())
    {
        //sceneManager.GetScene<GameplayScene>().Pause();
        if (!sceneManager.GetScene<PauseScene>().IsLoaded() && !sceneManager.GetScene<GameOverScene>().IsLoaded())
        {
            sceneManager.LoadScene<PauseScene>(SceneLoadMode::Additive);
        }
    }
    else if (event->is<sf::Event::FocusGained>())
    {
    }
}

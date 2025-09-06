// Copyright (c) Eric Jeker 2025.

#include "GameplayState.h"

#include "Components/CurrentLevel.h"
#include "Components/Lives.h"
#include "Components/MaxLevel.h"
#include "Components/Multiplier.h"
#include "Components/Score.h"
#include "Scenes/Debug/DebugScene.h"
#include "Scenes/GameOver/GameOverScene.h"
#include "Scenes/GameWon/GameWonScene.h"
#include "Scenes/Gameplay/GameplayScene.h"
#include "Scenes/Hud/HudScene.h"
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
    LOG_DEBUG("GameplayState::Enter");
    GameState::Enter();

    // --- Create the game session ---
    const auto& world = GetWorld();
    world.set<Score>({});
    world.set<Lives>({});
    world.set<Multiplier>({});
    world.set<CurrentLevel>({});
    world.set<MaxLevel>({2});

    auto& sceneManager = GameService::Get<SceneManager>();

    // --- Add scenes to the game state ---
    sceneManager.AddScene<DebugScene>(std::make_unique<DebugScene>(GetWorld()));
    sceneManager.AddScene<GameplayScene>(std::make_unique<GameplayScene>(GetWorld()));
    sceneManager.AddScene<GameOverScene>(std::make_unique<GameOverScene>(GetWorld()));
    sceneManager.AddScene<GameWonScene>(std::make_unique<GameWonScene>(GetWorld()));
    sceneManager.AddScene<PauseScene>(std::make_unique<PauseScene>(GetWorld()));
    sceneManager.AddScene<HudScene>(std::make_unique<HudScene>(GetWorld()));

    // --- Load the default scene ---
    sceneManager.LoadScene<GameplayScene>(SceneLoadMode::Single);
    sceneManager.LoadScene<HudScene>(SceneLoadMode::Additive);
    sceneManager.LoadScene<DebugScene>(SceneLoadMode::Additive);
}

void GameplayState::Exit()
{
    LOG_DEBUG("GameplayState::Exit");
    GameState::Exit();

    // --- Clean-up scenes ---
    auto& sceneManager = GameService::Get<SceneManager>();
    sceneManager.RemoveScene<DebugScene>();
    sceneManager.RemoveScene<GameplayScene>();
    sceneManager.RemoveScene<GameOverScene>();
    sceneManager.RemoveScene<GameWonScene>();
    sceneManager.RemoveScene<PauseScene>();
    sceneManager.RemoveScene<HudScene>();

    // --- Remove game session singletons ---
    const auto& world = GetWorld();
    world.remove<Score>();
    world.remove<Lives>();
    world.remove<Multiplier>();
    world.remove<CurrentLevel>();
    world.remove<MaxLevel>();
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

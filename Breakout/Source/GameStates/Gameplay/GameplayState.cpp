// Copyright (c) Eric Jeker 2025.

#include "GameplayState.h"

#include "Modules/Breakout/Singletons/CurrentLevel.h"
#include "Modules/Breakout/Singletons/Lives.h"
#include "Modules/Breakout/Singletons/MaxLevel.h"
#include "Modules/Breakout/Singletons/Multiplier.h"
#include "Modules/Breakout/Singletons/Score.h"
#include "Scenes/Debug/DebugScene.h"
#include "Scenes/GameOver/GameOverScene.h"
#include "Scenes/GameWon/GameWonScene.h"
#include "Scenes/Gameplay/GameplayScene.h"
#include "Scenes/Hud/HudScene.h"
#include "Scenes/Pause/PauseScene.h"

#include "Core/GameService.h"
#include "Core/GameStates/GameState.h"
#include "Core/Managers/SceneManager.h"

GameplayState::GameplayState(flecs::world& world)
    : GameState(world)
{
}

void GameplayState::Enter()
{
    GameState::Enter();

    // --- Create the game session ---
    const auto& world = GetWorld();

    // --- Initialize game session singletons ---
    world.set<Score>({0});
    world.set<Lives>({3});
    world.set<Multiplier>({1});
    world.set<CurrentLevel>({1});
    world.set<MaxLevel>({3});

    auto& sceneManager = GameService::Get<SceneManager>();

    // --- Add scenes to the game state ---
    sceneManager.AddScene<DebugScene>(std::make_unique<DebugScene>(GetWorld()));
    sceneManager.AddScene<GameplayScene>(std::make_unique<GameplayScene>(GetWorld()));
    sceneManager.AddScene<GameOverScene>(std::make_unique<GameOverScene>(GetWorld()));
    sceneManager.AddScene<GameWonScene>(std::make_unique<GameWonScene>(GetWorld()));
    sceneManager.AddScene<PauseScene>(std::make_unique<PauseScene>(GetWorld()));
    sceneManager.AddScene<HudScene>(std::make_unique<HudScene>(GetWorld()));

    // --- Load the default scenes ---
    sceneManager.LoadScene<GameplayScene>(SceneLoadMode::Single);
    sceneManager.LoadScene<HudScene>(SceneLoadMode::Additive);
    sceneManager.LoadScene<DebugScene>(SceneLoadMode::Additive);
}

void GameplayState::Exit()
{
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

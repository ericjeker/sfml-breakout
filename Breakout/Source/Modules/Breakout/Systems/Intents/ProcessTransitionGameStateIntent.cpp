// Copyright (c) Eric Jeker 2025.

#include "ProcessTransitionGameStateIntent.h"

#include "GameStates/Gameplay/GameplayState.h"
#include "GameStates/Menu/MenuState.h"
#include "Modules/Breakout/Components/Intents/TransitionGameStateIntent.h"
#include "Modules/Breakout/Singletons/CurrentLevel.h"
#include "Modules/Breakout/Singletons/GameStateGameLost.h"
#include "Modules/Breakout/Singletons/GameStateGameWon.h"
#include "Modules/Breakout/Singletons/GameStateMenu.h"
#include "Modules/Breakout/Singletons/GameStatePaused.h"
#include "Modules/Breakout/Singletons/GameStatePlaying.h"
#include "Modules/Breakout/Singletons/Lives.h"
#include "Modules/Breakout/Singletons/MaxLevel.h"
#include "Modules/Breakout/Singletons/Multiplier.h"
#include "Modules/Breakout/Singletons/Score.h"
#include "Modules/Breakout/Tasks/PauseGame.h"
#include "Modules/Breakout/Tasks/ResumeGame.h"
#include "Scenes/Debug/DebugScene.h"
#include "Scenes/GameOver/GameOverScene.h"
#include "Scenes/GameWon/GameWonScene.h"
#include "Scenes/Gameplay/GameplayScene.h"
#include "Scenes/Hud/HudScene.h"
#include "Scenes/Pause/PauseScene.h"

#include "Core/GameService.h"
#include "Core/Managers/GameStateManager.h"
#include "Core/Managers/SceneManager.h"
#include "Core/Modules/Window/Components/DeferredEvent.h"
#include "Core/Utils/Logger.h"

namespace
{

auto Update()
{
    return [](const flecs::entity& e, const TransitionGameStateIntent& intent) {
        const auto& world = e.world();

        // Remove all game states from the world
        world.remove<GameStatePlaying>();
        world.remove<GameStatePaused>();
        world.remove<GameStateMenu>();
        world.remove<GameStateGameWon>();
        world.remove<GameStateGameLost>();

        switch (intent.state)
        {
            case GameTransitions::OpenMenu:
                LOG_DEBUG("BreakoutModule::ProcessGameInstanceStateTransition -> OpenMenu");
                e.world().entity().set<DeferredEvent>({.callback = [](flecs::world& stage) {
                    GameService::Get<GameStateManager>().ChangeState(std::make_unique<MenuState>(stage));
                }});
                e.world().add<GameStateMenu>();
                break;
            case GameTransitions::StartPlaying:
                LOG_DEBUG("BreakoutModule::ProcessGameInstanceStateTransition -> StartPlaying");
                e.world().entity().set<DeferredEvent>({.callback = [](flecs::world& stage) {
                    GameService::Get<GameStateManager>().ChangeState(std::make_unique<GameplayState>(stage));
                }});
                e.world().add<GameStatePlaying>();
                break;
            case GameTransitions::RestartPlaying:
                LOG_DEBUG("BreakoutModule::ProcessGameInstanceStateTransition -> RestartPlaying");
                world.entity().set<DeferredEvent>({[](const flecs::world&) {
                    auto& sceneManager = GameService::Get<SceneManager>();
                    sceneManager.LoadScene<GameplayScene>(SceneLoadMode::Single);
                    sceneManager.LoadScene<DebugScene>(SceneLoadMode::Additive);
                    sceneManager.LoadScene<HudScene>(SceneLoadMode::Additive);
                }});

                // --- Reset the Game State ---
                world.set<Score>({0});
                world.set<Lives>({3});
                world.set<Multiplier>({1});
                world.set<CurrentLevel>({1});

                e.world().add<GameStatePlaying>();
                break;
            case GameTransitions::PauseGame:
                LOG_DEBUG("BreakoutModule::ProcessGameInstanceStateTransition -> PauseGame");
                e.world().entity().set<DeferredEvent>({[](const flecs::world&) {
                    auto& sceneManager = GameService::Get<SceneManager>();
                    sceneManager.LoadScene<PauseScene>(SceneLoadMode::Additive);
                }});

                PauseGame::Run(e.world());
                e.world().add<GameStatePaused>();
                break;
            case GameTransitions::ResumePlaying:
                LOG_DEBUG("BreakoutModule::ProcessGameInstanceStateTransition -> ResumePlaying");
                e.world().entity().set<DeferredEvent>({[](const flecs::world&) {
                    auto& sceneManager = GameService::Get<SceneManager>();
                    sceneManager.UnloadScene<PauseScene>();
                }});

                ResumeGame::Run(e.world());
                e.world().add<GameStatePlaying>();
                break;
            case GameTransitions::GameWon:
                LOG_DEBUG("BreakoutModule::ProcessGameInstanceStateTransition -> GameWon");
                e.world().entity().set<DeferredEvent>({[](const flecs::world&) {
                    auto& sceneManager = GameService::Get<SceneManager>();
                    sceneManager.LoadScene<GameWonScene>(SceneLoadMode::Additive);
                }});

                PauseGame::Run(e.world());
                e.world().add<GameStateGameWon>();
                break;
            case GameTransitions::GameLost:
                LOG_DEBUG("BreakoutModule::ProcessGameInstanceStateTransition -> GameLost");
                e.world().entity().set<DeferredEvent>({[](const flecs::world&) {
                    auto& sceneManager = GameService::Get<SceneManager>();
                    sceneManager.LoadScene<GameOverScene>(SceneLoadMode::Additive);
                }});

                PauseGame::Run(e.world());
                e.world().add<GameStateGameLost>();
                break;
        }

        e.destruct();
    };
}

} // namespace

void ProcessTransitionGameStateIntent::Register(const flecs::world& world)
{

    world.system<const TransitionGameStateIntent>("ProcessGameInstanceStateTransition")
        .write<GameStatePlaying>()
        .write<GameStatePaused>()
        .write<GameStateMenu>()
        .write<GameStateGameWon>()
        .write<GameStateGameLost>()
        .kind(flecs::PreUpdate)
        .each(Update());
}

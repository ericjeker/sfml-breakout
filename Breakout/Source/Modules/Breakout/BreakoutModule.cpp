// Copyright (c) Eric Jeker 2025.

#include "BreakoutModule.h"

#include "Components/Intents/TransitionGameStateIntent.h"
#include "GameStates/Gameplay/GameplayState.h"
#include "GameStates/Menu/MenuState.h"
#include "Modules/Breakout/Singletons/Lives.h"
#include "Modules/Breakout/Singletons/Multiplier.h"
#include "Modules/Breakout/Singletons/Score.h"
#include "Scenes/Debug/DebugScene.h"
#include "Scenes/GameOver/GameOverScene.h"
#include "Scenes/GameWon/GameWonScene.h"
#include "Scenes/Gameplay/GameplayScene.h"
#include "Scenes/Gameplay/Tasks/PauseGame.h"
#include "Scenes/Hud/HudScene.h"
#include "Scenes/Pause/PauseScene.h"
#include "Singletons/CurrentLevel.h"
#include "Singletons/GameStateGameLost.h"
#include "Singletons/GameStateGameWon.h"
#include "Singletons/GameStateMenu.h"
#include "Singletons/GameStatePaused.h"
#include "Singletons/GameStatePlaying.h"
#include "Singletons/MaxLevel.h"

#include "Core/GameService.h"
#include "Core/Managers/GameStateManager.h"
#include "Core/Modules/Window/Components/DeferredEvent.h"
#include "Core/Utils/Logger.h"

namespace
{

auto ProcessGameInstanceStateTransition()
{
    return [](const flecs::entity& e, const TransitionGameStateIntent& intent) {
        const auto& world = e.world();

        world.remove<GameStatePlaying>();
        world.remove<GameStatePaused>();
        world.remove<GameStateMenu>();
        world.remove<GameStateGameWon>();
        world.remove<GameStateGameLost>();

        switch (intent.state)
        {
            case GameTransitions::OpenMenu:
                LOG_DEBUG("BreakoutModule::ProcessGameInstanceStateTransition -> OpenMenu");
                e.world().entity().set<DeferredEvent>({.callback = [](flecs::world& world) {
                    GameService::Get<GameStateManager>().ChangeState(std::make_unique<MenuState>(world));
                }});
                e.world().add<GameStateMenu>();
                break;
            case GameTransitions::StartPlaying:
                LOG_DEBUG("BreakoutModule::ProcessGameInstanceStateTransition -> StartPlaying");
                e.world().entity().set<DeferredEvent>({.callback = [](flecs::world& world) {
                    GameService::Get<GameStateManager>().ChangeState(std::make_unique<GameplayState>(world));
                }});
                e.world().add<GameStatePlaying>();
                break;
            case GameTransitions::RestartPlaying:
                LOG_DEBUG("BreakoutModule::ProcessGameInstanceStateTransition -> RestartPlaying");
                world.entity().set<DeferredEvent>({[](const flecs::world& world) {
                    auto& sceneManager = GameService::Get<SceneManager>();
                    sceneManager.LoadScene<GameplayScene>(SceneLoadMode::Single);
                    sceneManager.LoadScene<DebugScene>(SceneLoadMode::Additive);
                    sceneManager.LoadScene<HudScene>(SceneLoadMode::Additive);
                }});

                world.set<Score>({.score = 0});
                world.set<Lives>({3});
                world.set<Multiplier>({1});
                world.set<CurrentLevel>({1});

                e.world().add<GameStatePlaying>();
                break;
            case GameTransitions::PauseGame:
                LOG_DEBUG("BreakoutModule::ProcessGameInstanceStateTransition -> PauseGame");
                e.world().entity().set<DeferredEvent>({[](const flecs::world& world) {
                    auto& sceneManager = GameService::Get<SceneManager>();
                    sceneManager.LoadScene<PauseScene>(SceneLoadMode::Additive);
                }});
                e.world().add<GameStatePaused>();
                break;
            case GameTransitions::ResumePlaying:
                LOG_DEBUG("BreakoutModule::ProcessGameInstanceStateTransition -> ResumePlaying");
                e.world().entity().set<DeferredEvent>({[](const flecs::world& world) {
                    auto& sceneManager = GameService::Get<SceneManager>();
                    sceneManager.UnloadScene<PauseScene>();
                }});
                e.world().add<GameStatePlaying>();
                break;
            case GameTransitions::GameWon:
                LOG_DEBUG("BreakoutModule::ProcessGameInstanceStateTransition -> GameWon");
                e.world().entity().set<DeferredEvent>({[](const flecs::world& world) {
                    auto& sceneManager = GameService::Get<SceneManager>();
                    sceneManager.LoadScene<GameWonScene>(SceneLoadMode::Additive);
                }});
                e.world().add<GameStateGameWon>();
                break;
            case GameTransitions::GameLost:
                LOG_DEBUG("BreakoutModule::ProcessGameInstanceStateTransition -> GameLost");
                e.world().entity().set<DeferredEvent>({[](const flecs::world& world) {
                    auto& sceneManager = GameService::Get<SceneManager>();
                    sceneManager.LoadScene<GameOverScene>(SceneLoadMode::Additive);
                }});
                e.world().add<GameStateGameLost>();
                break;
        }

        e.destruct();
    };
}

} // namespace

namespace Modules
{

BreakoutModule::BreakoutModule(const flecs::world& world)
{
    // --- Register Singletons ---
    world.singleton<GameStatePlaying>();
    world.singleton<GameStatePaused>();
    world.singleton<GameStateMenu>();
    world.singleton<GameStateGameWon>();
    world.singleton<GameStateGameLost>();

    world.singleton<Score>();
    world.singleton<Lives>();
    world.singleton<Multiplier>();
    world.singleton<CurrentLevel>();
    world.singleton<MaxLevel>();

    // --- Register Components ---
    world.component<TransitionGameStateIntent>();

    // --- Initialize Singletons ---
    world.add<GameStateMenu>();

    // --- Register Systems ---
    world.system<const TransitionGameStateIntent>("ProcessGameInstanceStateTransition")
        .write<GameStatePlaying>()
        .write<GameStatePaused>()
        .write<GameStateMenu>()
        .write<GameStateGameWon>()
        .write<GameStateGameLost>()
        .kind(flecs::PreUpdate)
        .each(ProcessGameInstanceStateTransition());
}

} // namespace Modules

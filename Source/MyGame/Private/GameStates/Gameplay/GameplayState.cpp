// Copyright (c) Eric Jeker 2025.

#include "GameplayState.h"

#include "Core/Logger.h"
#include "Core/Managers/SceneManager.h"
#include "Events/ResumeGame.h"
#include "Scenes/ControllerDemoScene.h"
#include "Scenes/DebugScene.h"
#include "Scenes/PauseScene.h"

void GameplayState::Enter()
{
    auto& sceneManager = GameService::Get<SceneManager>();

    LOG_DEBUG("(GameplayState::GameplayState): Adding scenes to the SceneManager");
    sceneManager.AddScene<ControllerDemoScene>(std::make_unique<ControllerDemoScene>());
    sceneManager.AddScene<PauseScene>(std::make_unique<PauseScene>());
    sceneManager.AddScene<DebugScene>(std::make_unique<DebugScene>());

    LOG_DEBUG("(GameplayState::Enter): Loading Controller");
    sceneManager.LoadScene<ControllerDemoScene>(SceneLoadMode::Single);
    sceneManager.LoadScene<DebugScene>(SceneLoadMode::Additive);

    // --- Add the Listeners ---
    GameService::Get<EventManager>().Subscribe<ResumeGame>(_resumeGameListener);
}

void GameplayState::Exit()
{
    LOG_DEBUG("(GameplayState::Exit): Clean up scene manager and unsubscribe");
    auto& sceneManager = GameService::Get<SceneManager>();
    sceneManager.RemoveScene<ControllerDemoScene>();
    sceneManager.RemoveScene<PauseScene>();
    sceneManager.RemoveScene<DebugScene>();
}

void GameplayState::HandleEvent(const std::optional<sf::Event>& event)
{
    auto& sceneManager = GameService::Get<SceneManager>();

    if (event->is<sf::Event::FocusLost>())
    {
        sceneManager.GetScene<ControllerDemoScene>().Pause();
        sceneManager.LoadScene<PauseScene>(SceneLoadMode::Additive);
    }
    else if (event->is<sf::Event::FocusGained>())
    {
    }
}

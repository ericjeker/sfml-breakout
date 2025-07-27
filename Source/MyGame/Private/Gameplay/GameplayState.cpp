// Copyright (c) Eric Jeker 2025.

#include "../Events/RequestGameResume.h"
#include "../Scenes/DebugScene.h"
#include "../Scenes/PauseScene.h"
#include "GameplayState.h"
#include "Logger.h"
#include "Managers/SceneManager.h"

void GameplayState::Enter()
{
    auto& sceneManager = GameService::Get<SceneManager>();

    LOG_DEBUG("(GameplayState::GameplayState): Adding scenes to the SceneManager");
    sceneManager.AddScene<BouncingBallScene>(std::make_unique<BouncingBallScene>());
    sceneManager.AddScene<PauseScene>(std::make_unique<PauseScene>());
    sceneManager.AddScene<DebugScene>(std::make_unique<DebugScene>());

    LOG_DEBUG("(GameplayState::Enter): Loading BouncingBallScene");
    sceneManager.LoadScene<BouncingBallScene>(SceneLoadMode::Single);
    sceneManager.LoadScene<DebugScene>(SceneLoadMode::Additive);

    // --- Add the Listeners ---
    GameService::Get<EventManager>().Subscribe<RequestGameResume>(_resumeGameListener);
}

void GameplayState::Exit()
{
    LOG_DEBUG("(GameplayState::Exit): Clean up scene manager and unsubscribe");
    auto& sceneManager = GameService::Get<SceneManager>();
    sceneManager.RemoveScene<BouncingBallScene>();
    sceneManager.RemoveScene<PauseScene>();
    sceneManager.RemoveScene<DebugScene>();

    // GameService::Get<EventManager>().Unsubscribe<RequestGameResume>(_resumeGameListener);
}

void GameplayState::HandleEvent(const std::optional<sf::Event>& event, sf::RenderWindow& window)
{
    auto& sceneManager = GameService::Get<SceneManager>();

    if (event->is<sf::Event::FocusLost>())
    {
        sceneManager.GetScene<BouncingBallScene>().Pause();
        sceneManager.LoadScene<PauseScene>(SceneLoadMode::Additive);
        _isPaused = true;
    }
    else if (event->is<sf::Event::FocusGained>())
    {
    }
    else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
    {
        if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
        {
            if (_isPaused)
            {
                sceneManager.UnloadScene<PauseScene>();
                sceneManager.GetScene<BouncingBallScene>().Resume();
                _isPaused = false;
            }
            else
            {
                sceneManager.GetScene<BouncingBallScene>().Pause();
                sceneManager.LoadScene<PauseScene>(SceneLoadMode::Additive);
                _isPaused = true;
            }
        }
    }
}

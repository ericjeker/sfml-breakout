// Copyright (c) Eric Jeker 2025.

#include "MainMenuState.h"

#include "../Events/RequestGameExit.h"
#include "../Events/RequestGameResume.h"
#include "../Scenes/DebugScene.h"
#include "../Scenes/PauseScene.h"
#include "Logger.h"
#include "Managers/SceneManager.h"

MainMenuState::MainMenuState()
{
    auto& sceneManager = GameService::Get<SceneManager>();

    LOG_DEBUG("(MainMenuState::MainMenuState): Adding scenes to the SceneManager");
    sceneManager.AddScene<BouncingBallScene>(std::make_unique<BouncingBallScene>());
    // sceneManager.AddScene<MiniRtsScene>(std::make_unique<MiniRtsScene>());
    sceneManager.AddScene<PauseScene>(std::make_unique<PauseScene>());
    sceneManager.AddScene<DebugScene>(std::make_unique<DebugScene>());
}

void MainMenuState::Enter()
{
    LOG_DEBUG("(MainMenuState::Enter): Loading BouncingBallScene");
    GameService::Get<SceneManager>().LoadScene<BouncingBallScene>(SceneLoadMode::Single);
    GameService::Get<SceneManager>().LoadScene<DebugScene>(SceneLoadMode::Additive);

    // --- Add the Listeners ---
    GameService::Get<EventManager>().Subscribe<RequestGameResume>(_resumeGameListener);
    GameService::Get<EventManager>().Subscribe<RequestGameExit>(_exitGameListener);
}

void MainMenuState::Exit()
{
    LOG_DEBUG("(MainMenuState::Exit)");
    GameService::Get<SceneManager>().CleanUp();
    // GameService::Get<EventManager>().Unsubscribe<RequestGameResume>(_resumeGameListener);
    // GameService::Get<EventManager>().Unsubscribe<RequestGameExit>(_exitGameListener);
}

void MainMenuState::Update(float deltaTime)
{
}

void MainMenuState::HandleEvent(const std::optional<sf::Event>& event)
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

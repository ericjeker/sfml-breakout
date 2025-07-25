// Copyright (c) Eric Jeker 2025.

#include "MainMenuState.h"

#include "../Events/ExitGameRequestedEvent.h"
#include "../Events/ResumeGameRequestedEvent.h"
#include "../Scenes/DebugScene.h"
#include "../Scenes/DemoScene.h"
#include "../Scenes/PauseScene.h"
#include "Logger.h"
#include "Managers/SceneManager.h"

MainMenuState::MainMenuState()
{
    auto& sceneManager = GameService::Get<SceneManager>();

    LOG_DEBUG("(MainMenuState::MainMenuState): Adding scenes to the SceneManager");
    sceneManager.AddScene<BouncingBallFlecsScene>(std::make_unique<BouncingBallFlecsScene>());
    sceneManager.AddScene<MiniRtsScene>(std::make_unique<MiniRtsScene>());
    sceneManager.AddScene<DemoScene>(std::make_unique<DemoScene>());
    sceneManager.AddScene<PauseScene>(std::make_unique<PauseScene>());
    sceneManager.AddScene<DebugScene>(std::make_unique<DebugScene>());
}

void MainMenuState::Enter()
{
    LOG_DEBUG("(MainMenuState::Enter): Loading BouncingBallFlecsScene");
    GameService::Get<SceneManager>().LoadScene<BouncingBallFlecsScene>(SceneLoadMode::Single);
    GameService::Get<SceneManager>().LoadScene<DebugScene>(SceneLoadMode::Additive);

    // --- Add the Listeners ---
    GameService::Get<EventManager>().Subscribe<ResumeGameRequestedEvent>(_resumeGameListener);
    GameService::Get<EventManager>().Subscribe<ExitGameRequestedEvent>(_exitGameListener);
}

void MainMenuState::Exit()
{
    LOG_DEBUG("(MainMenuState::Exit)");
    GameService::Get<SceneManager>().CleanUp();
    // GameService::Get<EventManager>().Unsubscribe<ResumeGameRequestedEvent>(_resumeGameListener);
    // GameService::Get<EventManager>().Unsubscribe<ExitGameRequestedEvent>(_exitGameListener);
}

void MainMenuState::Update(float deltaTime)
{
}

void MainMenuState::HandleEvent(const std::optional<sf::Event>& event, sf::RenderWindow& window)
{
    auto& sceneManager = GameService::Get<SceneManager>();

    if (event->is<sf::Event::FocusLost>())
    {
        sceneManager.GetScene<BouncingBallFlecsScene>().Pause();
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
                sceneManager.GetScene<BouncingBallFlecsScene>().Resume();
                _isPaused = false;
            }
            else
            {
                sceneManager.GetScene<BouncingBallFlecsScene>().Pause();
                sceneManager.LoadScene<PauseScene>(SceneLoadMode::Additive);
                _isPaused = true;
            }
        }
    }
}

// Copyright (c) Eric Jeker 2025.

#include "MainMenuState.h"

#include "../Events/ExitGameRequestedEvent.h"
#include "../Events/ResumeGameRequestedEvent.h"
#include "../Scenes/DebugScene.h"
#include "../Scenes/DemoScene.h"
#include "../Scenes/PauseScene.h"
#include "Logger.h"
#include "Managers/SceneManager.h"

MainMenuState::MainMenuState(GameService& gameService)
    : GameState(gameService)
{
    auto& sceneManager = GetGameService().Get<SceneManager>();
    auto& resourceManager = GetGameService().Get<ResourceManager>();
    auto& eventManager = GetGameService().Get<EventManager>();

    LOG_DEBUG("(MainMenuState::MainMenuState): Adding scenes to the SceneManager");
    sceneManager.AddScene<BouncingBallFlecsScene>(std::make_unique<BouncingBallFlecsScene>(resourceManager, eventManager));
    sceneManager.AddScene<MiniRtsScene>(std::make_unique<MiniRtsScene>(resourceManager, eventManager));
    sceneManager.AddScene<DemoScene>(std::make_unique<DemoScene>(resourceManager, eventManager));
    sceneManager.AddScene<PauseScene>(std::make_unique<PauseScene>(resourceManager, eventManager));
    sceneManager.AddScene<DebugScene>(std::make_unique<DebugScene>(resourceManager, eventManager));
}

void MainMenuState::Enter()
{
    LOG_DEBUG("(MainMenuState::Enter): Loading BouncingBallFlecsScene");
    GetGameService().Get<SceneManager>().LoadScene<BouncingBallFlecsScene>(SceneLoadMode::Single);
    GetGameService().Get<SceneManager>().LoadScene<DebugScene>(SceneLoadMode::Additive);

    // --- Add the Listeners ---
    GetGameService().Get<EventManager>().Subscribe<ResumeGameRequestedEvent>(_resumeGameListener);
    GetGameService().Get<EventManager>().Subscribe<ExitGameRequestedEvent>(_exitGameListener);
}

void MainMenuState::Exit()
{
    LOG_DEBUG("(MainMenuState::Exit)");
    GetGameService().Get<SceneManager>().CleanUp();
    // GetGameService().Get<EventManager>().Unsubscribe<ResumeGameRequestedEvent>(_resumeGameListener);
    // GetGameService().Get<EventManager>().Unsubscribe<ExitGameRequestedEvent>(_exitGameListener);
}

void MainMenuState::Update(float deltaTime)
{
}

void MainMenuState::HandleEvent(const std::optional<sf::Event>& event, sf::RenderWindow& window)
{
    auto& sceneManager = GetGameService().Get<SceneManager>();

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

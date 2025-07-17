// Copyright (c) Eric Jeker 2025.

#include "MainMenuState.h"

#include "../Scenes/BouncingBallScene.h"
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
    sceneManager.AddScene<BouncingBallScene>(std::make_unique<BouncingBallScene>(resourceManager, eventManager));
    sceneManager.AddScene<DemoScene>(std::make_unique<DemoScene>(resourceManager, eventManager));
    sceneManager.AddScene<PauseScene>(std::make_unique<PauseScene>(resourceManager, eventManager));
    sceneManager.AddScene<DebugScene>(std::make_unique<DebugScene>(resourceManager, eventManager));
}

void MainMenuState::Enter()
{
    LOG_DEBUG("(MainMenuState::Enter): Loading BouncingBallScene");
    GetGameService().Get<SceneManager>().LoadScene<BouncingBallScene>(SceneLoadMode::Single);
    GetGameService().Get<SceneManager>().LoadScene<DebugScene>(SceneLoadMode::Additive);
}

void MainMenuState::Exit()
{
    LOG_DEBUG("(MainMenuState::Exit)");
    GetGameService().Get<SceneManager>().CleanUp();
}

void MainMenuState::Update(float deltaTime)
{
}

void MainMenuState::HandleEvent(const std::optional<sf::Event>& event, sf::RenderWindow& window)
{
    auto& sceneManager = GetGameService().Get<SceneManager>();

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

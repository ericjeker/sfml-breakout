// Copyright (c) Eric Jeker 2025.

#include "MainMenuState.h"

#include "../Scenes/DemoScene.h"
#include "../Scenes/PauseScene.h"
#include "Logger.h"
#include "Managers/SceneManager.h"

MainMenuState::MainMenuState(GameService& gameService)
    : GameState(gameService)
{
    LOG_DEBUG("(MainMenuState::MainMenuState): Adding DemoScene to the SceneManager");
    auto& sceneManager = GetGameService().Get<SceneManager>();
    auto& resourceManager = GetGameService().Get<ResourceManager>();
    auto& eventManager = GetGameService().Get<EventManager>();

    sceneManager.AddScene<DemoScene>(std::make_unique<DemoScene>(resourceManager, eventManager));
    sceneManager.AddScene<PauseScene>(std::make_unique<PauseScene>(resourceManager, eventManager));
}

void MainMenuState::Enter()
{
    LOG_DEBUG("(MainMenuState::Enter): Loading DemoScene");
    GetGameService().Get<SceneManager>().LoadScene<DemoScene>(SceneLoadMode::Single);
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
    if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
    {
        if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
        {
            auto& sceneManager = GetGameService().Get<SceneManager>();
            if (_isPaused)
            {
                sceneManager.UnloadScene<PauseScene>();
                sceneManager.GetScene<DemoScene>().Resume();
                _isPaused = false;
            }
            else
            {
                sceneManager.GetScene<DemoScene>().Pause();
                sceneManager.LoadScene<PauseScene>(SceneLoadMode::Additive);
                _isPaused = true;
            }
        }
    }
}

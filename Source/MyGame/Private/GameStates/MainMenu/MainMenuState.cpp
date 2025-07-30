// Copyright (c) Eric Jeker 2025.

#include "MainMenuState.h"

#include "GameInstance.h"
#include "Logger.h"
#include "Managers/GameService.h"
#include "Managers/SceneManager.h"
#include "Scenes/MainMenuScene.h"

void MainMenuState::Enter()
{
    LOG_DEBUG("(MainMenuState::MainMenuState): Adding scenes to the SceneManager");
    auto& sceneManager = GameService::Get<SceneManager>();
    sceneManager.AddScene<MainMenuScene>(std::make_unique<MainMenuScene>());

    LOG_DEBUG("(MainMenuState::Enter): Loading MainMenuScene");
    sceneManager.LoadScene<MainMenuScene>(SceneLoadMode::Single);
}

void MainMenuState::Exit()
{
    LOG_DEBUG("(MainMenuState::Exit)");
    auto& sceneManager = GameService::Get<SceneManager>();
    sceneManager.RemoveScene<MainMenuScene>();
}

void MainMenuState::HandleEvent(const std::optional<sf::Event>& event)
{
    if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
    {
        if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
        {
            GameService::Get<GameInstance>().RequestExit();
        }
    }
}

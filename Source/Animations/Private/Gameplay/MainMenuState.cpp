// Copyright (c) Eric Jeker 2025.

#include "MainMenuState.h"

#include "../Scenes/DemoScene.h"
#include "Logger.h"

MainMenuState::MainMenuState(SceneManager& sceneManager)
    : _sceneManager(sceneManager)
{
}

void MainMenuState::Enter()
{
    LOG_DEBUG("(MainMenuState::Enter): Adding DemoScene to the SceneManager");
    _sceneManager.AddScene(std::make_unique<DemoScene>());
}

void MainMenuState::Exit()
{
    LOG_DEBUG("(MainMenuState::Exit)");
}

void MainMenuState::Update(float deltaTime)
{
}

void MainMenuState::HandleEvent(const std::optional<sf::Event>& event)
{
    if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
    {
        if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
        {
            // TODO: Close the window
        }
    }
}

// Copyright (c) Eric Jeker 2025.

#include "MainMenuState.h"

#include "../Scenes/DemoScene.h"
#include "Logger.h"

MainMenuState::MainMenuState(SceneManager& sceneManager)
    : _sceneManager(sceneManager)
{
    LOG_DEBUG("(MainMenuState::MainMenuState): Adding DemoScene to the SceneManager");
    _sceneManager.AddScene<DemoScene>(std::make_unique<DemoScene>());
}

void MainMenuState::Enter()
{
    LOG_DEBUG("(MainMenuState::Enter): Loading DemoScene");
    _sceneManager.LoadScene<DemoScene>(SceneLoadMode::Single);
}

void MainMenuState::Exit()
{
    LOG_DEBUG("(MainMenuState::Exit)");
    _sceneManager.UnloadScene<DemoScene>();
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
        }
    }
}

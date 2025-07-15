// Copyright (c) Eric Jeker 2025.

#include "MainMenuState.h"

#include "../Scenes/BlueDemoScene.h"
#include "../Scenes/RedDemoScene.h"
#include "Logger.h"

MainMenuState::MainMenuState(SceneManager& sceneManager)
    : _sceneManager(sceneManager)
{
    LOG_DEBUG("(MainMenuState::MainMenuState): Adding RedDemoScene to the SceneManager");
    _sceneManager.AddScene<RedDemoScene>(std::make_unique<RedDemoScene>());
    _sceneManager.AddScene<BlueDemoScene>(std::make_unique<BlueDemoScene>());
}

void MainMenuState::Enter()
{
    LOG_DEBUG("(MainMenuState::Enter): Loading RedDemoScene");
    _sceneManager.LoadScene<RedDemoScene>(SceneLoadMode::Single);
}

void MainMenuState::Exit()
{
    LOG_DEBUG("(MainMenuState::Exit)");
    _sceneManager.UnloadScene<RedDemoScene>();
}

void MainMenuState::Update(float deltaTime)
{
}

void MainMenuState::HandleEvent(const std::optional<sf::Event>& event)
{
    if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
    {
        if (keyPressed->scancode == sf::Keyboard::Scancode::Right)
        {
            _sceneManager.LoadScene<BlueDemoScene>(SceneLoadMode::Additive);
        }
        else if (keyPressed->scancode == sf::Keyboard::Scancode::Left)
        {
            _sceneManager.UnloadScene<BlueDemoScene>();
        }
    }
}

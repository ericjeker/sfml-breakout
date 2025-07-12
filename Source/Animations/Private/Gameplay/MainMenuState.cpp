// Copyright (c) Eric Jeker 2025.

#include "MainMenuState.h"

#include "Logger.h"

MainMenuState::MainMenuState(SceneManager& sceneManager)
    : _sceneManager(sceneManager)
{
}

void MainMenuState::Enter()
{
    LOG_DEBUG("(MainMenuState::Enter): Entering MainMenuState");
}

void MainMenuState::Exit()
{
    LOG_DEBUG("(MainMenuState::Exit)");
}

void MainMenuState::Update(float deltaTime)
{
}

void MainMenuState::Render(sf::RenderWindow& window)
{
}

void MainMenuState::HandleEvent(const sf::Event& event)
{
}

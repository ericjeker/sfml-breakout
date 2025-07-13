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
    auto scene = std::make_unique<DemoScene>();
    _sceneManager.AddScene(std::move(scene));
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
}

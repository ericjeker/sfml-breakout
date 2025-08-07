// Copyright (c) Eric Jeker 2025.

#include "MainMenuState.h"

#include "Core/GameInstance.h"
#include "Core/Logger.h"
#include "Core/Managers/GameService.h"
#include "Core/Managers/SceneManager.h"
#include "Core/Modules/UI/Components/MouseReleased.h"
#include "Core/Modules/UI/Prefabs/MouseReleasedEvent.h"
#include "Scenes/MainMenuScene.h"

MainMenuState::MainMenuState(flecs::world& world)
    : GameState(world)
{
}

void MainMenuState::Enter()
{
    // TODO: Initialize State Global Systems

    LOG_DEBUG("(MainMenuState::MainMenuState): Adding scenes to the SceneManager");
    auto& sceneManager = GameService::Get<SceneManager>();
    sceneManager.AddScene<MainMenuScene>(std::make_unique<MainMenuScene>(GetWorld()));

    LOG_DEBUG("(MainMenuState::Enter): LoingBallSceneading MainMenuScene");
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
    else if (auto* mouseReleased = event->getIf<sf::Event::MouseButtonReleased>())
    {
        if (mouseReleased->button == sf::Mouse::Button::Left)
        {
            GetWorld().entity().is_a<MouseReleasedEvent>().set<MouseReleased>(
                {.position = mouseReleased->position, .button = mouseReleased->button}
            );
        }
    }
}

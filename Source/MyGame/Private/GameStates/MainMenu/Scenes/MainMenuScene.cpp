// Copyright (c) Eric Jeker 2025.

#include "MainMenuScene.h"

#include "Core/Configuration.h"
#include "Core/GameInstance.h"
#include "Core/Logger.h"
#include "Core/Managers/EventManager.h"
#include "Core/Managers/GameService.h"
#include "Core/Managers/ResourceManager.h"
#include "Core/Modules/Render/Components/Size.h"
#include "Core/Modules/Render/Components/TextRenderable.h"
#include "Core/Modules/Render/Components/Transform.h"
#include "Core/Modules/Render/Prefabs/Rectangle.h"
#include "Core/Modules/UI/Components/Clickable.h"
#include "Core/Modules/UI/Components/EventTrigger.h"
#include "Core/Modules/UI/Components/Interactable.h"
#include "Core/Modules/UI/Components/MousePosition.h"
#include "Core/Modules/UI/Components/MousePressed.h"
#include "Core/Modules/UI/Components/MouseReleased.h"
#include "Core/Modules/UI/Prefabs/Button.h"
#include "Core/Modules/UI/Prefabs/MouseReleasedEvent.h"
#include "Core/Modules/UI/Prefabs/Text.h"
#include "Core/Themes/Nord.h"
#include "Events/ExitGame.h"
#include "Events/StartGame.h"


MainMenuScene::MainMenuScene(flecs::world& world)
    : Scene(world)
{
}

void MainMenuScene::Initialize()
{
    Scene::Initialize();
    LOG_DEBUG("(MainMenuScene:Initialize)");

    constexpr float centerX = Configuration::WINDOW_SIZE.x / 2;
    constexpr float centerY = Configuration::WINDOW_SIZE.y / 2;

    const auto& world = GetWorld();

    float zOrder = 0;

    // --- Create Background ---
    Prefabs::Rectangle::Create(
        world,
        {
            .size = sf::Vector2f{Configuration::WINDOW_SIZE},
            .color = NordTheme::PolarNight4,
            .position = {0.f, 0.f},
            .zOrder = zOrder++,
        }
    )
        .child_of(GetRootEntity());

    // --- Add Title ---
    Prefabs::Text::Create(
        world,
        {.text = "Main Menu",
         .fontAsset = "Orbitron-Bold",
         .fontSize = 60.f,
         .textColor = NordTheme::SnowStorm3,
         .origin = sf::Vector2f{0.5f, 0.5f},
         .position = {centerX, centerY - 200},
         .zOrder = zOrder++}
    )
        .child_of(GetRootEntity());

    // --- Add Play Button ---
    Prefabs::Button::Create(
        world,
        {
            .text = "Play",
            .fontAsset = "Orbitron-Bold",
            .fontSize = 48.f,
            .textColor = NordTheme::SnowStorm3,
            .backgroundColor = sf::Color::Transparent,
            .position = {centerX, centerY},
            .zOrder = zOrder++,
            .onClick = [this]() { GameService::Get<EventManager>().EmitDeferred<StartGame>({}, this); },
        }
    )
        .child_of(GetRootEntity());

    // --- Add Exit Button ---
    Prefabs::Button::Create(
        world,
        {
            .text = "Exit",
            .fontAsset = "Orbitron-Regular",
            .fontSize = 36.f,
            .textColor = NordTheme::SnowStorm3,
            .backgroundColor = sf::Color::Transparent,
            .position = {centerX, centerY + 100},
            .zOrder = zOrder++,
            .onClick = [this]() { GameService::Get<EventManager>().EmitDeferred<ExitGame>({}, this); },
        }
    )
        .child_of(GetRootEntity());

}

void MainMenuScene::HandleEvent(const std::optional<sf::Event>& event)
{
    if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
    {
        if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
        {
            GameService::Get<GameInstance>().RequestExit();
        }
    }
    else if (const auto* mouseReleased = event->getIf<sf::Event::MouseButtonReleased>())
    {
        if (mouseReleased->button == sf::Mouse::Button::Left)
        {
            GetWorld().entity().is_a<MouseReleasedEvent>().set<MouseReleased>(
                {.position = mouseReleased->position, .button = mouseReleased->button}
            );
        }
    }
}

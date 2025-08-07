// Copyright (c) Eric Jeker 2025.

#include "PauseScene.h"

#include "Core/Configuration.h"
#include "Core/Logger.h"
#include "Core/Managers/EventManager.h"
#include "Core/Managers/GameService.h"
#include "Core/Managers/ResourceManager.h"
#include "Core/Modules/Render/Components/Size.h"
#include "Core/Modules/Render/Components/Transform.h"
#include "Core/Modules/Render/Prefabs/Rectangle.h"
#include "Core/Modules/Render/RenderModule.h"
#include "Core/Modules/UI/Components/Clickable.h"
#include "Core/Modules/UI/Components/EventTrigger.h"
#include "Core/Modules/UI/Components/Interactable.h"
#include "Core/Modules/UI/Components/MouseReleased.h"
#include "Core/Modules/UI/Prefabs/Button.h"
#include "Core/Modules/UI/Prefabs/MouseReleasedEvent.h"
#include "Core/Modules/UI/Prefabs/Text.h"
#include "Core/Themes/Nord.h"
#include "Events/NavigateToMainMenu.h"
#include "Events/ResumeGame.h"

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>


PauseScene::PauseScene(flecs::world& world)
    : Scene(world)
{
}


void PauseScene::Initialize()
{
    Scene::Initialize();
    LOG_DEBUG("(PauseScene:Initialize)");

    constexpr float CENTER_X = Configuration::WINDOW_SIZE.x / 2;
    constexpr float CENTER_Y = Configuration::WINDOW_SIZE.y / 2;

    const auto& world = GetWorld();

    float zOrder = 0.f;

    // --- Overlay ---
    auto backgroundColor = NordTheme::PolarNight1;
    backgroundColor.a = 127;

    Prefabs::Rectangle::Create(
        world,
        {.size = sf::Vector2f{Configuration::WINDOW_SIZE},
         .color = backgroundColor,
         .position = {0.f, 0.f},
         .scale = {1.f, 1.f},
         .rotation = 0.f}
    )
        .child_of(GetRootEntity());

    // --- Add Pause Text ---
    Prefabs::Text::Create(
        world,
        {.text = "Game Paused",
         .fontAsset = "Orbitron-Bold",
         .fontSize = 60.f,
         .textColor = NordTheme::SnowStorm3,
         .origin = sf::Vector2f{0.5f, 0.5f},
         .position = {CENTER_X, CENTER_Y - 200},
         .zOrder = ++zOrder}
    )
        .child_of(GetRootEntity());

    // --- Add Resume Button ---
    Prefabs::Button::Create(
        world,
        {
            .text = "Resume",
            .fontAsset = "Orbitron-Bold",
            .fontSize = 48.f,
            .textColor = NordTheme::SnowStorm3,
            .backgroundColor = sf::Color::Transparent,
            .position = {CENTER_X, CENTER_Y},
            .zOrder = ++zOrder,
            .onClick = [this]() { GameService::Get<EventManager>().EmitDeferred<ResumeGame>({}, this); },
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
            .position = {CENTER_X, CENTER_Y + 100},
            .zOrder = ++zOrder,
            .onClick = [this]() { GameService::Get<EventManager>().EmitDeferred<NavigateToMainMenu>({}, this); },
        }
    )
        .child_of(GetRootEntity());
}

void PauseScene::HandleEvent(const std::optional<sf::Event>& event)
{
    if (!IsLoaded() || IsPaused())
    {
        return;
    }

    if (const auto* mouseReleased = event->getIf<sf::Event::MouseButtonReleased>())
    {
        if (mouseReleased->button != sf::Mouse::Button::Left)
        {
            return;
        }

        GetWorld().entity().is_a<MouseReleasedEvent>().set<MouseReleased>(
            {.position = mouseReleased->position, .button = mouseReleased->button}
        );
    }
}

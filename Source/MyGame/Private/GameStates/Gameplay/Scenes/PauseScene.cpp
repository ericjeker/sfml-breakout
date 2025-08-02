// Copyright (c) Eric Jeker 2025.

#include "PauseScene.h"

#include "../../../../../Core/Public/Modules/UI/Components/EventTrigger.h"
#include "Configuration.h"
#include "Events/NavigateToMainMenu.h"
#include "Events/ResumeGame.h"
#include "Logger.h"
#include "Managers/EventManager.h"
#include "Managers/GameService.h"
#include "Managers/ResourceManager.h"
#include "Modules/Render/Components/Size.h"
#include "Modules/Render/Components/Transform.h"
#include "Modules/Render/Prefabs/Rectangle.h"
#include "Modules/Render/RenderModule.h"
#include "Modules/UI/Components/Clickable.h"
#include "Modules/UI/Components/Interactable.h"
#include "Modules/UI/Prefabs/Button.h"
#include "Modules/UI/Prefabs/Text.h"
#include "Themes/Nord.h"

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>


void PauseScene::Initialize()
{
    LOG_DEBUG("(PauseScene:Initialize)");
    constexpr float CENTER_X = Configuration::WINDOW_SIZE.x / 2;
    constexpr float CENTER_Y = Configuration::WINDOW_SIZE.y / 2;

    auto world = GetWorld();
    world.import <Modules::RenderModule>();

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
    );

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
    );

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
    );

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
    );
}

void PauseScene::HandleEvent(const std::optional<sf::Event>& event)
{
    if (!IsLoaded() || IsPaused())
    {
        return;
    }

    if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>())
    {
        if (mousePressed->button != sf::Mouse::Button::Left)
        {
            return;
        }

        const sf::Vector2<float> mousePosition(mousePressed->position);

        GetWorld().each(
            [&](const Interactable& interactable,
                const Clickable& clickable,
                const Transform& t,
                const Size& s,
                const EventTrigger& eventTrigger)
            {
                sf::FloatRect bounds;
                bounds.size = s.size;
                bounds.position = t.position;

                if (bounds.contains(mousePosition))
                {
                    eventTrigger.callback();
                }
            }
        );
    }
}

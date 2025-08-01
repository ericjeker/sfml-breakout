// Copyright (c) Eric Jeker 2025.

#include "PauseScene.h"

#include "Events/NavigateToMainMenu.h"
#include "Events/ResumeGame.h"
#include "Managers/EventManager.h"
#include "Managers/GameService.h"
#include "Managers/ResourceManager.h"
#include "Modules/Physics/Components/Transform.h"
#include "Modules/Render/Components/RectangleRenderable.h"
#include "Modules/Render/Components/TextRenderable.h"
#include "Modules/Render/RenderModule.h"
#include "Modules/UI/Prefabs/Button.h"
#include "Modules/UI/Prefabs/Text.h"

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>

#include <Logger.h>
#include <Themes/Nord.h>

#include <Components/EventTrigger.h>
#include <Configuration.h>


void PauseScene::Initialize()
{
    LOG_DEBUG("(PauseScene:Initialize)");
    constexpr float CENTER_X = Configuration::WINDOW_SIZE.x / 2;
    constexpr float CENTER_Y = Configuration::WINDOW_SIZE.y / 2;

    auto world = GetWorld();
    world.import <Modules::RenderModule>();

    // --- Overlay ---
    sf::RectangleShape background(sf::Vector2f{Configuration::WINDOW_SIZE});
    auto backgroundColor = NordTheme::PolarNight1;
    backgroundColor.a = 127;
    background.setFillColor(backgroundColor);

    world.entity()
        .set<RectangleRenderable>({std::move(background)})
        .set<Transform>({.position = {0.f, 0.f}, .scale = {1.f, 1.f}, .rotation = 0.f});

    // --- Add Pause Text ---
    Prefabs::Text::Create(
        world,
        {
            .text = "Game Paused",
            .fontAsset = "Orbitron-Bold",
            .fontSize = 60.f,
            .position = {CENTER_X, CENTER_Y - 200},
            .textColor = NordTheme::SnowStorm3,
            .origin = sf::Vector2f{0.5f, 0.5f},
        }
    );

    // --- Add Resume Button ---
    Prefabs::Button::Create(
        world,
        {
            .text = "Resume",
            .fontAsset = "Orbitron-Bold",
            .fontSize = 48.f,
            .position = {CENTER_X, CENTER_Y},
            .textColor = NordTheme::SnowStorm3,
            .backgroundColor = sf::Color::Transparent,
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
            .position = {CENTER_X, CENTER_Y + 100},
            .textColor = NordTheme::SnowStorm3,
            .backgroundColor = sf::Color::Transparent,
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

        LOG_DEBUG("(PauseScene::HandleEvent): Handle click");

        GetWorld().each(
            [&](const TextRenderable& textRenderable, const EventTrigger& eventTrigger)
            {
                if (textRenderable.text->getGlobalBounds().contains(mousePosition))
                {
                    eventTrigger.callback();
                }
            }
        );
    }
}

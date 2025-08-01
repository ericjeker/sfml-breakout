// Copyright (c) Eric Jeker 2025.

#include "MainMenuScene.h"

#include "Events/ExitGame.h"
#include "Events/StartGame.h"
#include "Managers/EventManager.h"
#include "Managers/GameService.h"
#include "Managers/ResourceManager.h"
#include "Modules/Render/Components/TextRenderable.h"
#include "Modules/Render/Prefabs/Rectangle.h"
#include "Modules/Render/Prefabs/Sprite.h"
#include "Modules/Render/RenderModule.h"
#include "Modules/UI/Prefabs/Button.h"
#include "Modules/UI/Prefabs/Text.h"
#include "Themes/Nord.h"

#include <Logger.h>

#include <Components/EventTrigger.h>
#include <Configuration.h>


void MainMenuScene::Initialize()
{
    Scene::Initialize();

    LOG_DEBUG("(MainMenuScene:Initialize)");
    constexpr float centerX = Configuration::WINDOW_SIZE.x / 2;
    constexpr float centerY = Configuration::WINDOW_SIZE.y / 2;

    auto world = GetWorld();
    world.import <Modules::RenderModule>();

    int zOrder = 0;

    // --- Create Background ---
    Prefabs::Rectangle::Create(
        world,
        {
            .size = sf::Vector2f{Configuration::WINDOW_SIZE},
            .position = {0.f, 0.f},
            .color = NordTheme::PolarNight4,
            .zOrder = zOrder++,
        }
    );

    Prefabs::Sprite::
        Create(world, {.textureAsset = "background", .origin = {0.5f, 0.5f}, .position = {centerX, centerY}, .scale = {2.f, 2.f}, .zOrder = zOrder++});

    // --- Add Title ---
    Prefabs::Text::Create(
        world,
        {
            .text = "Main Menu",
            .fontAsset = "Orbitron-Bold",
            .fontSize = 60.f,
            .position = {centerX, centerY - 200},
            .textColor = NordTheme::SnowStorm3,
            .origin = sf::Vector2f{0.5f, 0.5f},
            .zOrder = zOrder++
        }
    );

    // --- Add Play Button ---
    Prefabs::Button::Create(
        world,
        {
            .text = "Play",
            .fontAsset = "Orbitron-Bold",
            .fontSize = 48.f,
            .position = {centerX, centerY},
            .textColor = NordTheme::SnowStorm3,
            .backgroundColor = sf::Color::Transparent,
            .zOrder = zOrder++,
            .onClick = [this]() { GameService::Get<EventManager>().EmitDeferred<StartGame>({}, this); },
        }
    );

    // --- Add Exit Button ---
    Prefabs::Button::Create(
        world,
        {
            .text = "Exit",
            .fontAsset = "Orbitron-Regular",
            .fontSize = 36.f,
            .position = {centerX, centerY + 100},
            .textColor = NordTheme::SnowStorm3,
            .backgroundColor = sf::Color::Transparent,
            .zOrder = zOrder++,
            .onClick = [this]() { GameService::Get<EventManager>().EmitDeferred<ExitGame>({}, this); },
        }
    );
}

void MainMenuScene::HandleEvent(const std::optional<sf::Event>& event)
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

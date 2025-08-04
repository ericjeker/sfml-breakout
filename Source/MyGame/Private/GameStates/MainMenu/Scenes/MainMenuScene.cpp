// Copyright (c) Eric Jeker 2025.

#include "MainMenuScene.h"

#include "Core/Configuration.h"
#include "Core/Logger.h"
#include "Core/Managers/EventManager.h"
#include "Core/Managers/GameService.h"
#include "Core/Managers/ResourceManager.h"
#include "Core/Modules/Render/Components/Size.h"
#include "Core/Modules/Render/Components/TextRenderable.h"
#include "Core/Modules/Render/Components/Transform.h"
#include "Core/Modules/Render/Prefabs/Rectangle.h"
#include "Core/Modules/Render/RenderModule.h"
#include "Core/Modules/UI/Components/Clickable.h"
#include "Core/Modules/UI/Components/EventTrigger.h"
#include "Core/Modules/UI/Components/Interactable.h"
#include "Core/Modules/UI/Prefabs/Button.h"
#include "Core/Modules/UI/Prefabs/Text.h"
#include "Core/Modules/UI/UIModule.h"
#include "Core/Themes/Nord.h"
#include "Events/ExitGame.h"
#include "Events/StartGame.h"


void MainMenuScene::Initialize()
{
    LOG_DEBUG("(MainMenuScene:Initialize)");
    constexpr float centerX = Configuration::WINDOW_SIZE.x / 2;
    constexpr float centerY = Configuration::WINDOW_SIZE.y / 2;

    auto world = GetWorld();
    // clang-format off
    world.import<Modules::UIModule>();
    world.import<Modules::RenderModule>();
    // clang-format on

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
    );

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
    );

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
            .position = {centerX, centerY + 100},
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

// Copyright (c) Eric Jeker 2025.

#include "MainMenuScene.h"

#include "Events/ExitGame.h"
#include "Events/StartGame.h"
#include "Managers/EventManager.h"
#include "Managers/GameService.h"
#include "Managers/ResourceManager.h"
#include "Modules/Render/Prefabs/Rectangle.h"
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

    // --- Create entities ---
    Prefabs::Rectangle::Create(
        world,
        {
            .position = {0.f, 0.f},
            .size = sf::Vector2f{Configuration::WINDOW_SIZE},
            .color = NordTheme::PolarNight1,
        }
    );

    // --- Add Title ---
    const auto* const fontRegular = GameService::Get<ResourceManager>().GetResource<sf::Font>("Orbitron-Regular");
    const auto* const fontBold = GameService::Get<ResourceManager>().GetResource<sf::Font>("Orbitron-Bold");

    Prefabs::Text::Create(
        world,
        {
            .text = "Main Menu",
            .position = {centerX, centerY - 200},
            .font = fontBold,
            .size = 60.f,
            .color = NordTheme::SnowStorm3,
            .origin = sf::Vector2f{0.5f, 0.5f},
        }
    );

    // --- Add Play Button ---
    Prefabs::Button::Create(
        world,
        {
            .text = "Play",
            .font = fontRegular,
            .fontSize = 36.f,
            .position = {centerX, centerY},
            .onClick = [this]() { GameService::Get<EventManager>().EmitDeferred<StartGame>({}, this); },
            .textColor = NordTheme::SnowStorm3,
        }
    );

    // --- Add Exit Button ---
    Prefabs::Button::Create(
        world,
        {
            .text = "Exit",
            .font = fontRegular,
            .fontSize = 36.f,
            .position = {centerX, centerY + 100},
            .onClick = [this]() { GameService::Get<EventManager>().EmitDeferred<ExitGame>({}, this); },
            .textColor = NordTheme::SnowStorm3,
        }
    );

    GetWorld().system<Transform, TextRenderable>().each(ApplyTransform);
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
                if (textRenderable.text.getGlobalBounds().contains(mousePosition))
                {
                    eventTrigger.callback();
                }
            }
        );
    }
}

void MainMenuScene::ApplyTransform(const Transform& t, TextRenderable& textRenderable)
{
    textRenderable.text.setPosition(t.position);
}

void MainMenuScene::ProcessBackground(const Transform& t, RectangleRenderable& bg)
{
    bg.shape.setPosition(t.position);
}

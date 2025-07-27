// Copyright (c) Eric Jeker 2025.

#include "PauseScene.h"

#include "../Events/RequestGameResume.h"
#include "../Events/RequestReturnToMainMenu.h"
#include "Managers/EventManager.h"
#include "Managers/GameService.h"
#include "Managers/ResourceManager.h"

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>

#include <Logger.h>
#include <Themes/Nord.h>

#include <Components/EventTrigger.h>
#include <Components/TextRenderable.h>
#include <Components/Transform.h>
#include <Configuration.h>


void PauseScene::Initialize()
{
    LOG_DEBUG("(PauseScene:Initialize)");
    constexpr float centerX = Configuration::WINDOW_SIZE.x / 2;
    constexpr float centerY = Configuration::WINDOW_SIZE.y / 2;
    const auto ecsWorld = GetWorld();

    // --- Overlay ---
    auto background = std::make_unique<sf::RectangleShape>(sf::Vector2f{Configuration::WINDOW_SIZE});
    auto backgroundColor = NordTheme::PolarNight1;
    backgroundColor.a = 127;
    background->setFillColor(backgroundColor);

    ecsWorld.entity()
        .set<BackgroundRenderable>({std::move(background)})
        .set<Transform>({.position = {0.f, 0.f}, .scale = {1.f, 1.f}, .rotation = 0.f});

    // --- Add Pause Text ---
    const auto fontRegular = GameService::Get<ResourceManager>().GetResource<sf::Font>("Orbitron-Regular");
    const auto fontBold = GameService::Get<ResourceManager>().GetResource<sf::Font>("Orbitron-Bold");

    auto pauseText = std::make_unique<sf::Text>(*fontBold, "Pause Scene", 60.f);
    pauseText->setFillColor(NordTheme::SnowStorm3);
    pauseText->setOrigin(pauseText->getLocalBounds().size / 2.f);
    CreateTextEntity(std::move(pauseText), {centerX, centerY - 200});

    // --- Add Resume Button ---
    auto resumeText = std::make_unique<sf::Text>(*fontRegular, "Resume", 36.f);
    resumeText->setFillColor(NordTheme::SnowStorm3);
    resumeText->setOrigin(resumeText->getLocalBounds().size / 2.f);
    CreateButtonEntity(
        std::move(resumeText),
        {centerX, centerY},
        [this]() { GameService::Get<EventManager>().EmitDeferred<RequestGameResume>({}, this); }
    );

    // --- Add Exit Button ---
    auto exitText = std::make_unique<sf::Text>(*fontRegular, "Exit", 28.f);
    exitText->setFillColor(NordTheme::SnowStorm3);
    exitText->setOrigin(exitText->getLocalBounds().size / 2.f);
    CreateButtonEntity(
        std::move(exitText),
        {centerX, centerY + 100},
        [this]() { GameService::Get<EventManager>().EmitDeferred<RequestReturnToMainMenu>({}, this); }
    );

    // --- Add the Systems ---
    GetWorld().system<Transform, BackgroundRenderable>().each(ProcessBackground);
    GetWorld().system<Transform, TextRenderable>().each(ProcessText);
}


void PauseScene::HandleEvent(const std::optional<sf::Event>& event, sf::RenderWindow& window)
{
    if (!IsLoaded() || IsPaused())
    {
        return;
    }

    if (auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>())
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

void PauseScene::Render(sf::RenderWindow& window)
{
    GetWorld().each([&](const BackgroundRenderable& bg) { window.draw(*bg.shape); });
    GetWorld().each([&](const TextRenderable& textRenderable) { window.draw(*textRenderable.text); });
}

void PauseScene::CreateTextEntity(std::unique_ptr<sf::Text> text, const sf::Vector2f position)
{
    GetWorld().entity().set<Transform>({.position = position}).set<TextRenderable>({.text = std::move(text)});
}

void PauseScene::CreateButtonEntity(std::unique_ptr<sf::Text> text, const sf::Vector2f position, const std::function<void()>& callback)
{
    GetWorld()
        .entity()
        .set<Transform>({.position = position})
        .set<TextRenderable>({.text = std::move(text)})
        .set<EventTrigger>({.callback = callback});
}

void PauseScene::ProcessText(const Transform& t, const TextRenderable& textRenderable)
{
    textRenderable.text->setPosition(t.position);
}

void PauseScene::ProcessBackground(const Transform& t, const BackgroundRenderable& bg)
{
    bg.shape->setPosition(t.position);
}

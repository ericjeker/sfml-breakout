// Copyright (c) Eric Jeker 2025.

#include "MainMenuScene.h"

#include "../Events/ExitGame.h"
#include "../Events/StartGame.h"
#include "../Modules/RenderModule/RenderModule.h"
#include "Managers/EventManager.h"
#include "Managers/GameService.h"
#include "Managers/ResourceManager.h"
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

    auto ecsWorld = GetWorld();
    ecsWorld.import <RenderModule::RenderModule>();

    // --- Create entities ---
    auto backgroundDrawable = std::make_unique<sf::RectangleShape>();
    backgroundDrawable->setSize(sf::Vector2f{Configuration::WINDOW_SIZE});
    backgroundDrawable->setFillColor(NordTheme::Frost1);

    ecsWorld.entity().set<RectangleShape>({std::move(backgroundDrawable)}).set<Transform>({});

    // --- Add Title ---
    const auto fontRegular = GameService::Get<ResourceManager>().GetResource<sf::Font>("Orbitron-Regular");
    const auto fontBold = GameService::Get<ResourceManager>().GetResource<sf::Font>("Orbitron-Bold");

    auto mainMenuText = std::make_unique<sf::Text>(*fontBold, "Main Menu", 60.f);
    mainMenuText->setFillColor(NordTheme::SnowStorm3);
    mainMenuText->setOrigin(mainMenuText->getLocalBounds().size / 2.f);
    CreateTextEntity(std::move(mainMenuText), {centerX, centerY - 200});

    // --- Add Play Button ---
    auto playButton = std::make_unique<sf::Text>(*fontRegular, "Play", 36.f);
    playButton->setFillColor(NordTheme::SnowStorm3);
    playButton->setOrigin(playButton->getLocalBounds().size / 2.f);
    CreateButtonEntity(
        std::move(playButton),
        {centerX, centerY},
        [this]() { GameService::Get<EventManager>().EmitDeferred<StartGame>({}, this); }
    );

    // --- Add Exit Button ---
    auto exitButton = std::make_unique<sf::Text>(*fontRegular, "Exit", 28.f);
    exitButton->setFillColor(NordTheme::SnowStorm3);
    exitButton->setOrigin(exitButton->getLocalBounds().size / 2.f);
    CreateButtonEntity(
        std::move(exitButton),
        {centerX, centerY + 100},
        [this]() { GameService::Get<EventManager>().EmitDeferred<ExitGame>({}, this); }
    );

    GetWorld().system<Transform, TextRenderable>().each(ProcessText);
}

void MainMenuScene::HandleEvent(const std::optional<sf::Event>& event, sf::RenderWindow& window)
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

        LOG_DEBUG("(MainMenuScene::HandleEvent): Handle click");

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

void MainMenuScene::CreateTextEntity(std::unique_ptr<sf::Text> text, sf::Vector2f position)
{
    GetWorld().entity().set<Transform>({.position = position}).set<TextRenderable>({.text = std::move(text)});
}

void MainMenuScene::CreateButtonEntity(
    std::unique_ptr<sf::Text> text,
    const sf::Vector2f position,
    const std::function<void()>& callback
)
{
    GetWorld()
        .entity()
        .set<Transform>({.position = position})
        .set<TextRenderable>({.text = std::move(text)})
        .set<EventTrigger>({.callback = callback});
}

void MainMenuScene::ProcessText(const Transform& t, const TextRenderable& textRenderable)
{
    textRenderable.text->setPosition(t.position);
}

void MainMenuScene::ProcessBackground(const Transform& t, const RectangleShape& bg)
{
    bg.shape->setPosition(t.position);
}


// Copyright (c) Eric Jeker 2025.

#include "PauseScene.h"

#include "../Components/BoundingBoxComponent.h"
#include "../Components/EventComponent.h"
#include "../Events/ExitGameRequestedEvent.h"
#include "../Events/ResumeGameRequestedEvent.h"
#include "../Systems/EventSystem.h"
#include "Managers/GameService.h"

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>

#include <Logger.h>
#include <Systems/DrawableRenderer.h>
#include <Themes/Nord.h>

#include <Components/DrawableComponent.h>
#include <Components/TransformComponent.h>
#include <Configuration.h>



void PauseScene::Initialize()
{
    LOG_DEBUG("(PauseScene:Initialize)");

    // --- Overlay ---
    auto background = std::make_unique<sf::RectangleShape>(sf::Vector2f{Configuration::WINDOW_SIZE});
    auto backgroundColor = NordTheme::PolarNight1;
    backgroundColor.a = 127;
    background->setFillColor(backgroundColor);

    auto backgroundEntity = std::make_unique<Entity>(GenerateId());
    backgroundEntity->AddComponent<DrawableComponent>({.drawable = std::move(background)});
    backgroundEntity->AddComponent<TransformComponent>({});
    AddEntity(std::move(backgroundEntity));

    // --- Add Pause Text ---
    const float centerX = Configuration::WINDOW_SIZE.x / 2;
    const float centerY = Configuration::WINDOW_SIZE.y / 2;

    const auto fontRegular = GameService::Get<ResourceManager>().GetResource<sf::Font>("Orbitron-Regular");
    const auto fontBold = GameService::Get<ResourceManager>().GetResource<sf::Font>("Orbitron-Bold");

    auto pauseText = std::make_unique<sf::Text>(*fontBold, "Pause Scene", 60.f);
    pauseText->setFillColor(NordTheme::SnowStorm3);
    pauseText->setOrigin(pauseText->getLocalBounds().size / 2.f);
    AddEntity(std::move(CreateTextEntity(std::move(pauseText), {centerX, centerY - 200})));

    auto resumeText = std::make_unique<sf::Text>(*fontRegular, "Resume", 36.f);
    resumeText->setFillColor(NordTheme::SnowStorm3);
    resumeText->setOrigin(resumeText->getLocalBounds().size / 2.f);
    AddEntity(
        std::move(CreateButtonEntity(
            std::move(resumeText),
            {centerX, centerY},
            [this]() { GameService::Get<EventManager>().EmitDeferred<ResumeGameRequestedEvent>({}, this); }
        ))
    );

    auto exitText = std::make_unique<sf::Text>(*fontRegular, "Exit", 28.f);
    exitText->setFillColor(NordTheme::SnowStorm3);
    exitText->setOrigin(exitText->getLocalBounds().size / 2.f);
    AddEntity(
        std::move(CreateButtonEntity(
            std::move(exitText),
            {centerX, centerY + 100},
            [this]() { GameService::Get<EventManager>().EmitDeferred<ExitGameRequestedEvent>({}, this); }
        ))
    );

    // --- Add the Systems ---
    AddSystem(std::make_unique<EventSystem>());
    AddSystem(std::make_unique<DrawableRenderer>());
}

std::unique_ptr<Entity> PauseScene::CreateTextEntity(std::unique_ptr<sf::Text> text, const sf::Vector2f position)
{
    auto entity = std::make_unique<Entity>(GenerateId());
    entity->AddComponent<DrawableComponent>({.drawable = std::move(text)});
    entity->AddComponent<TransformComponent>({.position = position});

    return entity;
}

std::unique_ptr<Entity> PauseScene::CreateButtonEntity(
    std::unique_ptr<sf::Text> text,
    const sf::Vector2f position,
    const std::function<void()>& callback
)
{
    auto entity = std::make_unique<Entity>(GenerateId());
    entity->AddComponent<TransformComponent>({.position = position});
    entity->AddComponent<EventComponent>({.callback = callback});
    entity->AddComponent<BoundingBoxComponent>({.bounds = text->getGlobalBounds()});
    entity->AddComponent<DrawableComponent>({.drawable = std::move(text)});

    return entity;
}

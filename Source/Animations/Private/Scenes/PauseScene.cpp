// Copyright (c) Eric Jeker 2025.

#include "PauseScene.h"

#include "ApplicationConfiguration.h"

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>

#include <Logger.h>
#include <Systems/DrawableRenderer.h>
#include <Themes/Nord.h>

#include <Components/DrawableComponent.h>
#include <Components/TransformComponent.h>

PauseScene::PauseScene(ResourceManager& resourceManager, EventManager& eventManager)
    : Scene(resourceManager, eventManager)
{
}

void PauseScene::Initialize()
{
    LOG_DEBUG("(PauseScene:Initialize)");

    // -- Overlay --
    auto background = std::make_unique<sf::RectangleShape>(sf::Vector2f{ApplicationConfiguration::windowSize});
    auto backgroundColor = NordTheme::PolarNight1;
    backgroundColor.a = 127;
    background->setFillColor(backgroundColor);

    auto backgroundEntity = std::make_unique<Entity>(GenerateId());
    backgroundEntity->AddComponent<DrawableComponent>({.drawable = std::move(background)});
    backgroundEntity->AddComponent<TransformComponent>({});
    AddEntity(std::move(backgroundEntity));

    // -- Add Pause Text --
    const auto font = GetResourceManager().GetResource<sf::Font>("Orbitron-Regular");
    auto text = std::make_unique<sf::Text>(*font, "Pause Scene");
    text->setFillColor(NordTheme::SnowStorm3);
    text->setOrigin(text->getLocalBounds().size / 2.f);

    const float centerX = ApplicationConfiguration::windowSize.x / 2;
    const float centerY = ApplicationConfiguration::windowSize.y / 2;

    auto entity = std::make_unique<Entity>(GenerateId());
    entity->AddComponent<DrawableComponent>({.drawable = std::move(text)});
    entity->AddComponent<TransformComponent>({.position = {centerX, centerY}});

    AddEntity(std::move(entity));

    // -- Add the Systems --
    AddSystem(std::make_unique<DrawableRenderer>());
}

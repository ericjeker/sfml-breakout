// Copyright (c) Eric Jeker 2025.

#include "DemoScene.h"

#include "ApplicationConfiguration.h"
#include "Components/AnimationComponent.h"
#include "Components/DrawableComponent.h"
#include "Components/TransformComponent.h"
#include "Logger.h"
#include "Systems/AnimationSystem.h"
#include "Systems/DrawableRenderer.h"
#include "Themes/Nord.h"

#include <SFML/Graphics/RectangleShape.hpp>

DemoScene::DemoScene(ResourceManager& resourceManager, EventManager& eventManager)
    : Scene(resourceManager, eventManager)
{
}

void DemoScene::Initialize()
{
    LOG_DEBUG("(DemoScene:Initialize)");
    Scene::Initialize();

    auto background = std::make_unique<sf::RectangleShape>(sf::Vector2f{ApplicationConfiguration::windowSize});
    background->setFillColor(NordTheme::Frost1);
    background->setPosition({0.f, 0.f});

    auto backgroundEntity = std::make_unique<Entity>(GenerateId());
    backgroundEntity->AddComponent<DrawableComponent>({.drawable = std::move(background)});
    backgroundEntity->AddComponent<TransformComponent>({});
    AddEntity(std::move(backgroundEntity));

    // --- Create the Entities ---
    auto rectangleShape = std::make_unique<sf::RectangleShape>(sf::Vector2f{200.f, 200.f});
    rectangleShape->setFillColor(NordTheme::Aurora1);
    rectangleShape->setOrigin({100.f, 100.f});

    DrawableComponent drawable;
    drawable.drawable = std::move(rectangleShape);

    TransformComponent transform;
    transform.position = {500.f, 500.f};
    transform.scale = {1.f, 1.f};
    transform.rotation = sf::degrees(45.f);

    AnimationComponent animation;
    animation.type = AnimationComponent::ROTATE;
    animation.startFloat = 0.f;
    animation.endFloat = 360.f;
    animation.duration = 1.f;
    animation.loop = true;

    auto rectangleEntity = std::make_unique<Entity>(GenerateId());
    rectangleEntity->AddComponent<DrawableComponent>(std::move(drawable));
    rectangleEntity->AddComponent<TransformComponent>(transform);
    rectangleEntity->AddComponent<AnimationComponent>(animation);

    AddEntity(std::move(rectangleEntity));

    // --- Create the Systems ---
    AddSystem(std::make_unique<DrawableRenderer>());
    AddSystem(std::make_unique<AnimationSystem>());
}

void DemoScene::HandleEvent(const std::optional<sf::Event>& event, sf::RenderWindow& window)
{
    Scene::HandleEvent(event, window);

    if (!IsLoaded())
    {
        return;
    }

    if (event->is<sf::Event::MouseButtonPressed>())
    {
        LOG_DEBUG("(DemoScene:HandleEvent): Mouse button pressed");
    }
}

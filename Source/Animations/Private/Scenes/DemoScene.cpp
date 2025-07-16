// Copyright (c) Eric Jeker 2025.

#include "DemoScene.h"

#include "../Components/AnimationComponent.h"
#include "../Components/DrawableComponent.h"
#include "../Components/TransformComponent.h"
#include "../Systems/AnimationSystem.h"
#include "../Systems/DrawableRenderer.h"
#include "Logger.h"

#include <SFML/Graphics/RectangleShape.hpp>

DemoScene::DemoScene(ResourceManager& resourceManager, EventManager& eventManager)
    : Scene(resourceManager, eventManager)
{
}

void DemoScene::Initialize()
{
    LOG_DEBUG("(DemoScene:Initialize)");
    Scene::Initialize();

    // --- Create the Entities ---
    auto rectangleShape = std::make_unique<sf::RectangleShape>(sf::Vector2f{200.f, 200.f});
    rectangleShape->setFillColor(sf::Color::White);
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

    auto entity = std::make_unique<Entity>(GenerateId());
    entity->AddComponent<DrawableComponent>(std::move(drawable));
    entity->AddComponent<TransformComponent>(std::move(transform));
    entity->AddComponent<AnimationComponent>(std::move(animation));

    AddEntity(std::move(entity));

    // --- Create the Systems ---
    auto renderer = std::make_unique<DrawableRenderer>();
    AddSystem(std::move(renderer));
    auto animationSystem = std::make_unique<AnimationSystem>();
    AddSystem(std::move(animationSystem));
}

void DemoScene::HandleEvent(const std::optional<sf::Event>& event, sf::RenderWindow& window)
{
    Scene::HandleEvent(event, window);

    if (event->is<sf::Event::MouseButtonPressed>())
    {
        LOG_DEBUG("(DemoScene:HandleEvent): Mouse button pressed");
    }
}

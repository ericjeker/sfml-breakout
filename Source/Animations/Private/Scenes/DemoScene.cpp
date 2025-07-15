// Copyright (c) Eric Jeker 2025.

#include "DemoScene.h"

#include "../Components/Animation.h"
#include "../Components/Drawable.h"
#include "../Components/Transform.h"
#include "../Systems/AnimationSystem.h"
#include "../Systems/DrawableRenderer.h"
#include "Logger.h"

#include <SFML/Graphics/RectangleShape.hpp>

void DemoScene::Initialize()
{
    LOG_DEBUG("(DemoScene:Initialize)");

    // --- Create the Entities ---
    auto rectangleShape = std::make_unique<sf::RectangleShape>(sf::Vector2f{200.f, 200.f});
    rectangleShape->setFillColor(sf::Color::White);
    rectangleShape->setOrigin({100.f, 100.f});

    Drawable drawable;
    drawable.drawable = std::move(rectangleShape);

    Transform transform;
    transform.position = {500.f, 500.f};
    transform.scale = {1.f, 1.f};
    transform.rotation = sf::degrees(45.f);

    Animation animation;
    animation.type = Animation::ROTATE;
    animation.startFloat = 0.f;
    animation.endFloat = 360.f;
    animation.duration = 1.f;
    animation.loop = true;

    auto circleEntity = std::make_unique<Entity>(GenerateId());
    circleEntity->AddComponent<Drawable>(std::move(drawable));
    circleEntity->AddComponent<Transform>(std::move(transform));
    circleEntity->AddComponent<Animation>(std::move(animation));

    AddEntity(std::move(circleEntity));

    // --- Create the Systems ---
    auto renderer = std::make_unique<DrawableRenderer>();
    AddSystem(std::move(renderer));
    auto animationSystem = std::make_unique<AnimationSystem>();
    AddSystem(std::move(animationSystem));
}

// Copyright (c) Eric Jeker 2025.

#include "../Components/Drawable.h"
#include "../Systems/DrawableRenderer.h"
#include "Logger.h"
#include "RedDemoScene.h"

#include <SFML/Graphics/CircleShape.hpp>

void RedDemoScene::Initialize()
{
    LOG_DEBUG("(RedDemoScene:Initialize)");

    // --- Create the Entities ---
    auto circle = std::make_unique<sf::CircleShape>(50.f);
    circle->setFillColor(sf::Color::Red);
    circle->setPosition({200.f, 200.f});

    Drawable drawable;
    drawable.drawable = std::move(circle);

    auto circleEntity = std::make_unique<Entity>(GenerateId());
    circleEntity->AddComponent<Drawable>(std::move(drawable));

    AddEntity(std::move(circleEntity));

    // --- Create the Systems ---
    auto renderer = std::make_unique<DrawableRenderer>();
    AddSystem(std::move(renderer));
}

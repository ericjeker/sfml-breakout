// Copyright (c) Eric Jeker 2025.

#include "BlueDemoScene.h"

#include "../Components/Drawable.h"
#include "../Systems/DrawableRenderer.h"
#include "Logger.h"

#include <SFML/Graphics/CircleShape.hpp>

void BlueDemoScene::Initialize()
{
    LOG_DEBUG("(BlueDemoScene:Initialize)");

    // --- Create the Entities ---
    auto circle = std::make_unique<sf::CircleShape>(50.f);
    circle->setFillColor(sf::Color::Blue);
    circle->setPosition({200.f, 500.f});

    Drawable drawable;
    drawable.drawable = std::move(circle);

    auto circleEntity = std::make_unique<Entity>(GenerateId());
    circleEntity->AddComponent<Drawable>(std::move(drawable));

    AddEntity(std::move(circleEntity));

    // --- Create the Systems ---
    auto renderer = std::make_unique<DrawableRenderer>();
    AddSystem(std::move(renderer));
}

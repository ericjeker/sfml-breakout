// Copyright (c) Eric Jeker 2025.

#include "DemoScene.h"

#include <SFML/Graphics/CircleShape.hpp>

DemoScene::DemoScene()
{
    _circle = std::make_unique<sf::CircleShape>(50.f);
    _circle->setFillColor(sf::Color::Red);
    _circle->setPosition({200.f, 200.f});
}

void DemoScene::Update(float deltaTime)
{
}

void DemoScene::Render(sf::RenderWindow& window)
{
    window.draw(*_circle);
}

void DemoScene::HandleEvent(const std::optional<sf::Event>& event)
{
}

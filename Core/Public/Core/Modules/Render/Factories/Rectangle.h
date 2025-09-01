// Copyright (c) Eric Jeker 2025.

#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>

#include <flecs.h>


namespace Factories
{

struct RectangleParams
{
    sf::Vector2f size;
    sf::Color color = sf::Color::Magenta;
    sf::Vector2f origin = {0.f, 0.f};
    sf::Vector2f position = {0.f, 0.f};
    sf::Vector2f scale = {1.f, 1.f};
    float rotation = 0.f;
    float zOrder = 0;
};

struct Rectangle
{
    static flecs::entity Create(const flecs::world& world, const RectangleParams& params);
};

} // namespace Factories

// Copyright (c) Eric Jeker 2025.

#pragma once

#include <SFML/Graphics.hpp>

#include <flecs.h>


namespace Factories
{

struct CircleParams
{
    float radius = 4.f;
    sf::Color color = sf::Color::Magenta;
    sf::Vector2f origin = {0.f, 0.f};
    sf::Vector2f position = {0.f, 0.f};
    sf::Vector2f scale = {1.f, 1.f};
    float zOrder = 0;
};

struct Circle
{
    static flecs::entity Create(const flecs::world& world, const CircleParams& params);
};

} // namespace Factories

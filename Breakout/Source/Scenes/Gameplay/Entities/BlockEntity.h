// Copyright (c) Eric Jeker 2025.

#pragma once

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>

#include <flecs.h>


struct BlockEntity
{
    struct BlockParams
    {
        sf::Vector2f position = {0.f, 0.f};
        sf::Color color = sf::Color::White;
        sf::Vector2f size = {100.f, 20.f};
        float zOrder = 0.f;
        float rotation = 0.f;
        int health = 1;
        bool isIndestructible = false;
    };

    static flecs::entity Create(const flecs::world& world, const flecs::entity& rootEntity, const BlockParams& params);
};

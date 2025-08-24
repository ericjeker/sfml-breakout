// Copyright (c) Eric Jeker 2025.

#pragma once

#include <SFML/System/Vector2.hpp>

struct CollisionInfo
{
    bool hasCollision = false;
    sf::Vector2f contactPoint;
    sf::Vector2f normal;
    float penetrationDepth = 0.f;
};

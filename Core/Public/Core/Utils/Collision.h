// Copyright (c) Eric Jeker 2025.

#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>

namespace Collision
{

struct ContactInformation
{
    bool hasCollision = false;
    sf::Vector2f contactPoint;
    // Points away from the AABB toward the circle
    sf::Vector2f normal;
    float penetrationDepth = 0.f;
};

ContactInformation CheckAABBCircleCollision(const sf::FloatRect& aabb, const sf::Vector2f& circleCenter, float circleRadius);

} // namespace Collision

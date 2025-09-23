// Copyright (c) Eric Jeker 2025.

#pragma once

#include "Core/Modules/Physics/Components/CollisionInfo.h"

#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>

namespace Core
{

CollisionInfo CheckAABBCircleCollision(const sf::FloatRect& aabb, const sf::Vector2f& circleCenter, float circleRadius);

} // namespace Core

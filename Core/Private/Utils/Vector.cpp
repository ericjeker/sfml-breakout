// Copyright (c) Eric Jeker 2025.

#include "Core/Utils/Vector.h"

namespace Vector
{

float Dot(const sf::Vector2f& a, const sf::Vector2f& b)
{
    return a.x * b.x + a.y * b.y;
}

} // namespace Vector

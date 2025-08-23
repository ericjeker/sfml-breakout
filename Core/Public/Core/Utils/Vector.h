// Copyright (c) Eric Jeker 2025.

#pragma once

#include <SFML/System/Vector2.hpp>

namespace Vector
{

// For some reason, SFML doesn't provide a dot product...
float Dot(const sf::Vector2f& a, const sf::Vector2f& b);

}

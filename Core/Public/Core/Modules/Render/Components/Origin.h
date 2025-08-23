// Copyright (c) Eric Jeker 2025.

#pragma once

#include <SFML/System/Vector2.hpp>


/**
 * Origin is relative to size; it is expressed by value from 0.f to 1.f, where 0.5, 0.5f is the center.
 */
struct Origin
{
    sf::Vector2f origin = {0.f, 0.f};
};

// Copyright (c) Eric Jeker 2025.

#pragma once

#include <SFML/System/Vector2.hpp>

struct GravitySettings
{
    sf::Vector2f gravity;
    float pixelsPerCentimeter{};
    bool enabled = true;
};

// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef PHYSICSCONSTANTS_H
#define PHYSICSCONSTANTS_H

#include "Configuration.h"

#include <SFML/System/Vector2.hpp>

namespace PhysicsConstants
{

// Gravity values
constexpr float EARTH_GRAVITY = 981.f; // cm/s²
constexpr sf::Vector2f GRAVITY_DOWN = {0.f, EARTH_GRAVITY};
constexpr sf::Vector2f GRAVITY_UP = {0.f, -EARTH_GRAVITY};
constexpr sf::Vector2f NO_GRAVITY = {0.f, 0.f};

// Other physics constants
constexpr float DEFAULT_DAMPING = 0.98f;
constexpr float NO_DAMPING = 1.f;
constexpr float DEFAULT_BALL_RADIUS = 8.f;

// Consider that the height of the screen is 2 meters (200 cm) high
constexpr float PIXELS_PER_CENTIMETER = Configuration::WINDOW_SIZE.y / 200.f;

}


#endif

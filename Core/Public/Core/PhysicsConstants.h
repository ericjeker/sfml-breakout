// Copyright (c) Eric Jeker 2025.

#pragma once

#include "Configuration.h"

#include <SFML/System/Vector2.hpp>

// TODO: Move that to the Physics module
namespace PhysicsConstants
{
// Gravity values (our world use centimeters and seconds)
constexpr float EARTH_GRAVITY = 981.f; // cm/s²
constexpr sf::Vector2f GRAVITY_DOWN = {0.f, EARTH_GRAVITY};
constexpr sf::Vector2f GRAVITY_UP = {0.f, -EARTH_GRAVITY};
constexpr sf::Vector2f NO_GRAVITY = {0.f, 0.f};

// Other physics constants
constexpr float DEFAULT_DAMPING = 0.98f;
constexpr float NO_DAMPING = 1.f;

// Consider that the height of the screen is 2 meters (200 cm) high
constexpr float PIXELS_PER_CENTIMETER = Configuration::RESOLUTION.y / 200.f;

constexpr float NO_FRICTION = 0.f;
} // namespace PhysicsConstants



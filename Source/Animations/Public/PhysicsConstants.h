// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef PHYSICSCONSTANTS_H
#define PHYSICSCONSTANTS_H

#include <SFML/System/Vector2.hpp>

namespace PhysicsConstants
{

// Gravity values
constexpr float EARTH_GRAVITY = 981.f; // cm/s²
constexpr sf::Vector2f GRAVITY_DOWN = {0.f, EARTH_GRAVITY};
constexpr sf::Vector2f GRAVITY_UP = {0.f, -EARTH_GRAVITY};
constexpr sf::Vector2f NO_GRAVITY = {0.f, 0.f};

// Other physics constants
constexpr float DEFAULT_FRICTION = 0.98f;
constexpr float DEFAULT_BALL_RADIUS = 5.f;

constexpr float PIXELS_PER_CENTIMETER = ApplicationConfiguration::WINDOW_SIZE.x / 200.f;

}


#endif

// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Scenes/Component.h"

#include <SFML/System/Vector2.hpp>


struct TransformComponent final : Component
{
    sf::Vector2f position{0.f, 0.f};
    sf::Angle rotation = sf::degrees(0);
    sf::Vector2f scale{1.f, 1.f};
};


#endif

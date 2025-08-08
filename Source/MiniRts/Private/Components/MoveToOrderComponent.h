// Copyright (c) Eric Jeker 2025.

#pragma once



#include "Scenes/Component.h"

#include <SFML/System/Vector2.hpp>

struct MoveToOrderComponent : Component
{
    sf::Vector2f target;
};




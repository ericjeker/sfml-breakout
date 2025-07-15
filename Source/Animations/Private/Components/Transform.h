// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Scenes/Component.h"

#include <SFML/System/Vector2.hpp>


struct Transform final : public Component
{
    sf::Vector2f position;
    float rotation;
    sf::Vector2f scale;
};


#endif

// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef BALLRENDERABLE_H
#define BALLRENDERABLE_H

#include <SFML/Graphics/CircleShape.hpp>

#include <memory>

struct CircleShape
{
    std::unique_ptr<sf::CircleShape> shape;
};


#endif

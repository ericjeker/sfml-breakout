// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef BALLRENDERABLE_H
#define BALLRENDERABLE_H

#include <memory>
#include <SFML/Graphics/CircleShape.hpp>


struct BallRenderable
{
    std::unique_ptr<sf::CircleShape> shape;
};


#endif

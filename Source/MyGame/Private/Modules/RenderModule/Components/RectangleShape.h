// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef BACKGROUNDRENDERABLE_H
#define BACKGROUNDRENDERABLE_H

#include <SFML/Graphics/RectangleShape.hpp>

#include <memory>


struct RectangleShape
{
    std::unique_ptr<sf::RectangleShape> shape;
};


#endif

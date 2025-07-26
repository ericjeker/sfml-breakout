// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef BACKGROUNDRENDERABLE_H
#define BACKGROUNDRENDERABLE_H

#include <memory>
#include <SFML/Graphics/RectangleShape.hpp>


struct BackgroundRenderable
{
    std::unique_ptr<sf::RectangleShape> shape;
};


#endif

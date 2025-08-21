// Copyright (c) Eric Jeker 2025.

#pragma once



#include <SFML/System/Vector2.hpp>


struct Transform
{
    sf::Vector2f position = {0.f, 0.f};
    sf::Vector2f scale = {1.f, 1.f};
    float rotation = 0.f;
};




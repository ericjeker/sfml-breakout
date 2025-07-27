// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef BUTTONBACKGROUND_H
#define BUTTONBACKGROUND_H

#include <SFML/Graphics.hpp>


struct ButtonBackground
{
    sf::Color backgroundColor = sf::Color(50, 50, 50, 200);
    sf::Color hoverColor = sf::Color(70, 70, 70, 200);
    sf::Vector2f padding{10.0f, 10.0f};
};


#endif

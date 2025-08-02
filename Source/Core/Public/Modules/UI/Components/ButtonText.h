// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef BUTTONTEXT_H
#define BUTTONTEXT_H

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>


struct ButtonText
{
    std::string text;
    float fontSize = 28.0f;
    std::string fontAsset = "Orbitron-Regular";
    sf::Vector2f position{0.0f, 0.0f};
    sf::Color textColor = sf::Color::White;
    sf::Color hoverColor = sf::Color(70, 70, 70, 200);
};


#endif

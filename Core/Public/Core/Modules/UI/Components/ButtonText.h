// Copyright (c) Eric Jeker 2025.

#pragma once

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>

#include <string>


struct ButtonText
{
    std::string text;
    float fontSize = 28.0f;
    std::string fontAsset = "Orbitron-Regular";
    sf::Vector2f position{0.0f, 0.0f};
    sf::Color textColor = sf::Color::White;
    sf::Color hoverColor = sf::Color(70, 70, 70, 200);
};




// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef BUTTON_H
#define BUTTON_H

#include <SFML/Graphics.hpp>

#include <flecs.h>


namespace Prefabs
{

struct ButtonParams
{
    std::string text = "Button Text";
    const sf::Font* font;
    float fontSize = 28.0f;
    sf::Vector2f position = {0.0f, 0.0f};
    sf::Vector2f padding = {10.0f, 10.0f};
    sf::Color textColor = sf::Color::White;
    sf::Color textHoverColor = sf::Color::White;
    sf::Color backgroundColor = sf::Color::Black;
    sf::Color hoverColor = sf::Color::Black;
    // TODO: Add outline parameter
    std::function<void()> onClick = nullptr;
};

struct Button
{
    static flecs::entity Create(const flecs::world& world, const ButtonParams& params);
};

} // namespace Prefabs


#endif

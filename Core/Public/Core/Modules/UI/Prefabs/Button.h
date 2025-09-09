// Copyright (c) Eric Jeker 2025.

#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>

#include <flecs.h>
#include <functional>
#include <string>


// TODO: Move this to Builders/Factories
namespace Prefabs
{

struct ButtonParams
{
    std::string text = "Button Text";
    std::string fontAsset = "Orbitron-Regular";
    float fontSize = 28.0f;
    sf::Color textColor = sf::Color::White;
    sf::Color textHoverColor = sf::Color::White;
    sf::Color backgroundColor = sf::Color::Transparent;
    sf::Color hoverColor = sf::Color::Transparent;
    sf::Vector2f origin = {0.5f, 0.5f};
    sf::Vector2f position = {0.0f, 0.0f};
    sf::Vector2f padding = {20.0f, 10.0f};
    float zOrder = 0;
    // TODO: Add outline parameter
    std::function<void(const flecs::world&)> onClick = nullptr;
};

struct Button
{
    static flecs::entity Create(const flecs::world& world, const ButtonParams& params);
};

} // namespace Prefabs

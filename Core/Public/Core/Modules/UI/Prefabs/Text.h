// Copyright (c) Eric Jeker 2025.

#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>

#include <flecs.h>
#include <string>


// TODO: Move this to Builders/Factories
namespace Prefabs
{

struct TextParams
{
    std::string text = "Default Text";
    std::string fontAsset = "Orbitron-Regular";
    float fontSize = 12.f;
    sf::Color textColor = sf::Color::Magenta;
    sf::Vector2f origin = {0.f, 0.f};
    sf::Vector2f position = {0.f, 0.f};
    sf::Vector2f scale = {1.f, 1.f};
    float rotation = 0.f;
    float zOrder = 0;
};

struct Text
{
    static flecs::entity Create(const flecs::world& world, const TextParams& params);
};

} // namespace Prefabs

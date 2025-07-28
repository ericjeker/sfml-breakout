// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef TEXT_H
#define TEXT_H

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>

#include <flecs.h>
#include <string>

namespace Prefabs
{

struct TextParams
{
    std::string text;
    const sf::Font* font;
    float size = 10.f;
    sf::Color color = sf::Color::Magenta;
    sf::Vector2f origin = {0.f, 0.f};
    sf::Vector2f position = {0.f, 0.f};
    sf::Vector2f scale = {1.f, 1.f};
    float rotation = 0.f;
};

struct Text
{
    static flecs::entity Create(const flecs::world& world, const TextParams& params);
};

} // namespace Prefabs


#endif

// Copyright (c) Eric Jeker 2025.

#pragma once



#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>

#include <flecs.h>
#include <string>

namespace Factories
{

struct SpriteParams
{
    std::string textureAsset = nullptr;
    sf::Vector2f origin = {0.f, 0.f};
    sf::Vector2f position = {0.f, 0.f};
    sf::Vector2f scale = {1.f, 1.f};
    float rotation = 0.f;
    float zOrder = 0;
};

struct Sprite
{
    static flecs::entity Create(const flecs::world& world, const SpriteParams& params);
};

} // namespace Factories




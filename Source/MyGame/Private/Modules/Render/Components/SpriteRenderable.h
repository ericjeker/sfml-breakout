// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef SPRITERENDERABLE_H
#define SPRITERENDERABLE_H

#include <SFML/Graphics/Sprite.hpp>


struct SpriteRenderable
{
    std::unique_ptr<sf::Sprite> sprite;
};


#endif

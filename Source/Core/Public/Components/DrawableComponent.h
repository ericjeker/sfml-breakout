// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef DRAWABLE_H
#define DRAWABLE_H

#include "Scenes/Component.h"

#include <SFML/Graphics/Drawable.hpp>

#include <memory>

enum class DrawableType
{
    Undefined,
    Sprite,
    Text,
    RectangleShape,
    CircleShape,
    Custom
};

struct DrawableComponent final : Component
{
    DrawableType type = DrawableType::Undefined;
    std::unique_ptr<sf::Drawable> drawable;
    sf::FloatRect cachedBounds = {{0.f, 0.f}, {0.f, 0.f}};
    bool boundsNeedUpdate = true;
    bool isVisible = true;
};


#endif

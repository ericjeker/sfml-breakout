// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef BOUNDINGBOXCOMPONENT_H
#define BOUNDINGBOXCOMPONENT_H

#include <SFML/Graphics/Rect.hpp>

#include <Scenes/Component.h>


struct BoundingBoxComponent : Component
{
    sf::FloatRect bounds;
    // If true, calculate from DrawableComponent; if false, use custom bounds
    bool useLocalBounds = true;
    bool boundsNeedsUpdate = true;

    // Custom bounds (when useLocalBounds is false)
    sf::Vector2f size = {0.f, 0.f};
    // Offset from the entity position
    sf::Vector2f offset = {0.f, 0.f};
};


#endif

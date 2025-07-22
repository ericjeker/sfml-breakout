// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef SFML_H
#define SFML_H

#include "Components/DrawableComponent.h"

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>


inline sf::FloatRect GetDrawableBounds(const DrawableComponent* drawableComp)
{
    if (!drawableComp->boundsNeedUpdate)
    {
        return drawableComp->cachedBounds;
    }

    sf::FloatRect bounds;
    switch (drawableComp->type)
    {
        case DrawableType::Sprite:
            bounds = static_cast<sf::Sprite*>(drawableComp->drawable.get())->getGlobalBounds();
            break;
        case DrawableType::Text:
            bounds = static_cast<sf::Text*>(drawableComp->drawable.get())->getGlobalBounds();
            break;
        case DrawableType::RectangleShape:
            bounds = static_cast<sf::RectangleShape*>(drawableComp->drawable.get())->getGlobalBounds();
            break;
        case DrawableType::CircleShape:
            bounds = static_cast<sf::CircleShape*>(drawableComp->drawable.get())->getGlobalBounds();
            break;
        default:
            bounds = sf::FloatRect(); // Default empty bounds
            break;
    }

    // Update cache
    const_cast<DrawableComponent*>(drawableComp)->cachedBounds = bounds;
    const_cast<DrawableComponent*>(drawableComp)->boundsNeedUpdate = false;

    return bounds;
}


#endif

// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef ANIMATION_H
#define ANIMATION_H

#include <SFML/System/Vector2.hpp>


struct AnimationComponent final : Component
{
    enum Type
    {
        ROTATE,
        SCALE,
        MOVE,
        FADE
    };

    Type type;
    float duration;
    float elapsed = 0.f;
    bool loop = false;

    // Animation-specific data
    sf::Vector2f startValue;
    sf::Vector2f endValue;
    float startFloat;
    float endFloat;
};


#endif

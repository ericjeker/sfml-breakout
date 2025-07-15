// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef ANIMATIONSYSTEM_H
#define ANIMATIONSYSTEM_H

#include "Systems/System.h"


class AnimationSystem final : public System
{
public:
    void Update(const std::vector<std::unique_ptr<Entity>>& entities, float deltaTime) override;

private:
    float lerp(const float a, const float b, const float t)
    {
        return a + t * (b - a);
    }

    sf::Vector2f lerp(const sf::Vector2f& a, const sf::Vector2f& b, float t)
    {
        return sf::Vector2f(lerp(a.x, b.x, t), lerp(a.y, b.y, t));
    }
};


#endif //ANIMATIONSYSTEM_H

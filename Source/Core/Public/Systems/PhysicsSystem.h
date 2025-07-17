// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef PHYSICSSYSTEM_H
#define PHYSICSSYSTEM_H

#include "System.h"


class PhysicsSystem final : public System
{
public:
    PhysicsSystem(sf::Vector2f gravity, float pixelsPerMeter);
    void Update(const std::vector<std::unique_ptr<Entity>>& entities, float deltaTime) override;

private:
    sf::Vector2f _gravity{0.f, 9.81f};
    float _pixelsPerMeter{1.f};
};


#endif

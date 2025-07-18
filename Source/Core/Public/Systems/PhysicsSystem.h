// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef PHYSICSSYSTEM_H
#define PHYSICSSYSTEM_H

#include "System.h"


class PhysicsSystem final : public System
{
public:
    PhysicsSystem(sf::Vector2f gravity, float pixelsPerCentimeters);
    void Update(const std::vector<std::unique_ptr<Entity>>& entities, float deltaTime) override;

private:
    sf::Vector2f _gravity{0.f, 0.f};
    float _pixelsPerCentimeters{1.f};
};


#endif

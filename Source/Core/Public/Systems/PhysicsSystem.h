// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef PHYSICSSYSTEM_H
#define PHYSICSSYSTEM_H

#include "System.h"


class PhysicsSystem final : public System
{
public:
    PhysicsSystem(sf::Vector2f gravity, float pixelsPerCentimeters);
    void Update(const std::unique_ptr<Entity>& entity, float deltaTime) override;

    void ToggleGravity();

private:
    sf::Vector2f _gravity{0.f, 0.f};
    // Persist the previous value of gravity for toggling
    sf::Vector2f _oldGravity;
    float _pixelsPerCentimeters{1.f};
};


#endif

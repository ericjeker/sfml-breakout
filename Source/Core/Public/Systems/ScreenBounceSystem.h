// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef SCREENBOUNCESYSTEM_H
#define SCREENBOUNCESYSTEM_H

#include "System.h"

class ScreenBounceSystem final : public System
{
public:
    explicit ScreenBounceSystem(sf::Vector2u screenSize);
    void Update(const std::unique_ptr<Entity>& entity, float deltaTime) override;

private:
    sf::Vector2u _screenSize;
};

#endif

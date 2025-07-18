// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef SYSTEM_H
#define SYSTEM_H

#include "Scenes/Entity.h"

#include <SFML/Graphics/RenderWindow.hpp>


class System
{
public:
    System() = default;
    virtual ~System() = default;

    virtual void Update(const std::unique_ptr<Entity>& entity, float deltaTime);
    virtual void Render(const std::unique_ptr<Entity>& entity, sf::RenderWindow& window);
    virtual void HandleEvent(const std::unique_ptr<Entity>& entity, const std::optional<sf::Event>& event, sf::RenderWindow& window);
};


#endif

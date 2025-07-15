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

    virtual void Update(const std::vector<std::unique_ptr<Entity>>& entities, float deltaTime);
    virtual void Render(const std::vector<std::unique_ptr<Entity>>& entities, sf::RenderWindow& window);
    virtual void HandleEvent(const std::vector<std::unique_ptr<Entity>>& entities, const std::optional<sf::Event>& event);
};


#endif

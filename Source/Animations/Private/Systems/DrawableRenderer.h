// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef DRAWABLERENDERER_H
#define DRAWABLERENDERER_H

#include "Scenes/Entity.h"
#include "Systems/System.h"

#include <SFML/Graphics/RenderWindow.hpp>


class DrawableRenderer final : public System
{
public:
    void Render(const std::vector<std::unique_ptr<Entity>>& entities, sf::RenderWindow& window) override;
    void Update(const std::vector<std::unique_ptr<Entity>>& entities, float deltaTime) override;
    void HandleEvent(const std::vector<std::unique_ptr<Entity>>& entities, const std::optional<sf::Event>& event) override;
};


#endif

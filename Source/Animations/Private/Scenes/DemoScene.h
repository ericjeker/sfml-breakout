// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef DEMOSCENE_H
#define DEMOSCENE_H

#include "Scenes/Scene.h"

#include <SFML/Graphics/CircleShape.hpp>


class DemoScene final : public Scene
{
public:
    DemoScene();
    ~DemoScene() override = default;

    void Update(float deltaTime) override;
    void Render(sf::RenderWindow& window) override;
    void HandleEvent(const std::optional<sf::Event>& event) override;

private:
    std::unique_ptr<sf::CircleShape> _circle;
};


#endif

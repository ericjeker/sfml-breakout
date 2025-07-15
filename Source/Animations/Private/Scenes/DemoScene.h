// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef DEMOSCENE_H
#define DEMOSCENE_H

#include "Scenes/Scene.h"

#include <SFML/Graphics/CircleShape.hpp>


class DemoScene final : public Scene
{
public:
    DemoScene() = default;
    void Initialize() override;
    void Shutdown() override;

private:
    std::unique_ptr<sf::CircleShape> _circle;
};


#endif

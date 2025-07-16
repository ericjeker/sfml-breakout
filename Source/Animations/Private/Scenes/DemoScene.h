// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef DEMOSCENE_H
#define DEMOSCENE_H

#include "Scenes/Scene.h"


class DemoScene final : public Scene
{
public:
    DemoScene(ResourceManager& resourceManager, EventManager& eventManager);
    void Initialize() override;
    void HandleEvent(const std::optional<sf::Event>& event, sf::RenderWindow& window) override;
};


#endif

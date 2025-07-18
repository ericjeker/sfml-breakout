// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef BOUNCINGBALLSCENE_H
#define BOUNCINGBALLSCENE_H

#include "Scenes/Scene.h"

class BouncingBallScene final : public Scene {
public:
    BouncingBallScene(ResourceManager& resourceManager, EventManager& eventManager);

    void Initialize() override;
    void HandleEvent(const std::optional<sf::Event>& event, sf::RenderWindow& window) override;

    std::unique_ptr<Entity> CreateBallEntity(sf::Vector2f position, sf::Vector2f velocity);
};


#endif

// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef PAUSESCENE_H
#define PAUSESCENE_H

#include "Scenes/Scene.h"

class PauseScene final : public Scene
{
public:
    PauseScene(ResourceManager& resourceManager, EventManager& eventManager);
    void Initialize() override;

private:
    static std::unique_ptr<Entity> CreateTextEntity(std::unique_ptr<sf::Text> text, sf::Vector2f position);
    static std::unique_ptr<Entity> CreateButtonEntity(
        std::unique_ptr<sf::Text> text,
        sf::Vector2f position,
        const std::function<void()>& callback
    );
};


#endif

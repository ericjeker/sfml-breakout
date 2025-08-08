// Copyright (c) Eric Jeker 2025.

#pragma once




#include "Managers/EventManager.h"
#include "Managers/ResourceManager.h"
#include "Scenes/Scene.h"

#include <SFML/Graphics/RenderWindow.hpp>

#include <SFML/Window/Event.hpp>

class MiniRtsScene final : public Scene
{
public:
    MiniRtsScene() = default;

    void Initialize() override;
    void HandleEvent(const std::optional<sf::Event>& event, sf::RenderWindow& window) override;

private:
    void AddBackground();
    void AddPlayerEntity();
    void AddTargetEntity();
};




// Copyright (c) Eric Jeker 2025.

#pragma once



#include "Core/Scenes/Scene.h"

#include <SFML/Graphics/Text.hpp>


class PauseScene final : public Scene
{
public:
    explicit PauseScene(flecs::world& world);
    void Initialize() override;
    void HandleEvent(const std::optional<sf::Event>& event) override;
};




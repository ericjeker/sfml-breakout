// Copyright (c) Eric Jeker 2025.

#pragma once
#include "Core/Scenes/Scene.h"


class GameWonScene final : public Scene
{
public:
    explicit GameWonScene(flecs::world& world);
    void Initialize() override;
    void HandleEvent(const std::optional<sf::Event>& event) override;
    void CreateUISystems(const flecs::world& world);
};

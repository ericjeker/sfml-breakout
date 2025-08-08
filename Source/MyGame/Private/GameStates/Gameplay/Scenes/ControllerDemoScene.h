// Copyright (c) Eric Jeker 2025.

#pragma once



#include "Core/Scenes/Scene.h"


class ControllerDemoScene final : public Scene
{
public:
    explicit ControllerDemoScene(flecs::world& world);
    void Initialize() override;
    void HandleEvent(const std::optional<sf::Event>& event) override;

private:
    static void CreateInputBindings(const flecs::world& world);
    void CreateMovementSystem(const flecs::world& world);
    void CreateUISystem(const flecs::world& world);
    void CreatePlayerEntity(const flecs::world& world);
};




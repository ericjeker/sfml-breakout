// Copyright (c) Eric Jeker 2025.

#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

#include <flecs.h>
#include <optional>


class GameState
{
public:
    explicit GameState(flecs::world& world);
    virtual ~GameState() = default;

    virtual void Enter();
    virtual void Exit();

    flecs::entity GetRootEntity() const;

    void Pause();
    void Resume();
    [[nodiscard]] bool IsPaused() const;

    virtual void HandleEvent(const std::optional<sf::Event>& event);

    flecs::world& GetWorld();
    [[nodiscard]] const flecs::world& GetWorld() const;

private:
    flecs::world& _world;
    flecs::entity _rootEntity;
};



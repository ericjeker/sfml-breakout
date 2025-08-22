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

    virtual void Enter() = 0;
    virtual void Exit() = 0;

    virtual void HandleEvent(const std::optional<sf::Event>& event);

    void Pause();
    void Resume();
    bool IsPaused() const;

    flecs::world& GetWorld();
    [[nodiscard]] const flecs::world& GetWorld() const;

private:
    flecs::world& _world;
};



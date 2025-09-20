// Copyright (c) Eric Jeker 2025.

#pragma once

#include <SFML/Graphics/RenderWindow.hpp>

#include <flecs.h>

/**
 * @brief The GameState is a Layer that we can push to the game stack using the GameStateManager.
 */
class GameState
{
public:
    explicit GameState(flecs::world& world);
    virtual ~GameState() = default;

    virtual void Enter();
    virtual void Exit();

    [[nodiscard]] flecs::entity GetRootEntity() const;

    void Pause();
    void Resume();
    [[nodiscard]] bool IsPaused() const;

    [[nodiscard]] flecs::world& GetWorld() ;
    [[nodiscard]] const flecs::world& GetWorld() const;

private:
    flecs::world& _world;
    flecs::entity _rootEntity;
};

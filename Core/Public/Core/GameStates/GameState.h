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

    // TODO: do I really need a state management in my game state?
    void Pause();
    void Resume();
    [[nodiscard]] bool IsPaused() const;

    [[nodiscard]] flecs::world& GetWorld() ;
    [[nodiscard]] const flecs::world& GetWorld() const;

private:
    flecs::world& _world;

    // TODO: do I really need a root entity for my game states?
    flecs::entity _rootEntity;
};

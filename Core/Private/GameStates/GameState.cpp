// Copyright (c) Eric Jeker 2025.

#include "Core/GameStates/GameState.h"

#include "Core/Logger.h"

GameState::GameState(flecs::world& world)
    : _world(world)
{
}

void GameState::Pause()
{
    LOG_DEBUG("(GameState::Pause)");
    _paused = true;
}

void GameState::Resume()
{
    LOG_DEBUG("(GameState::Resume)");
    _paused = false;
}

bool GameState::IsPaused() const
{
    return _paused;
}

void GameState::HandleEvent(const std::optional<sf::Event>& event)
{
}

flecs::world& GameState::GetWorld()
{
    return _world;
}

const flecs::world& GameState::GetWorld() const
{
    return _world;
}

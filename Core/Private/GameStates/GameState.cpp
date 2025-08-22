// Copyright (c) Eric Jeker 2025.

#include "Core/GameStates/GameState.h"

#include "Core/Utils/Logger.h"

GameState::GameState(flecs::world& world)
    : _world(world)
{
}

void GameState::Pause()
{
    LOG_DEBUG("(GameState::Pause)");
}

void GameState::Resume()
{
    LOG_DEBUG("(GameState::Resume)");
}

bool GameState::IsPaused() const
{
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

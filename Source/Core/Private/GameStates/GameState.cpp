// Copyright (c) Eric Jeker 2025.

#include "Core/GameStates/GameState.h"

#include "Core/Logger.h"

GameState::GameState(const flecs::world& world)
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

void GameState::Update(float deltaTime)
{
}

const flecs::world& GameState::GetWorld() const
{
    return _world;
}

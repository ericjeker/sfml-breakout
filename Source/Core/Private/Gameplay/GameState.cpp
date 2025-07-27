// Copyright (c) Eric Jeker 2025.

#include "Gameplay/GameState.h"

#include "Logger.h"

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

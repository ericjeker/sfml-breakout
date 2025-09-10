// Copyright (c) Eric Jeker 2025.

#include "Core/Managers/GameStateManager.h"

void GameStateManager::PushState(std::unique_ptr<GameState> state)
{
    if (!_stateStack.empty())
    {
        _stateStack.top()->Pause();
    }

    _stateStack.push(std::move(state));
    _stateStack.top()->Enter();
}

void GameStateManager::PopState()
{
    if (_stateStack.empty())
    {
        return;
    }

    _stateStack.top()->Exit();
    _stateStack.pop();

    if (!_stateStack.empty())
    {
        _stateStack.top()->Resume();
    }
}

void GameStateManager::ChangeState(std::unique_ptr<GameState> state)
{
    if (!_stateStack.empty())
    {
        _stateStack.top()->Exit();
        _stateStack.pop();
    }

    _stateStack.push(std::move(state));
    _stateStack.top()->Enter();
}

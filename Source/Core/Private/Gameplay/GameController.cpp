// Copyright (c) Eric Jeker 2025.

#include "Gameplay/GameController.h"

void GameController::PushState(std::unique_ptr<GameState> state)
{
    if (!_stateStack.empty())
    {
        _stateStack.top()->Pause();
    }

    _stateStack.push(std::move(state));
    _stateStack.top()->Enter();
}

void GameController::PopState()
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

void GameController::ChangeState(std::unique_ptr<GameState> state)
{
    if (!_stateStack.empty())
    {
        _stateStack.top()->Exit();
        _stateStack.pop();
    }

    _stateStack.push(std::move(state));
    _stateStack.top()->Enter();
}

void GameController::Update(const float deltaTime)
{
    if (_stateStack.empty())
    {
        return;
    }

    _stateStack.top()->Update(deltaTime);
}

void GameController::Render(sf::RenderWindow& window)
{
    if (_stateStack.empty())
    {
        return;
    }

    _stateStack.top()->Render(window);
}

void GameController::HandleEvent(const std::optional<sf::Event>& event)
{
    if (_stateStack.empty())
    {
        return;
    }

    _stateStack.top()->HandleEvent(event);
}

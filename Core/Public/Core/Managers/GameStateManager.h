// Copyright (c) Eric Jeker 2025.

#pragma once

#include "Core/GameStates/GameState.h"

#include <SFML/Graphics/RenderWindow.hpp>

#include <memory>
#include <stack>


/**
 * @brief Manage game states with a stack state pattern. This is similar to a LayerManager.
 *
 * Examples of State-Scene Relationships:
 *
 * - One-to-One Mapping, one state has one scene loaded from a manifest file
 * - Dynamic Loading, the GameState is identical but loaded with a different manifest file
 * - Multiple Scene per State: the GameState loads different scenes by itself
 */
class GameStateManager
{
public:
    // State stack management
    void PushState(std::unique_ptr<GameState> state);
    void PopState();
    void ChangeState(std::unique_ptr<GameState> state);

private:
    std::unique_ptr<GameState> _currentState;
    std::stack<std::unique_ptr<GameState>> _stateStack;
};

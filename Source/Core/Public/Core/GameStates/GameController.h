// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include "GameState.h"

#include <SFML/Graphics/RenderWindow.hpp>

#include <memory>
#include <stack>


/**
 * @class: GameController
 *
 * @short Game Controller with a stack state pattern.
 *
 * Given we use a Stack State, only the top State is Updated.
 *
 * Examples of State-Scene Relationships:
 *
 * - One-to-One Mapping, one state has one scene loaded from a manifest file
 * - Dynamic Loading, the GameState is identical but loaded with a different manifest file
 * - Multiple Scene per State: the GameState loads different scenes by itself
 */
class GameController
{
public:
    // State stack management
    void PushState(std::unique_ptr<GameState> state);
    void PopState();
    void ChangeState(std::unique_ptr<GameState> state);

    // Delegate event handling to the current top state
    void HandleEvent(const std::optional<sf::Event>& event);

private:
    std::unique_ptr<GameState> _currentState;
    std::stack<std::unique_ptr<GameState>> _stateStack;
};


#endif

// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef ACTIONINTENT_H
#define ACTIONINTENT_H

#include <SFML/Window/Event.hpp>

enum class PlayerAction
{
    None,
    MoveLeft,
    MoveRight,
    MoveUp,
    MoveDown,
};

/**
 * @brief The Action component is the registration of an intent to do an action.
 * The event that triggered that intent is also registered in the component and holds
 * the parameters of the action, for example, the amplitude of the movement.
 */
struct Action
{
    PlayerAction action = PlayerAction::None;
    // sf::Event event;
};


#endif

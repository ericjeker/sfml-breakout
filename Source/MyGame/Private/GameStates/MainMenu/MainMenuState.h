// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef MAINMENUSTATE_H
#define MAINMENUSTATE_H

#include "GameStates/GameState.h"


class MainMenuState final : public GameState
{
public:
    void Enter() override;
    void Exit() override;

    void HandleEvent(const std::optional<sf::Event>& event, sf::RenderWindow& window) override;
};


#endif

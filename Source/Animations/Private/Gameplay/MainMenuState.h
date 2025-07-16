// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef MAINMENUSTATE_H
#define MAINMENUSTATE_H

#include "Gameplay/GameState.h"
#include "Managers/GameService.h"


class MainMenuState final : public GameState
{
public:
    explicit MainMenuState(GameService& gameService);
    ~MainMenuState() override = default;

    void Enter() override;
    void Exit() override;

    void Update(float deltaTime) override;
    void HandleEvent(const std::optional<sf::Event>& event, sf::RenderWindow& window) override;

private:
    bool _isPaused = false;
};
#endif

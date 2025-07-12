// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef MAINMENUSTATE_H
#define MAINMENUSTATE_H

#include "Gameplay/GameController.h"
#include "Managers/SceneManager.h"

class MainMenuState final : public GameState
{
public:
    explicit MainMenuState(SceneManager& sceneManager);
    ~MainMenuState() override = default;

    void Enter() override;
    void Exit() override;
    void Update(float deltaTime) override;
    void Render(sf::RenderWindow& window) override;
    void HandleEvent(const sf::Event& event) override;

private:
    SceneManager& _sceneManager;
};


#endif

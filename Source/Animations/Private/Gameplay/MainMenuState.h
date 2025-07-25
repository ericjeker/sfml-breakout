// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef MAINMENUSTATE_H
#define MAINMENUSTATE_H

#include "../Events/RequestGameExit.h"
#include "../Events/RequestGameResume.h"
#include "../Scenes/BouncingBallScene.h"
#include "../Scenes/PauseScene.h"
#include "GameInstance.h"
#include "Gameplay/GameState.h"
#include "Managers/EventManager.h"
#include "Managers/GameService.h"
#include "Managers/SceneManager.h"
// #include "MiniRtsScene.h"


class MainMenuState final : public GameState
{
public:
    MainMenuState();
    ~MainMenuState() override = default;

    void Enter() override;
    void Exit() override;

    void Update(float deltaTime) override;
    void HandleEvent(const std::optional<sf::Event>& event, sf::RenderWindow& window) override;

private:
    bool _isPaused = false;

    EventListener<RequestGameResume> _resumeGameListener = [this](const RequestGameResume& event, void* sender)
    {
        GameService::Get<SceneManager>().UnloadScene<PauseScene>();
        GameService::Get<SceneManager>().GetScene<BouncingBallScene>().Resume();
        _isPaused = false;
    };

    EventListener<RequestGameExit> _exitGameListener = [this](const RequestGameExit& event, void* sender)
    { GameService::Get<GameInstance>().RequestExit(); };
};
#endif

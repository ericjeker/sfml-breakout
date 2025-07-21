// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef MAINMENUSTATE_H
#define MAINMENUSTATE_H

#include "../Events/ExitGameRequestedEvent.h"
#include "../Events/ResumeGameRequestedEvent.h"
#include "../Scenes/BouncingBallScene.h"
#include "../Scenes/PauseScene.h"
#include "Gameplay/GameState.h"
#include "Managers/GameService.h"
#include "Managers/SceneManager.h"


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

    EventListener<ResumeGameRequestedEvent> _resumeGameListener = [this](const ResumeGameRequestedEvent& event, void* sender)
    {
        GetGameService().Get<SceneManager>().UnloadScene<PauseScene>();
        GetGameService().Get<SceneManager>().GetScene<BouncingBallScene>().Resume();
        _isPaused = false;
    };

    EventListener<ExitGameRequestedEvent> _exitGameListener = [this](const ExitGameRequestedEvent& event, void* sender)
    { GetGameService().Get<sf::RenderWindow>().close(); };
};
#endif

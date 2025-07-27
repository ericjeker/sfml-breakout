// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef GAMEPLAYSTATE_H
#define GAMEPLAYSTATE_H

#include "../Events/RequestGameResume.h"
#include "../Scenes/BouncingBallScene.h"
#include "../Scenes/PauseScene.h"
#include "GameInstance.h"
#include "Gameplay/GameState.h"
#include "Managers/EventManager.h"
#include "Managers/GameService.h"
#include "Managers/SceneManager.h"


class GameplayState final : public GameState
{
public:
    void Enter() override;
    void Exit() override;

    void HandleEvent(const std::optional<sf::Event>& event, sf::RenderWindow& window) override;

private:
    bool _isPaused = false;

    EventListener<RequestGameResume> _resumeGameListener = [this](const RequestGameResume& event, void* sender)
    {
        GameService::Get<SceneManager>().UnloadScene<PauseScene>();
        GameService::Get<SceneManager>().GetScene<BouncingBallScene>().Resume();
        _isPaused = false;
    };

};
#endif

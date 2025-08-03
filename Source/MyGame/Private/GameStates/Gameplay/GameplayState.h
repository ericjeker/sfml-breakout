// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef GAMEPLAYSTATE_H
#define GAMEPLAYSTATE_H

#include "Events/ResumeGame.h"
#include "GameStates/GameState.h"
#include "Managers/EventManager.h"
#include "Managers/GameService.h"
#include "Managers/SceneManager.h"
#include "Scenes/BouncingBallScene.h"
#include "Scenes/ControllerDemoScene.h"
#include "Scenes/PauseScene.h"


class GameplayState final : public GameState
{
public:
    void Enter() override;
    void Exit() override;

    void HandleEvent(const std::optional<sf::Event>& event) override;

private:
    EventListener<ResumeGame> _resumeGameListener = [this](const ResumeGame& event, void* sender)
    {
        GameService::Get<SceneManager>().UnloadScene<PauseScene>();
        GameService::Get<SceneManager>().GetScene<ControllerDemoScene>().Resume();
    };
};
#endif

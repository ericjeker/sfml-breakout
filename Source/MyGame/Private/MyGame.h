// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef MYGAME_H
#define MYGAME_H

#include "Events/RequestGameExit.h"
#include "Events/RequestReturnToMainMenu.h"
#include "Events/RequestStartGame.h"
#include "GameInstance.h"
#include "Gameplay/GameController.h"
#include "Gameplay/GameplayState.h"
#include "Gameplay/MainMenuState.h"
#include "Managers/EventManager.h"
#include "Managers/GameService.h"

#include <functional>


class MyGame final : public GameInstance
{
public:
    void Initialize() override;
    void Shutdown() override;

private:
    EventListener<RequestGameExit> _exitGameListener = [this](const RequestGameExit& event, void* sender)
    { RequestExit(); };

    EventListener<RequestStartGame> _startGameListener = [](const RequestStartGame& event, void* sender)
    { GameService::Get<GameController>().ChangeState(std::make_unique<GameplayState>()); };

    EventListener<RequestReturnToMainMenu> _returnToMainMenuListener = [](const RequestReturnToMainMenu& event, void* sender)
    {
        GameService::Get<GameController>().ChangeState(std::make_unique<MainMenuState>());
    };
};


#endif

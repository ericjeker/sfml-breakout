// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef MYGAME_H
#define MYGAME_H

#include "Events/ExitGame.h"
#include "Events/NavigateToMainMenu.h"
#include "Events/StartGame.h"
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
    EventListener<ExitGame> _exitGameListener = [this](const ExitGame& event, void* sender)
    { RequestExit(); };

    EventListener<StartGame> _startGameListener = [](const StartGame& event, void* sender)
    { GameService::Get<GameController>().ChangeState(std::make_unique<GameplayState>()); };

    EventListener<NavigateToMainMenu> _returnToMainMenuListener = [](const NavigateToMainMenu& event, void* sender)
    {
        GameService::Get<GameController>().ChangeState(std::make_unique<MainMenuState>());
    };
};


#endif

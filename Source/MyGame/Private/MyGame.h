// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef MYGAME_H
#define MYGAME_H

#include "Core/GameInstance.h"
#include "Core/GameStates/GameController.h"
#include "Core/Managers/EventManager.h"
#include "Core/Managers/GameService.h"
#include "Events/ExitGame.h"
#include "Events/NavigateToMainMenu.h"
#include "Events/StartGame.h"
#include "GameStates/Gameplay/GameplayState.h"
#include "GameStates/MainMenu/MainMenuState.h"

#include <functional>


class MyGame final : public GameInstance
{
public:
    void Initialize() override;
    void Shutdown() override;

private:
    EventListener<ExitGame> _exitGameListener = [this](const ExitGame& event, void* sender) { RequestExit(); };
    EventListener<StartGame> _startGameListener = [this](const StartGame& event, void* sender)
    { GameService::Get<GameController>().ChangeState(std::make_unique<GameplayState>(GetWorld())); };
    EventListener<NavigateToMainMenu> _returnToMainMenuListener = [this](const NavigateToMainMenu& event, void* sender)
    { GameService::Get<GameController>().ChangeState(std::make_unique<MainMenuState>(GetWorld())); };
};


#endif

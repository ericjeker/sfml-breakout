// Copyright (c) Eric Jeker 2025.

#include "MyGame.h"

#include "Events/RequestReturnToMainMenu.h"
#include "Events/RequestStartGame.h"
#include "Gameplay/GameController.h"
#include "Gameplay/GameplayState.h"
#include "Gameplay/MainMenuState.h"
#include "Logger.h"
#include "Managers/ResourceManager.h"

void MyGame::Initialize()
{
    LOG_DEBUG("(MyGame::Initialization): Load Resources");
    GameService::Get<ResourceManager>().LoadResourcesFromManifest("Assets/Manifest.json");
    LOG_DEBUG("(MyGame::Initialization): Load the Main Menu State");
    GameService::Get<GameController>().ChangeState(std::make_unique<MainMenuState>());

    // --- Add event listeners ---
    GameService::Get<EventManager>().Subscribe<RequestGameExit>(_exitGameListener);
    GameService::Get<EventManager>().Subscribe<RequestStartGame>(_startGameListener);
    GameService::Get<EventManager>().Subscribe<RequestReturnToMainMenu>(_returnToMainMenuListener);
}

void MyGame::Shutdown()
{
    LOG_DEBUG("(MyGame::Shutdown)");
}

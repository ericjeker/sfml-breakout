// Copyright (c) Eric Jeker 2025.

#include "Gameplay/GameController.h"
#include "Gameplay/MainMenuState.h"
#include "Logger.h"
#include "Managers/ResourceManager.h"
#include "MyGame.h"

void MyGame::Initialize()
{
    LOG_DEBUG("(MyGame::Initialization): Load Resources");
    GameService::Get<ResourceManager>().LoadResourcesFromManifest("Assets/Manifest.json");
    LOG_DEBUG("(MyGame::Initialization): Load the Main Menu State");
    GameService::Get<GameController>().ChangeState(std::make_unique<MainMenuState>());
}

void MyGame::Shutdown()
{
    LOG_DEBUG("(MyGame::Shutdown)");
}

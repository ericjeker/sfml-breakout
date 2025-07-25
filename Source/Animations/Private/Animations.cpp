// Copyright (c) Eric Jeker 2025.

#include "Animations.h"

#include "Gameplay/GameController.h"
#include "Gameplay/MainMenuState.h"
#include "Logger.h"
#include "Managers/ResourceManager.h"

void Animations::Initialize()
{
    LOG_DEBUG("(Animations::Initialization): Load Resources");
    GameService::Get<ResourceManager>().LoadResourcesFromManifest("Assets/Manifest.json");
    LOG_DEBUG("(Animations::Initialization): Load the Main Menu State");
    GameService::Get<GameController>().ChangeState(std::make_unique<MainMenuState>());
}

void Animations::Shutdown()
{
    LOG_DEBUG("(Animations::Shutdown)");
}

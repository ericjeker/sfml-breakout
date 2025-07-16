// Copyright (c) Eric Jeker 2025.

#include "Animations.h"

#include "Gameplay/GameController.h"
#include "Gameplay/MainMenuState.h"
#include "Logger.h"
#include "Managers/ResourceManager.h"

void Animations::Initialize()
{
    LOG_DEBUG("(Animations::Initialization): Load Resources");
    GetGameService().Get<ResourceManager>().LoadResourcesFromManifest("Assets/Manifest.json");
    LOG_DEBUG("(Animations::Initialization): Load the Main Menu State");
    GetGameService().Get<GameController>().ChangeState(std::make_unique<MainMenuState>(GetGameService()));
}

void Animations::Shutdown()
{
    LOG_DEBUG("(Animations::Shutdown)");
}

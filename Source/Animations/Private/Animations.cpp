// Copyright (c) Eric Jeker 2025.

#include "Animations.h"

#include "Gameplay/GameController.h"
#include "Gameplay/MainMenuState.h"
#include "Logger.h"

void Animations::Initialize()
{
    LOG_DEBUG("(Animations::Initialization)");
    GetGameService()->Get<GameController>()->ChangeState(
        std::make_unique<MainMenuState>(*GetGameService()->Get<SceneManager>())
    );
}

void Animations::Shutdown()
{
    LOG_DEBUG("(Animations::Shutdown)");
}

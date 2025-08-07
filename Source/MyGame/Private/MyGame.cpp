// Copyright (c) Eric Jeker 2025.

#include "MyGame.h"

#include "Core/GameStates/GameController.h"
#include "Core/Logger.h"
#include "Core/Managers/AudioManager.h"
#include "Core/Managers/ResourceManager.h"
#include "Core/Modules/Render/RenderModule.h"
#include "Core/Modules/UI/UIModule.h"
#include "Events/NavigateToMainMenu.h"
#include "Events/StartGame.h"
#include "GameStates/MainMenu/MainMenuState.h"

void MyGame::Initialize()
{
    // --- Load all resources from the manifest file
    LOG_DEBUG("(MyGame::Initialization): Load Resources");
    GameService::Get<ResourceManager>().LoadResourcesFromManifest("Assets/Manifest.json");

    // --- Initialize the World with global systems and components ---
    // clang-format off
    GetWorld().import<Modules::UIModule>();
    GetWorld().import<Modules::RenderModule>();
    // clang-format on

    // --- Load the Main Menu State ---
    LOG_DEBUG("(MyGame::Initialization): Load the Main Menu State");
    GameService::Get<GameController>().ChangeState(std::make_unique<MainMenuState>(GetWorld()));

    // --- Start the main theme ---
    // GameService::Get<AudioManager>().PlayMusic("8mm");

    // --- Add global event listeners ---
    GameService::Get<EventManager>().Subscribe<ExitGame>(_exitGameListener);
    GameService::Get<EventManager>().Subscribe<StartGame>(_startGameListener);
    GameService::Get<EventManager>().Subscribe<NavigateToMainMenu>(_returnToMainMenuListener);
}

void MyGame::Shutdown()
{
    LOG_DEBUG("(MyGame::Shutdown)");
    GameService::Get<AudioManager>().StopMusic();
}

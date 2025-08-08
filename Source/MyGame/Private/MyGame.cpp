// Copyright (c) Eric Jeker 2025.

#include "MyGame.h"

#include "Core/GameStates/GameStateManager.h"
#include "Core/Logger.h"
#include "Core/Managers/AudioManager.h"
#include "Core/Managers/GameService.h"
#include "Core/Managers/ResourceManager.h"
#include "Core/Modules/Control/ControlModule.h"
#include "Core/Modules/Lifecycle/LifecycleModule.h"
#include "Core/Modules/Physics/PhysicsModule.h"
#include "Core/Modules/Render/RenderModule.h"
#include "Core/Modules/Scene/SceneModule.h"
#include "Core/Modules/UI/UIModule.h"
#include "GameStates/MainMenu/MainMenuState.h"

void MyGame::Initialize()
{
    // --- Load all resources from the manifest file
    LOG_DEBUG("(MyGame::Initialization): Load Resources");
    GameService::Get<ResourceManager>().LoadResourcesFromManifest("Assets/Manifest.json");

    // --- Initialize the World with global systems and singletons ---
    // clang-format off
    auto& world = GetWorld();
    world.import<Modules::LifecycleModule>();
    world.import<Modules::SceneModule>();
    world.import<Modules::UIModule>();
    world.import<Modules::ControlModule>();
    world.import<Modules::PhysicsModule>();
    world.import<Modules::RenderModule>();
    // clang-format on

    // --- Load the Main Menu State ---
    LOG_DEBUG("(MyGame::Initialization): Load the Main Menu State");
    GameService::Get<GameStateManager>().ChangeState(std::make_unique<MainMenuState>(world));

    // --- Start the main theme ---
    // GameService::Get<AudioManager>().PlayMusic("8mm");
}

void MyGame::Shutdown()
{
    LOG_DEBUG("(MyGame::Shutdown)");
    GameService::Get<AudioManager>().StopMusic();
}

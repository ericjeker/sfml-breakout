// Copyright (c) Eric Jeker 2025.

#include "Breakout.h"

#include "GameStates/MainMenu/MainMenuState.h"

#include "Core/Managers/AudioManager.h"
#include "Core/Managers/GameService.h"
#include "Core/Managers/GameStateManager.h"
#include "Core/Managers/ResourceManager.h"
#include "Core/Modules/Control/ControlModule.h"
#include "Core/Modules/Lifetime/LifetimeModule.h"
#include "Core/Modules/Particles/ParticlesModule.h"
#include "Core/Modules/Physics/PhysicsModule.h"
#include "Core/Modules/Render/RenderModule.h"
#include "Core/Modules/UI/UIModule.h"
#include "Core/Utils/Logger.h"

void Breakout::Initialize()
{
    // --- Load all resources from the manifest file ---
    LOG_DEBUG("Breakout::Initialization: Load Resources");
    GameService::Get<ResourceManager>().LoadResourcesFromManifest("Assets/Manifest.json");

    // --- Initialize the World with global systems and singletons ---
    // clang-format off
    auto& world = GetWorld();
    world.import<Modules::LifetimeModule>();
    world.import<Modules::UIModule>();
    world.import<Modules::ControlModule>();
    world.import<Modules::PhysicsModule>();
    world.import<Modules::RenderModule>();
    world.import<Modules::ParticlesModule>();
    // clang-format on

    // --- Add the GamePaused singleton ---
    // TODO: so in the end I haven't managed pause like this, but let's keep a trace of the first iteration
    //world.add<GamePaused>({});

    // --- Load the initial state where the initial scene will be loaded ---
    LOG_DEBUG("Breakout::Initialization: Load the Main Menu State");
    GameService::Get<GameStateManager>().ChangeState(std::make_unique<MainMenuState>(world));

    // --- Start the main theme ---
    // TODO: disabled during development as this was driving me insane
    // GameService::Get<AudioManager>().PlayMusic("8mm");
}

void Breakout::Shutdown()
{
    LOG_DEBUG("Breakout::Shutdown");
    GameService::Get<AudioManager>().StopMusic();
}

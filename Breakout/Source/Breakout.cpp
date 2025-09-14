// Copyright (c) Eric Jeker 2025.

#include "Breakout.h"

#include "GameStates/MainMenu/MainMenuState.h"

#include "Core/Managers/AudioManager.h"
#include "Core/Managers/GameService.h"
#include "Core/Managers/GameStateManager.h"
#include "Core/Managers/ResourceManager.h"
#include "Core/Modules/Camera/CameraModule.h"
#include "Core/Modules/Input/InputModule.h"
#include "Core/Modules/Lifetime/LifetimeModule.h"
#include "Core/Modules/Particles/ParticlesModule.h"
#include "Core/Modules/Physics/PhysicsModule.h"
#include "Core/Modules/Render/RenderModule.h"
#include "Core/Modules/UI/UIModule.h"
#include "Core/Utils/Logger.h"


void Breakout::Initialize()
{
    GameInstance::Initialize();

    // --- Load all resources from the manifest file ---
    LOG_DEBUG("Breakout::Initialization: Load Resources");
    GameService::Get<ResourceManager>().LoadResourcesFromManifest("Assets/Manifest.json");

    // --- Initialize the World with global systems and singletons ---
    auto& world = GetWorld();

    // Creates REST server on port 27750
    world.set<flecs::Rest>({});

    // clang-format off
    world.import<flecs::stats>(); // gather stats
    world.import<Modules::CameraModule>();
    world.import<Modules::InputModule>();
    world.import<Modules::LifetimeModule>();
    world.import<Modules::PhysicsModule>();
    world.import<Modules::ParticlesModule>();
    world.import<Modules::RenderModule>();
    world.import<Modules::UIModule>();
    // clang-format on

    // --- Load the initial state where the initial scene will be loaded ---
    LOG_DEBUG("Breakout::Initialization: Load the Main Menu State");
    GameService::Get<GameStateManager>().ChangeState(std::make_unique<MainMenuState>(world));

    // --- Start the main theme ---
    // TODO: disabled during development as this was driving me insane
    // GameService::Get<AudioManager>().PlayMusic("8mm");
}

void Breakout::Shutdown()
{
    GameInstance::Shutdown();

    LOG_DEBUG("Breakout::Shutdown");
    GameService::Get<AudioManager>().StopMusic();
}

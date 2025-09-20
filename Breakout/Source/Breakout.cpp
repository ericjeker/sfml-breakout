// Copyright (c) Eric Jeker 2025.

#include "Breakout.h"

#include "GameStates/Menu/MenuState.h"
#include "Modules/Breakout/BreakoutModule.h"
#include "Modules/Debug/DebugModule.h"

#include "Core/GameService.h"
#include "Core/Managers/AudioManager.h"
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

    // clang-format off
#ifdef DEBUG
    // Creates REST server on port 27750
    LOG_DEBUG("Breakout::Initialization: Creating flecs REST server");
    world.set<flecs::Rest>({});
    world.import<flecs::stats>();
#endif
    world.import<Core::Modules::CameraModule>();
    world.import<Core::Modules::InputModule>();
    world.import<Core::Modules::LifetimeModule>();
    world.import<Core::Modules::PhysicsModule>();
    world.import<Core::Modules::ParticlesModule>();
    world.import<Core::Modules::RenderModule>();
    world.import<Core::Modules::UIModule>();
    world.import<Modules::BreakoutModule>();
    world.import<Modules::DebugModule>();
    // clang-format on

    // --- Load the initial state where the initial scene will be loaded ---
    LOG_DEBUG("Breakout::Initialization: Load the Main Menu State");
    GameService::Get<GameStateManager>().ChangeState(std::make_unique<MenuState>(world));

    // --- Start the main theme ---
    // TODO: disabled during development as this was driving me insane
    // GameService::Get<AudioManager>().PlayMusic("8mm");
}

void Breakout::Shutdown()
{
    GameService::Get<AudioManager>().StopMusic();
    GameInstance::Shutdown();
}

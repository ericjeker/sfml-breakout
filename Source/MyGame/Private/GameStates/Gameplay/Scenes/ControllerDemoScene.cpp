// Copyright (c) Eric Jeker 2025.

#include "ControllerDemoScene.h"

#include "Core/Configuration.h"
#include "Core/Logger.h"
#include "Core/Managers/GameService.h"
#include "Core/Managers/SceneManager.h"
#include "Core/Modules/Control/Components/Command.h"
#include "Core/Modules/Control/Components/CommandQueue.h"
#include "Core/Modules/Control/Components/PossessedByPlayer.h"
#include "Core/Modules/Control/Components/Target.h"
#include "Core/Modules/Control/Singletons/InputBindings.h"
#include "Core/Modules/Physics/Components/Acceleration.h"
#include "Core/Modules/Physics/Components/Friction.h"
#include "Core/Modules/Physics/Components/Gravity.h"
#include "Core/Modules/Physics/Components/Velocity.h"
#include "Core/Modules/Render/Prefabs/Rectangle.h"
#include "Modules/ControlDemo/Components/MoveIntent.h"
#include "Modules/ControlDemo/Components/PauseIntent.h"
#include "Modules/ControlDemo/Components/ResumeIntent.h"
#include "PauseScene.h"


ControllerDemoScene::ControllerDemoScene(flecs::world& world)
    : Scene(world)
{
}

void ControllerDemoScene::Initialize()
{
    Scene::Initialize();
    LOG_DEBUG("(ControllerDemoScene:Initialize)");

    const auto& world = GetWorld();

    CreateInputBindings(world);
    CreateMovementSystem(world);
    CreateUISystem(world);
    CreatePlayerEntity(world);
}

void ControllerDemoScene::CreateInputBindings(const flecs::world& world)
{
    auto moveLeft = world.prefab("Command::MoveLeft").add<Command>().set<MoveIntent>({{-1.f, 0.f}});
    auto moveRight = world.prefab("Command::MoveRight").add<Command>().set<MoveIntent>({{1.f, 0.f}});
    auto moveUp = world.prefab("Command::MoveUp").add<Command>().set<MoveIntent>({{0.f, -1.f}});
    auto moveDown = world.prefab("Command::MoveDown").add<Command>().set<MoveIntent>({{0.f, 1.f}});
    auto pauseGame = world.prefab("Command::PauseGame").add<Command>().add<PauseIntent>();

    world.set<InputBindings>({{
        {InputKey::Keyboard(sf::Keyboard::Key::W), moveUp},
        {InputKey::Keyboard(sf::Keyboard::Key::A), moveLeft},
        {InputKey::Keyboard(sf::Keyboard::Key::S), moveDown},
        {InputKey::Keyboard(sf::Keyboard::Key::D), moveRight},
        {InputKey::Keyboard(sf::Keyboard::Key::Escape), pauseGame},
    }});
}

void ControllerDemoScene::CreateMovementSystem(const flecs::world& world)
{
    world.system<const MoveIntent, const Target>("MovementSystem")
        .each(
            [](const flecs::entity& cmd, const MoveIntent& i, const Target& t)
            {
                auto& [acceleration] = t.entity.get_mut<Acceleration>();
                acceleration.y += i.accelerate.y * 1000.f;
                acceleration.x += i.accelerate.x * 1000.f;

                // Destroy the command entity
                cmd.destruct();
            }
        )
        .child_of(GetRootEntity());
}

void ControllerDemoScene::CreateUISystem(const flecs::world& world)
{
    // Query for PauseIntent and pause the Scene
    world.system<const PauseIntent>("PauseSystem")
        .each(
            [&](const flecs::entity& cmd, const PauseIntent& p)
            {
                LOG_DEBUG("(PauseSystem)");
                auto& sceneManager = GameService::Get<SceneManager>();
                sceneManager.GetScene<ControllerDemoScene>().Pause();
                sceneManager.LoadScene<PauseScene>(SceneLoadMode::Additive);

                // Destroy the command entity
                cmd.destruct();
            }
        )
        .child_of(GetRootEntity());

    world.system<const ResumeIntent>("ResumeSystem")
        .each(
            [&](const flecs::entity& cmd, const ResumeIntent& p)
            {
                LOG_DEBUG("(ResumeSystem)");
                auto& sceneManager = GameService::Get<SceneManager>();
                sceneManager.UnloadScene<PauseScene>();
                sceneManager.GetScene<ControllerDemoScene>().Resume();

                // Destroy the command entity
                cmd.destruct();
            }
        )
        .child_of(GetRootEntity());
}

void ControllerDemoScene::CreatePlayerEntity(const flecs::world& world)
{
    constexpr float CENTER_X = Configuration::WINDOW_SIZE.x / 2;
    constexpr float CENTER_Y = Configuration::WINDOW_SIZE.y / 2;

    const flecs::entity entity = Prefabs::Rectangle::Create(
                                     world,
                                     {
                                         .size = {100.f, 20.f},
                                         .color = sf::Color::Red,
                                         .origin = {0.5f, 0.5f},
                                         .position = {CENTER_X, CENTER_Y},
                                         .zOrder = 0.f,
                                     }
    )
                                     .child_of(GetRootEntity());

    entity.add<PossessedByPlayer>().set<CommandQueue>({});
    entity.set<Acceleration>({});
    entity.set<Friction>({.friction = 10.f});
    entity.set<Gravity>({PhysicsConstants::NO_GRAVITY});
    entity.set<Velocity>({});
}

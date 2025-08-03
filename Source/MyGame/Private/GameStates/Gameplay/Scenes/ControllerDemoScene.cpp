// Copyright (c) Eric Jeker 2025.

#include "ControllerDemoScene.h"

#include "Configuration.h"
#include "Logger.h"
#include "Managers/GameService.h"
#include "Managers/SceneManager.h"
#include "Modules/Control/Components/Command.h"
#include "Modules/Control/Components/CommandQueue.h"
#include "Modules/Control/Components/PossessedByPlayer.h"
#include "Modules/Control/ControlModule.h"
#include "Modules/Control/Singletons/InputBindings.h"
#include "Modules/ControlDemo/Components/MoveIntent.h"
#include "Modules/ControlDemo/Components/PauseIntent.h"
#include "Modules/ControlDemo/Components/Target.h"
#include "Modules/Physics/Components/Acceleration.h"
#include "Modules/Physics/Components/Friction.h"
#include "Modules/Physics/Components/Gravity.h"
#include "Modules/Physics/Components/Velocity.h"
#include "Modules/Physics/PhysicsModule.h"
#include "Modules/Render/Prefabs/Rectangle.h"
#include "Modules/Render/RenderModule.h"
#include "PauseScene.h"


void ControllerDemoScene::Initialize()
{
    auto world = GetWorld();

    // Clang Format is confused...
    // clang-format off
    world.import<Modules::ControlModule>();
    world.import<Modules::PhysicsModule>();
    world.import<Modules::RenderModule>();
    // clang-format on

    CreateInputBindings(world);
    CreateMovementSystem(world);
    CreateUISystem(world);
    CreatePlayerEntity(world);

    // Input System, translate Input -> Command Entities
    world.system<const InputBindings>()
        .term_at(0)
        .singleton()
        .kind(flecs::PostLoad)
        .each(
            [&](flecs::iter& it, size_t, const InputBindings& b)
            {
                if (!IsLoaded() || IsPaused())
                {
                    return;
                }

                const auto q = it.world().query<const PossessedByPlayer>();

                // Loop each binding to see if the input is activated
                for (const auto& [inputKey, prefab] : b.map)
                {
                    if (!inputKey.isActivated())
                    {
                        continue;
                    }

                    LOG_DEBUG("(ControlModule::InputSystem): InputKey is activated, adding prefab.");

                    // TODO: tell this entity to last only for 1 frame
                    q.each([&](flecs::entity e, const PossessedByPlayer& p)
                           { it.world().entity().is_a(prefab).add<Target>().set<Target>({e}); });
                }
            }
        );
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
                LOG_DEBUG("(ControlModule::MovementSystem): MoveIntent");
                auto& [acceleration] = t.entity.get_mut<Acceleration>();
                acceleration.y += i.accelerate.y * 1000.f;
                acceleration.x += i.accelerate.x * 1000.f;

                // Destroy the command entity
                cmd.destruct();
            }
        );
}

void ControllerDemoScene::CreateUISystem(const flecs::world& world)
{
    world.system<const PauseIntent>("PauseSystem")
        .each(
            [](const flecs::entity& cmd, const PauseIntent& p)
            {
                LOG_DEBUG("(ControlModule::PauseSystem): PauseIntent");
                auto& sceneManager = GameService::Get<SceneManager>();
                sceneManager.GetScene<ControllerDemoScene>().Pause();
                sceneManager.LoadScene<PauseScene>(SceneLoadMode::Additive);

                // Destroy the command entity
                cmd.destruct();
            }
        );
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
    );

    entity.add<PossessedByPlayer>().set<CommandQueue>({});
    entity.set<Acceleration>({});
    entity.set<Friction>({.friction = 10.f});
    entity.set<Gravity>({PhysicsConstants::NO_GRAVITY});
    entity.set<Velocity>({});
}

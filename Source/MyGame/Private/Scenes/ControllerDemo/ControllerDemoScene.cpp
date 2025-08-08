// Copyright (c) Eric Jeker 2025.

#include "Scenes/ControllerDemo/ControllerDemoScene.h"

#include "Core/Configuration.h"
#include "Core/Events/DeferredEvent.h"
#include "Core/GameInstance.h"
#include "Core/Logger.h"
#include "Core/Managers/GameService.h"
#include "Core/Managers/SceneManager.h"
#include "Core/Modules/Control/Components/Command.h"
#include "Core/Modules/Control/Components/CommandQueue.h"
#include "Core/Modules/Control/Components/PossessedByPlayer.h"
#include "Core/Modules/Control/Components/Target.h"
#include "Core/Modules/Control/Singletons/InputBindings.h"
#include "Core/Modules/Lifecycle/Components/LifetimeOneFrame.h"
#include "Core/Modules/Physics/Components/Acceleration.h"
#include "Core/Modules/Physics/Components/Friction.h"
#include "Core/Modules/Physics/Components/Gravity.h"
#include "Core/Modules/Physics/Components/Velocity.h"
#include "Core/Modules/Render/Prefabs/Rectangle.h"
#include "Core/Modules/UI/Components/KeyPressed.h"
#include "Core/Modules/UI/Prefabs/KeyPressedEvent.h"
#include "Scenes/ControllerDemo/Components/MoveIntent.h"
#include "Scenes/ControllerDemo/Components/PauseIntent.h"
#include "Scenes/Pause/PauseScene.h"


ControllerDemoScene::ControllerDemoScene(flecs::world& world)
    : Scene(world)
{
}

void ControllerDemoScene::Initialize()
{
    Scene::Initialize();
    LOG_DEBUG("(ControllerDemoScene:Initialize)");

    const auto& world = GetWorld();

    // TODO: Asserts that necessary module are present in the world

    CreateInputBindings(world);
    CreateMovementSystem(world);
    CreateUISystem(world);
    CreatePlayerEntity(world);
}

void ControllerDemoScene::HandleEvent(const std::optional<sf::Event>& event)
{
    if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
    {
        // We still filter the scan code as to not populate the ECS with useless entities
        if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
        {
            GetWorld().entity().is_a<KeyPressedEvent>().set<KeyPressed>({
                .code = keyPressed->code,
                .scancode = keyPressed->scancode,
                .alt = keyPressed->alt,
                .control = keyPressed->control,
                .shift = keyPressed->shift,
            });
        }
    }
}

void ControllerDemoScene::CreateInputBindings(const flecs::world& world)
{
    auto moveLeft = world.prefab("Command::MoveLeft").add<Command>().set<MoveIntent>({{-1.f, 0.f}});
    auto moveRight = world.prefab("Command::MoveRight").add<Command>().set<MoveIntent>({{1.f, 0.f}});
    auto moveUp = world.prefab("Command::MoveUp").add<Command>().set<MoveIntent>({{0.f, -1.f}});
    auto moveDown = world.prefab("Command::MoveDown").add<Command>().set<MoveIntent>({{0.f, 1.f}});

    world.set<InputBindings>({{
        {InputKey::Keyboard(sf::Keyboard::Key::W), moveUp},
        {InputKey::Keyboard(sf::Keyboard::Key::A), moveLeft},
        {InputKey::Keyboard(sf::Keyboard::Key::S), moveDown},
        {InputKey::Keyboard(sf::Keyboard::Key::D), moveRight},
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
    // Query for KeyPressed and pause the Scene
    world.system<const KeyPressed>("ProcessKeyPressed")
        .kind(flecs::PostLoad)
        .each(
            [](const flecs::entity& e, const KeyPressed& k)
            {
                if (k.scancode == sf::Keyboard::Scancode::Escape)
                {
                    e.world().entity().add<LifetimeOneFrame>().add<Command>().add<PauseIntent>();
                }

                e.destruct();
            }
        )
        .child_of(GetRootEntity());

    world.system<const PauseIntent>("PauseSystem")
        .kind(flecs::PreUpdate)
        .each(
            [&](const flecs::entity& e, const PauseIntent& p)
            {
                e.world().entity().set<DeferredEvent>({[]
                                                       {
                                                           auto& sceneManager = GameService::Get<SceneManager>();
                                                           sceneManager.GetScene<ControllerDemoScene>().Pause();
                                                           sceneManager.LoadScene<PauseScene>(SceneLoadMode::Additive);
                                                       }});

                // Destroy the command entity
                e.destruct();
            }
        )
        .child_of(GetRootEntity());
}

void ControllerDemoScene::CreatePlayerEntity(const flecs::world& world)
{
    constexpr float CENTER_X = Configuration::WINDOW_SIZE.x / 2;
    constexpr float CENTER_Y = Configuration::WINDOW_SIZE.y / 2;

    Prefabs::Rectangle::Create(
        world,
        {
            .size = {100.f, 20.f},
            .color = sf::Color::Red,
            .origin = {0.5f, 0.5f},
            .position = {CENTER_X, CENTER_Y},
            .zOrder = 0.f,
        }
    )
        .child_of(GetRootEntity())
        .add<PossessedByPlayer>()
        .set<CommandQueue>({})
        .set<Acceleration>({})
        .set<Friction>({.friction = 10.f})
        .set<Gravity>({PhysicsConstants::NO_GRAVITY})
        .set<Velocity>({});
}

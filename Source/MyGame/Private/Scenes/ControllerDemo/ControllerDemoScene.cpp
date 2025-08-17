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
#include "Core/Modules/Lifetime/Components/Lifetime.h"
#include "Core/Modules/Lifetime/Components/LifetimeOneFrame.h"
#include "Core/Modules/Particles/Components/ParticleEmitter.h"
#include "Core/Modules/Particles/Prefabs/Particle.h"
#include "Core/Modules/Physics/Components/Acceleration.h"
#include "Core/Modules/Physics/Components/Friction.h"
#include "Core/Modules/Physics/Components/Gravity.h"
#include "Core/Modules/Physics/Components/Velocity.h"
#include "Core/Modules/Render/Components/Transform.h"
#include "Core/Modules/Render/Components/ZOrder.h"
#include "Core/Modules/Render/Prefabs/Rectangle.h"
#include "Core/Modules/UI/Components/KeyPressed.h"
#include "Core/Modules/UI/Prefabs/KeyPressedEvent.h"
#include "Core/Utils/Random.h"
#include "Scenes/ControllerDemo/Components/MoveIntent.h"
#include "Scenes/ControllerDemo/Components/PauseIntent.h"
#include "Scenes/Pause/PauseScene.h"

#include <cmath>


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
    else if (const auto* mouseReleased = event->getIf<sf::Event::MouseButtonReleased>())
    {
        if (mouseReleased->button == sf::Mouse::Button::Left)
        {
            // Create a custom particle generator for explosive particles
            auto explosiveGenerator = [](flecs::world world, const Transform& emitterPos) -> flecs::entity
            {
                // Create particles that shoot outward in all directions
                const float angle = Random::UniformFloat(0.f, 360.f);
                const float speed = Random::UniformFloat(50.f, 200.f);
                const sf::Vector2f velocity =
                    {static_cast<float>(std::cos(angle * M_PI / 180.f) * speed),
                     static_cast<float>(std::sin(angle * M_PI / 180.f) * speed)};

                return world.entity()
                    .is_a<Prefabs::Particle>()
                    .set<Velocity>({velocity})
                    .set<ZOrder>({1000.f})
                    .set<Transform>({.position = emitterPos.position})
                    .set<Lifetime>({Random::UniformFloat(1.0f, 3.0f)});
            };

            GetWorld()
                .entity()
                .set<ParticleEmitter>({.ratePerSecond = 200.f, .maxParticles = 500, .generator = explosiveGenerator})
                .set<Lifetime>({0.2f})
                .set<Transform>({.position = sf::Vector2f{mouseReleased->position}});
        }
        else if (mouseReleased->button == sf::Mouse::Button::Right)
        {
            // Create a different type of particle generator for fountain effect
            auto fountainGenerator = [](flecs::world world, const Transform& emitterPos) -> flecs::entity
            {
                // Create particles that shoot upward like a fountain
                const float angle = Random::UniformFloat(-30.f, 30.f); // Narrow upward cone
                const float speed = Random::UniformFloat(100.f, 300.f);
                const sf::Vector2f velocity = {
                    static_cast<float>(std::sin(angle * M_PI / 180.f) * speed),
                    static_cast<float>(-std::cos(angle * M_PI / 180.f) * speed) // Negative Y for upward
                };

                return world.entity()
                    .is_a<Prefabs::Particle>()
                    .set<Velocity>({velocity})
                    .set<ZOrder>({1000.f})
                    .set<Transform>({.position = emitterPos.position})
                    .set<Lifetime>({Random::UniformFloat(2.0f, 4.0f)})
                    .set<Gravity>({{0.f, 200.f}});
            };

            GetWorld()
                .entity()
                .set<ParticleEmitter>({.ratePerSecond = 30000.f, .maxParticles = 30000, .generator = fountainGenerator})
                .set<Lifetime>({1.f})
                .set<Transform>({.position = sf::Vector2f{mouseReleased->position}});
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

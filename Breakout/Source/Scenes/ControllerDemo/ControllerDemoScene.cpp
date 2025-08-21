// Copyright (c) Eric Jeker 2025.

#include "Scenes/ControllerDemo/ControllerDemoScene.h"

#include "Components/LaunchBallIntent.h"
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
#include "Core/Modules/Lifetime/Components/LifetimeOneFrame.h"
#include "Core/Modules/Physics/Components/Acceleration.h"
#include "Core/Modules/Physics/Components/CircleCollider.h"
#include "Core/Modules/Physics/Components/Friction.h"
#include "Core/Modules/Physics/Components/Velocity.h"
#include "Core/Modules/Render/Prefabs/Circle.h"
#include "Core/Modules/Render/Prefabs/Rectangle.h"
#include "Core/Modules/UI/Components/KeyPressed.h"
#include "Core/Modules/UI/Prefabs/KeyPressedEvent.h"
#include "Core/Utils/Random.h"
#include "Scenes/ControllerDemo/Components/Ball.h"
#include "Scenes/ControllerDemo/Components/MoveIntent.h"
#include "Scenes/ControllerDemo/Components/Paddle.h"
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
    CreateLocalSystems(world);
    CreateUISystem(world);
    CreatePaddle(world);
    CreateBlocks(world);
    CreateBall(world);
}

void ControllerDemoScene::HandleEvent(const std::optional<sf::Event>& event)
{
    if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
    {
        // We still filter the scan code as to not populate the ECS with useless entities
        if (keyPressed->scancode == sf::Keyboard::Scancode::Escape || keyPressed->scancode == sf::Keyboard::Scancode::Space)
        {
            GetWorld()
                .entity()
                .is_a<KeyPressedEvent>()
                .set<KeyPressed>({
                    .code = keyPressed->code,
                    .scancode = keyPressed->scancode,
                    .alt = keyPressed->alt,
                    .control = keyPressed->control,
                    .shift = keyPressed->shift,
                })
                .child_of(GetRootEntity());
        }
    }
}

void ControllerDemoScene::CreateInputBindings(const flecs::world& world)
{
    auto moveLeft = world.prefab("Command::MoveLeft").add<Command>().set<MoveIntent>({{-1.f, 0.f}}).child_of(GetRootEntity());
    auto moveRight = world.prefab("Command::MoveRight").add<Command>().set<MoveIntent>({{1.f, 0.f}}).child_of(GetRootEntity());

    world.set<InputBindings>(
        {{{InputKey::Keyboard(sf::Keyboard::Key::A), moveLeft}, {InputKey::Keyboard(sf::Keyboard::Key::D), moveRight}}}
    );
}

void ControllerDemoScene::CreateLocalSystems(const flecs::world& world)
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

    // Launch the ball currently on the paddle
    world.system<const LaunchBallIntent>("LaunchBallSystem")
        .each(
            [](const flecs::entity& cmd, const LaunchBallIntent& l)
            {
                cmd.world().query<const Ball, const PossessedByPlayer>().each(
                    [](const flecs::entity ball, const Ball b, const PossessedByPlayer p)
                    {
                        ball.remove<PossessedByPlayer>();
                        ball.remove<Friction>();
                        ball.set<Velocity>({{0.f, -500.f}});
                    }
                );

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
                else if (k.scancode == sf::Keyboard::Scancode::Space)
                {
                    e.world().entity().add<LifetimeOneFrame>().add<Command>().add<LaunchBallIntent>();
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

void ControllerDemoScene::CreatePaddle(const flecs::world& world)
{
    constexpr float CENTER_X = Configuration::WINDOW_SIZE.x / 2;

    Prefabs::Rectangle::Create(
        world,
        {
            .size = {100.f, 20.f},
            .color = sf::Color::Red,
            .origin = {0.5f, 0.5f},
            .position = {CENTER_X, Configuration::WINDOW_SIZE.y - 100.f},
            .zOrder = 0.f,
        }
    )
        .child_of(GetRootEntity())
        .add<PossessedByPlayer>()
        .add<Paddle>()
        .set<CommandQueue>({})
        .set<Acceleration>({})
        .set<Friction>({.friction = 10.f})
        .set<Velocity>({});
}

void ControllerDemoScene::CreateBlocks(const flecs::world& world)
{
    constexpr float COLUMNS = 10;
    constexpr float ROWS = 6;
    constexpr float BLOCK_SPACING = 10.f;
    constexpr float MARGINS = 100.f;

    constexpr sf::Vector2f PLAYGROUND =
        {Configuration::WINDOW_SIZE.x - (COLUMNS - 1) * BLOCK_SPACING - MARGINS * 2,
         Configuration::WINDOW_SIZE.y / 3 - (ROWS - 1) * BLOCK_SPACING - MARGINS};
    constexpr float BLOCK_WIDTH = PLAYGROUND.x / COLUMNS;
    constexpr float BLOCK_HEIGHT = PLAYGROUND.y / ROWS;

    for (float x = 0; x < COLUMNS; ++x)
    {
        const float posX = MARGINS + x * (BLOCK_WIDTH + BLOCK_SPACING);

        for (float y = 0; y < ROWS; ++y)
        {
            const float posY = MARGINS + y * (BLOCK_HEIGHT + BLOCK_SPACING);

            Prefabs::Rectangle::Create(
                world,
                {.size = {BLOCK_WIDTH, BLOCK_HEIGHT}, .color = sf::Color::Blue, .origin = {0.f, 0.f}, .position = {posX, posY}}
            ).child_of(GetRootEntity());
        }
    }
}

void ControllerDemoScene::CreateBall(const flecs::world& world)
{
    constexpr float CENTER_X = Configuration::WINDOW_SIZE.x / 2;
    constexpr float RADIUS = 20.f;

    Prefabs::Circle::Create(
        world,
        {
            .radius = RADIUS,
            .color = sf::Color::Red,
            .origin = {0.5f, 0.5f},
            .position = {CENTER_X, Configuration::WINDOW_SIZE.y - 120.f},
            .zOrder = 0.f,
        }
    )
        .child_of(GetRootEntity())
        .add<PossessedByPlayer>()
        .add<Ball>()
        .set<CommandQueue>({})
        .set<Acceleration>({})
        .set<Friction>({.friction = 10.f})
        .set<Velocity>({})
        .set<CircleCollider>({RADIUS});
}

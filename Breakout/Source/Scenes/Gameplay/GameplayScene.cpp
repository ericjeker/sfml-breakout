// Copyright (c) Eric Jeker 2025.

#include "Scenes/Gameplay/GameplayScene.h"

#include "Components/Ball.h"
#include "Components/GameOverIntent.h"
#include "Components/LaunchBallIntent.h"
#include "Components/MoveIntent.h"
#include "Components/NavigateToMainMenuIntent.h"
#include "Components/Paddle.h"
#include "Components/PauseGameIntent.h"
#include "Components/RestartGameIntent.h"
#include "Components/ResumeGameIntent.h"
#include "GameStates/MainMenu/MainMenuState.h"
#include "Prefabs/MoveLeftCommand.h"
#include "Prefabs/MoveRightCommand.h"
#include "Scenes/GameOver/GameOverScene.h"
#include "Scenes/Pause/PauseScene.h"

#include "Core/Configuration.h"
#include "Core/Events/DeferredEvent.h"
#include "Core/GameInstance.h"
#include "Core/Managers/GameService.h"
#include "Core/Managers/GameStateManager.h"
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
#include "Core/Modules/Render/Components/Transform.h"
#include "Core/Modules/Render/Prefabs/Circle.h"
#include "Core/Modules/Render/Prefabs/Rectangle.h"
#include "Core/Modules/UI/Components/KeyPressed.h"
#include "Core/Modules/UI/Prefabs/KeyPressedEvent.h"
#include "Core/Utils/Logger.h"

#include <SFML/Graphics.hpp>

#include <iostream>
#include <optional>


GameplayScene::GameplayScene(flecs::world& world)
    : Scene(world)
{
}

void GameplayScene::Initialize()
{
    Scene::Initialize();
    LOG_DEBUG("GameplayScene:Initialize");

    auto& world = GetWorld();

    // TODO: Asserts that necessary module are present in the world

    // --- Create entities ---
    CreateInputBindings(world);
    CreatePaddle(world);
    CreateBlocks(world);
    CreateBall(world);

    // --- Add local systems ---
    CreateLocalSystems(world);
    CreateUISystem(world);
}

void GameplayScene::HandleEvent(const std::optional<sf::Event>& event)
{
    if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
    {
        // We still filter the scan code as to not populate the ECS with useless entities
        if (keyPressed->scancode == sf::Keyboard::Scancode::Escape || keyPressed->scancode == sf::Keyboard::Scancode::Space)
        {
            // Add a KeyPressed event in the world that will be handled later during the update
            GetWorld()
                .entity()
                .is_a<Prefabs::KeyPressedEvent>()
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

void GameplayScene::CreateInputBindings(const flecs::world& world)
{
    // Create command prefabs
    auto moveLeft = world.prefab<Prefabs::MoveLeftCommand>().add<Command>().set<MoveIntent>({{-1.f, 0.f}}).child_of(GetRootEntity());
    auto moveRight = world.prefab<Prefabs::MoveRightCommand>().add<Command>().set<MoveIntent>({{1.f, 0.f}}).child_of(GetRootEntity());

    // Bind the left and right movement to A and D
    world.set<InputBindings>(
        {{{InputKey::Keyboard(sf::Keyboard::Key::A), moveLeft}, {InputKey::Keyboard(sf::Keyboard::Key::D), moveRight}}}
    );
}

void GameplayScene::CreateLocalSystems(const flecs::world& world)
{
    // Triggered by the Input Bindings and ControlSystem
    world.system<const MoveIntent, const Target>("PaddleMovementSystem")
        .kind(flecs::PreUpdate)
        .each([](const flecs::entity& cmd, const MoveIntent& i, const Target& t) {
            auto& [acceleration] = t.entity.get_mut<Acceleration>();
            acceleration.y += i.accelerate.y * 1000.f;
            acceleration.x += i.accelerate.x * 1000.f;

            // Destroy the command entity
            cmd.destruct();
        })
        .child_of(GetRootEntity());

    // Launch the ball currently on the paddle
    world.system<const LaunchBallIntent>("LaunchBallSystem")
        .kind(flecs::PreUpdate)
        .each([](const flecs::entity& cmd, const LaunchBallIntent& l) {
            cmd.world().query<const Ball, const PossessedByPlayer>().each(
                [](const flecs::entity ball, const Ball b, const PossessedByPlayer p) {
                    ball.remove<PossessedByPlayer>();
                    ball.remove<Friction>();
                    ball.set<Velocity>({{0.f, -1000.f}});
                }
            );

            // Destroy the command entity
            cmd.destruct();
        })
        .child_of(GetRootEntity());

    // Bounce the ball off the walls
    world.system<Transform, Velocity, const CircleCollider, const Ball>("ScreenBounce")
        .each(ProcessScreenBounce)
        .child_of(GetRootEntity());

    // Check if the ball is out of bounds -> GameOver or LoseOneLife
    world.system<Transform, const Ball>("OutOfBounds").each(ProcessOutOfBounds).child_of(GetRootEntity());
}

void GameplayScene::CreateUISystem(flecs::world& world)
{
    // Query for KeyPressed and pause the Scene
    world.system<const KeyPressed>("ProcessKeyPressed")
        .kind(flecs::PostLoad)
        .each([](const flecs::entity& e, const KeyPressed& k) {
            if (k.scancode == sf::Keyboard::Scancode::Escape)
            {
                e.world().entity().add<LifetimeOneFrame>().add<Command>().add<PauseGameIntent>();
            }
            else if (k.scancode == sf::Keyboard::Scancode::Space)
            {
                e.world().entity().add<LifetimeOneFrame>().add<Command>().add<LaunchBallIntent>();
            }

            e.destruct();
        })
        .child_of(GetRootEntity());

    // Navigate to the main menu state after quitting the game
    world.system<const NavigateToMainMenuIntent>()
        .kind(flecs::PreUpdate)
        .each([&](const flecs::entity& e, const NavigateToMainMenuIntent i) {
            // We defer the state change to the end of the frame
            e.world().entity().set<DeferredEvent>({.callback = [&] {
                GameService::Get<GameStateManager>().ChangeState(std::make_unique<MainMenuState>(world));
            }});

            e.destruct();
        })
        .child_of(GetRootEntity());

    // Game over, load the game over scene
    world.system<const GameOverIntent>("GameOverSystem")
        .kind(flecs::PreUpdate)
        .each([](const flecs::entity& e, const GameOverIntent& g) {
            e.world().entity().set<DeferredEvent>({[] {
                auto& sceneManager = GameService::Get<SceneManager>();
                sceneManager.LoadScene<GameOverScene>(SceneLoadMode::Additive);
            }});

            e.destruct();
        })
        .child_of(GetRootEntity());

    world.system<const RestartGameIntent>().kind(flecs::PreUpdate).each([&](const flecs::entity& e, const RestartGameIntent i) {
        // We defer the state change to the end of the frame
        e.world().entity().set<DeferredEvent>({.callback = [&] {
            auto& sceneManager = GameService::Get<SceneManager>();
            sceneManager.LoadScene<GameplayScene>(SceneLoadMode::Single);
        }});

        e.destruct();
    });

    // Resume the game, enable the entities again
    world.system<const ResumeGameIntent>("ResumeGameSystem")
        .kind(flecs::PreUpdate)
        .each([rootEntity = GetRootEntity()](const flecs::entity& e, const ResumeGameIntent& r) {
            e.world().entity().set<DeferredEvent>({[] {
                auto& sceneManager = GameService::Get<SceneManager>();
                sceneManager.UnloadScene<PauseScene>();
            }});

            e.world()
                .query_builder<>()
                .scope_open()
                .with<Ball>()
                .oper(flecs::Or)
                .with<Paddle>()
                .scope_close()
                .with(flecs::Disabled)
                .optional()
                .with(flecs::ChildOf, rootEntity)
                .each([](const flecs::entity& entity) { entity.enable(); });

            e.destruct();
        })
        .child_of(GetRootEntity());

    // Pause the game, disable the entities
    world.system<const PauseGameIntent>("PauseGameSystem")
        .kind(flecs::PreUpdate)
        .each([rootEntity = GetRootEntity()](const flecs::entity& e, const PauseGameIntent& p) {
            e.world().entity().set<DeferredEvent>({[] {
                // Add the pause scene
                auto& sceneManager = GameService::Get<SceneManager>();
                sceneManager.LoadScene<PauseScene>(SceneLoadMode::Additive);
            }});

            e.world()
                .query_builder<>()
                .scope_open()
                .with<Ball>()
                .oper(flecs::Or)
                .with<Paddle>()
                .scope_close()
                .with(flecs::ChildOf, rootEntity)
                .each([](const flecs::entity& entity) { entity.disable(); });

            // Destroy the command entity
            e.destruct();
        })
        .child_of(GetRootEntity());
}

void GameplayScene::CreatePaddle(const flecs::world& world)
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

void GameplayScene::CreateBlocks(const flecs::world& world)
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

    for (int x = 0; x < COLUMNS; ++x)
    {
        const float posX = MARGINS + x * (BLOCK_WIDTH + BLOCK_SPACING);

        for (int y = 0; y < ROWS; ++y)
        {
            const float posY = MARGINS + y * (BLOCK_HEIGHT + BLOCK_SPACING);

            Prefabs::Rectangle::Create(
                world,
                {.size = {BLOCK_WIDTH, BLOCK_HEIGHT}, .color = sf::Color::Blue, .origin = {0.f, 0.f}, .position = {posX, posY}}
            ).child_of(GetRootEntity());
        }
    }
}

void GameplayScene::CreateBall(const flecs::world& world)
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

void GameplayScene::ProcessScreenBounce(Transform& t, Velocity& v, const CircleCollider& c, const Ball& b)
{
    const float radius = c.radius;

    if (t.position.x - radius < 0.f)
    {
        v.velocity.x *= -1.f;
        t.position.x = radius;
    }
    else if (t.position.x + radius > Configuration::WINDOW_SIZE.x)
    {
        v.velocity.x *= -1.f;
        t.position.x = Configuration::WINDOW_SIZE.x - radius;
    }

    if (t.position.y - radius < 0.f)
    {
        v.velocity.y *= -1.f;
        t.position.y = radius;
    }
}

void GameplayScene::ProcessOutOfBounds(const flecs::entity ball, const Transform& t, const Ball& b)
{
    if (t.position.y > Configuration::WINDOW_SIZE.y)
    {
        LOG_DEBUG("GameplayScene::ProcessOutOfBounds -> Destroy ball");
        ball.destruct();

        LOG_DEBUG("GameplayScene::ProcessOutOfBounds -> Add GameOverIntent");
        ball.world().entity().add<LifetimeOneFrame>().add<Command>().add<GameOverIntent>();
    }
}

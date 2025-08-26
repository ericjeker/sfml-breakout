// Copyright (c) Eric Jeker 2025.

#include "Scenes/Gameplay/GameplayScene.h"

#include "Components/AttachedToPaddle.h"
#include "Components/Ball.h"
#include "Components/Block.h"
#include "Components/GameOverIntent.h"
#include "Components/GameWonIntent.h"
#include "Components/Health.h"
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
#include "Scenes/GameWon/GameWonScene.h"
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
#include "Core/Modules/Lifetime/Components/Lifetime.h"
#include "Core/Modules/Lifetime/Components/LifetimeOneFrame.h"
#include "Core/Modules/Physics/Components/Acceleration.h"
#include "Core/Modules/Physics/Components/ColliderShape.h"
#include "Core/Modules/Physics/Components/Friction.h"
#include "Core/Modules/Physics/Components/Velocity.h"
#include "Core/Modules/Render/Components/Origin.h"
#include "Core/Modules/Render/Components/Radius.h"
#include "Core/Modules/Render/Components/Size.h"
#include "Core/Modules/Render/Components/Transform.h"
#include "Core/Modules/Render/Prefabs/Circle.h"
#include "Core/Modules/Render/Prefabs/Rectangle.h"
#include "Core/Modules/UI/Components/KeyPressed.h"
#include "Core/Modules/UI/Prefabs/KeyPressedEvent.h"
#include "Core/Tags/ScenePaused.h"
#include "Core/Utils/Collision.h"
#include "Core/Utils/Logger.h"
#include "Core/Utils/Vector.h"

#include <SFML/Graphics.hpp>

#include <iostream>
#include <numbers>
#include <optional>
#include <string>
#include <tracy/Tracy.hpp>


GameplayScene::GameplayScene(flecs::world& world)
    : Scene(world)
{
}

void GameplayScene::Initialize()
{
    LOG_DEBUG("GameplayScene:Initialize");
    Scene::Initialize();

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
    // Only process when active
    if (GetRootEntity().has<ScenePaused>())
    {
        return;
    }

    if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
    {
        // We still filter the scan code as to not populate the ECS with useless entities
        if (keyPressed->scancode == sf::Keyboard::Scancode::Escape || keyPressed->scancode == sf::Keyboard::Scancode::Space)
        {
            LOG_DEBUG("GameplayScene::HandleEvent: KeyPressed");

            // Add a KeyPressed event to this scene, that will later be handled during the update
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
    // --- Paddle Movement System ---

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


    // --- Launching the Ball ---

    // Launch the ball currently on the paddle
    world.system<const LaunchBallIntent>("LaunchBallSystem")
        .kind(flecs::PreUpdate)
        .each([](const flecs::entity& cmd, const LaunchBallIntent& l) {
            // Get the ball entity
            cmd.world().query<const Ball, const AttachedToPaddle>().each(
                [](const flecs::entity ball, const Ball b, const AttachedToPaddle ap) {
                    ball.remove<AttachedToPaddle>();
                    ball.set<Velocity>({{0.f, -500.f}});
                    ball.set<ColliderShape>({Shape::Circle});
                }
            );

            // Destroy the command entity
            cmd.destruct();
        })
        .child_of(GetRootEntity());

    // When the ball is attached to the paddle, move it with the paddle
    world.system<Transform, const AttachedToPaddle, const Ball>("ApplyPaddlePositionToBall")
        .each(ApplyPaddlePositionToBall)
        .child_of(GetRootEntity());


    // --- Physics, Collision, Constraints ---

    // Bounce the ball off the walls
    world.system<Transform, Velocity, const ColliderShape, const Radius, const Ball>("ScreenBounce")
        .each(ProcessScreenBounce)
        .child_of(GetRootEntity());

    // Detect a collision with a collider (blocks or paddle)
    world.system<const Transform, const Size, const Origin, const ColliderShape>("CollisionDetectionSystem")
        .each(ProcessCollisionDetection)
        .child_of(GetRootEntity());

    // Constrain paddle to the screen
    world.system<Transform, const Size, const Paddle>("ConstrainPaddleToScreen")
        .each(ConstrainPaddleToScreen)
        .child_of(GetRootEntity());


    // --- Game Over / Game Won ---

    // Check if the ball is out of bounds -> GameOver or LoseOneLife
    // TODO: instead, check if there are no more ball on the screen = GAME OVER or LOSE ONE LIFE
    world.system<Transform, const Ball>("OutOfBounds").each(ProcessOutOfBounds).child_of(GetRootEntity());

    // Check if there are no more blocks on the screen = WIN
    world.system("CheckIfAllBlocksDestroyed")
        .run([&](const flecs::iter& it) {
            if (it.world().count<const Block>() == 0)
            {
                LOG_DEBUG("GameplayScene::CheckIfAllBlocksDestroyed -> Add GameWonIntent");
                it.world().entity().add<LifetimeOneFrame>().add<Command>().add<GameWonIntent>().child_of(GetRootEntity());
            }
        })
        .child_of(GetRootEntity());
}

void GameplayScene::CreateUISystem(flecs::world& world)
{
    // Query for KeyPressed and pause the Scene
    world.system<const KeyPressed>("ProcessKeyPressed")
        .kind(flecs::PostLoad)
        // Make sure we process only the KeyPressed events from this scene
        .with(flecs::ChildOf, GetRootEntity())
        .each([&](const flecs::entity& e, const KeyPressed& k) {
            if (k.scancode == sf::Keyboard::Scancode::Escape)
            {
                LOG_DEBUG("GameplayScene::ProcessKeyPressed: Escape -> Add PauseGameIntent");
                e.world().entity().add<LifetimeOneFrame>().add<Command>().add<PauseGameIntent>().child_of(GetRootEntity());
            }
            else if (k.scancode == sf::Keyboard::Scancode::Space)
            {
                e.world().entity().add<LifetimeOneFrame>().add<Command>().add<LaunchBallIntent>().child_of(GetRootEntity());
            }

            e.destruct();
        })
        .child_of(GetRootEntity());

    // Navigate to the main menu state after quitting the game
    world.system<const NavigateToMainMenuIntent>("ProcessNavigateToMainMenuIntent")
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
    world.system<const GameOverIntent>("ProcessGameOverIntent")
        .kind(flecs::PreUpdate)
        .each([](const flecs::entity& e, const GameOverIntent& g) {
            e.world().entity().set<DeferredEvent>({[] {
                auto& sceneManager = GameService::Get<SceneManager>();
                sceneManager.LoadScene<GameOverScene>(SceneLoadMode::Additive);
            }});

            e.destruct();
        })
        .child_of(GetRootEntity());

    // Game won, load the game over scene
    world.system<const GameWonIntent>("ProcessGameWonIntent")
        .kind(flecs::PreUpdate)
        .each([&](const flecs::entity& e, const GameWonIntent& g) {
            e.world().entity().set<DeferredEvent>({[] {
                auto& sceneManager = GameService::Get<SceneManager>();
                sceneManager.LoadScene<GameWonScene>(SceneLoadMode::Additive);
            }});

            // Get CheckIfAllBlocksDestroyed and disable it
            if (const auto system = GetRootEntity().lookup("CheckIfAllBlocksDestroyed"); system.is_valid())
            {
                LOG_DEBUG("GameplayScene::ProcessGameWonIntent -> Disable CheckIfAllBlocksDestroyed");
                system.disable();
            }

            PauseGame(e.world());

            e.destruct();
        })
        .child_of(GetRootEntity());

    world.system<const RestartGameIntent>("ProcessRestartGameIntent")
        .kind(flecs::PreUpdate)
        .each([&](const flecs::entity& e, const RestartGameIntent i) {
            // We defer the state change to the end of the frame
            e.world().entity().set<DeferredEvent>({.callback = [&] {
                auto& sceneManager = GameService::Get<SceneManager>();
                sceneManager.LoadScene<GameplayScene>(SceneLoadMode::Single);
            }});

            e.destruct();
        });

    // Resume the game, enable the entities again
    world.system<const ResumeGameIntent>("ProcessResumeGameIntent")
        .kind(flecs::PreUpdate)
        .each([rootEntity = GetRootEntity()](const flecs::entity& e, const ResumeGameIntent& r) {
            LOG_DEBUG("GameplayScene::ResumeGameSystem");

            e.world().entity().set<DeferredEvent>({[] {
                LOG_DEBUG("GameplayScene::ResumeGameSystem -> Unload Pause Scene");
                auto& sceneManager = GameService::Get<SceneManager>();
                sceneManager.UnloadScene<PauseScene>();
            }});

            rootEntity.remove<ScenePaused>();

            // Query for the Paddle and Ball to enable them
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

            LOG_DEBUG("GameplayScene::ResumeGameSystem -> Destroy ResumeGameIntent");
            e.destruct();
        })
        .child_of(GetRootEntity());

    // Pause the game, disable the entities
    world.system<const PauseGameIntent>("ProcessPauseGameIntent")
        .kind(flecs::PreUpdate)
        .each([this](const flecs::entity& e, const PauseGameIntent& p) { ProcessPauseGameIntent(e, p); })
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
        .set<Acceleration>({})
        .set<Friction>({.friction = 10.f})
        .set<Velocity>({})
        .set<ColliderShape>({Shape::Rectangle});
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

            Prefabs::Rectangle::
                Create(world, {.size = {BLOCK_WIDTH, BLOCK_HEIGHT}, .color = sf::Color::Blue, .origin = {0.f, 0.f}, .position = {posX, posY}})
                    .add<Block>()
                    .set<Health>({1})
                    .set<ColliderShape>({Shape::Rectangle})
                    .child_of(GetRootEntity());
        }
    }
}

void GameplayScene::CreateBall(const flecs::world& world)
{
    constexpr float CENTER_X = Configuration::WINDOW_SIZE.x / 2;
    constexpr float RADIUS = 10.f;

    Prefabs::Circle::Create(
        world,
        {
            .radius = RADIUS,
            .color = sf::Color::Red,
            .origin = {0.5f, 0.5f},
            .position = {CENTER_X, Configuration::WINDOW_SIZE.y - 125.f},
            .zOrder = 0.f,
        }
    )
        .child_of(GetRootEntity())
        // TODO: maybe just AttachedTo and add a socket information
        .add<AttachedToPaddle>()
        .add<Ball>()
        .set<Velocity>({});
}

void GameplayScene::ProcessScreenBounce(Transform& t, Velocity& v, const ColliderShape& c, const Radius& r, const Ball& b)
{
    const float radius = r.radius;

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

void GameplayScene::ProcessCollisionDetection(
    const flecs::entity& blockEntity,
    const Transform& transform,
    const Size& size,
    const Origin& origin,
    const ColliderShape& c
)
{
    ZoneScopedN("GameplayScene::CollisionDetectionSystem");

    // Calculate actual collision bounds using transform, size, and origin
    const sf::Vector2f actualOrigin = size.size.componentWiseMul(origin.origin);
    const sf::FloatRect colliderBounds(
        {
            transform.position.x - actualOrigin.x,
            transform.position.y - actualOrigin.y,
        },
        {size.size.x, size.size.y}
    );

    // Check if this is a paddle collision
    const bool isPaddle = blockEntity.has<Paddle>();

    // We query for all the balls
    blockEntity.world().query<Transform, Velocity, const Radius, const ColliderShape, const Ball>().each(
        [blockEntity, colliderBounds, isPaddle](
            const flecs::entity& ballEntity,
            Transform& ballTransform,
            Velocity& ballVelocity,
            const Radius& ballRadius,
            const ColliderShape& c,
            const Ball& b
        ) {
            const CollisionInfo
                collisionInfo = Collision::CheckAABBCircleCollision(colliderBounds, ballTransform.position, ballRadius.radius);

            if (!collisionInfo.hasCollision)
            {
                return;
            }

            // Add the debug information
            ballEntity.world().entity().set<Lifetime>({.5f}).set<CollisionInfo>(collisionInfo);

            // Move circle out of penetration
            ballTransform.position = ballTransform.position + collisionInfo.normal * collisionInfo.penetrationDepth;

            if (isPaddle)
            {
                // Calculate where on the paddle the ball hit relative to the paddle center
                const float paddleHalfWidth = colliderBounds.size.x * 0.5f;
                const float paddleCenter = colliderBounds.position.x + paddleHalfWidth;
                const float hitOffset = collisionInfo.contactPoint.x - paddleCenter;

                // Normalize to range [-1, 1]
                const float normalizedHit = std::clamp(hitOffset / paddleHalfWidth, -1.0f, 1.0f);

                // Create deflection based on hit position
                constexpr float DEFLECTION_STRENGTH = 0.5f;

                // Modify the normal to add horizontal deflection
                sf::Vector2f modifiedNormal = collisionInfo.normal;
                modifiedNormal.x += normalizedHit * DEFLECTION_STRENGTH;
                modifiedNormal = modifiedNormal.normalized();

                // Reflect with modified normal
                const sf::Vector2f reflected = ballVelocity.velocity -
                                               2.f * Vector::Dot(ballVelocity.velocity, modifiedNormal) * modifiedNormal;
                ballVelocity.velocity = reflected;
            }
            else
            {
                // Reflect velocity along collision normal
                const sf::Vector2f reflected = ballVelocity.velocity -
                                               2.f * Vector::Dot(ballVelocity.velocity, collisionInfo.normal) *
                                                   collisionInfo.normal;
                ballVelocity.velocity = reflected;

                // Remove one health from the block
                auto [health] = blockEntity.get_mut<Health>();
                health -= 1;

                if (health <= 0)
                {
                    // Destroy the block
                    blockEntity.destruct();
                }
            }
        }
    );
}

void GameplayScene::ConstrainPaddleToScreen(Transform& t, const Size& s, const Paddle& p)
{
    const float halfSize = s.size.x / 2;

    if (t.position.x - halfSize < 0.f)
    {
        t.position.x = halfSize;
    }
    else if (t.position.x + halfSize > Configuration::WINDOW_SIZE.x)
    {
        t.position.x = Configuration::WINDOW_SIZE.x - halfSize;
    }
}

void GameplayScene::ProcessPauseGameIntent(const flecs::entity& e, const PauseGameIntent& p)
{
    LOG_DEBUG("GameplayScene::PauseGameSystem");

    e.world().entity().set<DeferredEvent>({[] {
        LOG_DEBUG("GameplayScene::PauseGameSystem -> Load Pause Scene");
        auto& sceneManager = GameService::Get<SceneManager>();
        sceneManager.LoadScene<PauseScene>(SceneLoadMode::Additive);
    }});

    PauseGame(e.world());

    // Destroy the command entity
    LOG_DEBUG("GameplayScene::PauseGameSystem -> Destroy PauseGameIntent");
    e.destruct();
}

void GameplayScene::PauseGame(const flecs::world& w)
{
    GetRootEntity().add<ScenePaused>();

    // Query for the Paddle and Ball to disable them
    w.query_builder<>()
        .scope_open()
        .with<Ball>()
        .oper(flecs::Or)
        .with<Paddle>()
        .scope_close()
        .with(flecs::ChildOf, GetRootEntity())
        .each([](const flecs::entity& entity) { entity.disable(); });
}

void GameplayScene::ApplyPaddlePositionToBall(const flecs::entity& e, Transform& t, const AttachedToPaddle& ap, const Ball& b)
{
    // Get the paddle
    e.world().query<const Transform, const Paddle>().each(
        [&ballTransform = t](const Transform& paddleTransform, const Paddle& p) {
            ballTransform.position.x = paddleTransform.position.x;
        }
    );
}

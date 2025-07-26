// Copyright (c) Eric Jeker 2025.

#include "BouncingBallScene.h"

#ifdef TRACY_ENABLE
#include <tracy/Tracy.hpp>
#endif

#include "Components/GravitySettings.h"
#include "Configuration.h"
#include "PhysicsConstants.h"
#include "Themes/Nord.h"

#include <random>

constexpr int BALL_COUNT = 200;


void BouncingBallScene::Initialize()
{
    auto backgroundDrawable = std::make_unique<sf::RectangleShape>();
    backgroundDrawable->setSize(sf::Vector2f{Configuration::WINDOW_SIZE});
    backgroundDrawable->setFillColor(NordTheme::Frost1);

    const auto ecsWorld = GetWorld();

    ecsWorld.entity()
        .set<BackgroundRenderable>({std::move(backgroundDrawable)})
        .set<Transform>({.position = {0.f, 0.f}, .scale = {1.f, 1.f}, .rotation = 0.f});

    ecsWorld.set<GravitySettings>(
        {.gravity = PhysicsConstants::NO_GRAVITY, .pixelsPerCentimeter = PhysicsConstants::PIXELS_PER_CENTIMETER}
    );


    // --- Create LOTTA balls ---
    CreateBalls(BALL_COUNT);

    // --- Add systems ---
    ecsWorld.system<Transform, Velocity, RigidBody>().each(ProcessPhysics);
    ecsWorld.system<Transform, Velocity>().each(ProcessScreenBounce);
}

void BouncingBallScene::HandleEvent(const std::optional<sf::Event>& event, sf::RenderWindow& window)
{
    if (!IsLoaded() || IsPaused())
    {
        return;
    }

    // --- Handle the mouse events ---
    // if (event->is<sf::Event::MouseButtonPressed>())
    // {
    //     // Create a ball and glue it to the mouse cursor
    //     _throwStartPos = sf::Mouse::getPosition(window);
    //     auto entity = CreateBallEntity(sf::Vector2f(_throwStartPos), sf::Vector2f{0.f, 0.f});
    //     entity->AddComponent<PlayerPossessedComponent>({});
    //     AddEntity(std::move(entity));
    //     EmitBallCountChangedEvent();
    // }
    // else if (const auto* mouseMoved = event->getIf<sf::Event::MouseMoved>())
    // {
    //     // Update the mouse cursor position
    //     const auto position = mouseMoved->position;
    //     if (auto* player = GetEntityWithComponent<PlayerPossessedComponent>(); player != nullptr)
    //     {
    //         auto* transform = player->GetComponent<TransformComponent>();
    //         transform->position = sf::Vector2f(position);
    //     }
    // }
    // else if (event->is<sf::Event::MouseButtonReleased>())
    // {
    //     // When we release the mouse button, we calculate the distance with the initial button press, the resulting
    //     // delta is the velocity given to the ball
    //     const auto position = sf::Mouse::getPosition(window);
    //     const auto delta = position - _throwStartPos;
    //     LOG_DEBUG(
    //         std::format(
    //             "(BouncingBallScene:HandleEvent): Mouse button release with force {}",
    //             static_cast<sf::Vector2f>(delta).length()
    //         )
    //     );
    //     if (auto* player = GetEntityWithComponent<PlayerPossessedComponent>(); player != nullptr)
    //     {
    //         player->RemoveComponent<PlayerPossessedComponent>();
    //         player->AddComponent<PhysicsComponent>(
    //             {.velocity = sf::Vector2f(delta.x, delta.y) * PhysicsConstants::PIXELS_PER_CENTIMETER,
    //              .damping = PhysicsConstants::DEFAULT_DAMPING}
    //         );
    //
    //         if (auto* transform = player->GetComponent<TransformComponent>())
    //         {
    //             transform->position = sf::Vector2f(position);
    //         }
    //     }
    // }
    // // --- Handle the keyboard events ---
    // else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
    // {
    //     if (keyPressed->scancode == sf::Keyboard::Scan::G)
    //     {
    //         ToggleGravity();
    //     }
    //     else if (keyPressed->scancode == sf::Keyboard::Scan::Backspace || keyPressed->scancode == sf::Keyboard::Scan::R)
    //     {
    //         ResetSimulation();
    //     }
    //     else if (keyPressed->scancode == sf::Keyboard::Scan::Up)
    //     {
    //         IncreaseGravity();
    //     }
    //     else if (keyPressed->scancode == sf::Keyboard::Scan::Down)
    //     {
    //         DecreaseGravity();
    //     }
    //     else if (keyPressed->scancode == sf::Keyboard::Scan::NumpadPlus)
    //     {
    //         int numberOfBalls = 1;
    //         if (isKeyPressed(sf::Keyboard::Scan::LShift) || isKeyPressed(sf::Keyboard::Scan::RShift))
    //         {
    //             numberOfBalls *= 10;
    //         }
    //
    //         CreateBalls(numberOfBalls);
    //         EmitBallCountChangedEvent();
    //     }
    // }
}

void BouncingBallScene::Render(sf::RenderWindow& window)
{
    if (!IsLoaded())
        return;

    const auto ecsWorld = GetWorld();

    // Render background
    ecsWorld.each(
        [&](const Transform& t, const BackgroundRenderable& bg)
        {
            bg.shape->setPosition(t.position);
            window.draw(*bg.shape);
        }
    );

    // Render balls
    ecsWorld.each(
        [&](const Transform& t, const BallRenderable& ball)
        {
            ball.shape->setPosition(t.position);
            window.draw(*ball.shape);
        }
    );
}

void BouncingBallScene::CreateBalls(const int count)
{
    ZoneScopedN("BouncingBallScene::CreateBalls");

    // Create balls
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> distX(0.f, Configuration::WINDOW_SIZE.x);
    std::uniform_real_distribution<float> distY(0.f, Configuration::WINDOW_SIZE.y);
    std::uniform_real_distribution<float> velX(-50.f, 50.f);
    std::uniform_real_distribution<float> velY(-50.f, 50.f);

    for (int i = 0; i < count; ++i)
    {
        constexpr float radius = 5.f;
        const sf::Vector2f position = {distX(gen), distY(gen)};
        const sf::Vector2f velocity =
            {velX(gen) * PhysicsConstants::PIXELS_PER_CENTIMETER, velY(gen) * PhysicsConstants::PIXELS_PER_CENTIMETER};

        // Create a ball
        auto ballShape = std::make_unique<sf::CircleShape>(radius);
        ballShape->setFillColor(NordTheme::Aurora1);
        ballShape->setOrigin({radius, radius});

        GetWorld()
            .entity()
            .set<Transform>({.position = position})
            .set<Velocity>({velocity})
            .set<RigidBody>({.damping = PhysicsConstants::NO_DAMPING})
            .set<BallRenderable>({std::move(ballShape)});
    }
}

void BouncingBallScene::ProcessScreenBounce(const flecs::iter& it, size_t, Transform& t, Velocity& v)
{
    if (t.position.x < 0.f)
    {
        v.velocity.x *= -1.f;
        t.position.x = 0.f;
    }
    else if (t.position.x > Configuration::WINDOW_SIZE.x)
    {
        v.velocity.x *= -1.f;
        t.position.x = Configuration::WINDOW_SIZE.x;
    }

    if (t.position.y < 0.f)
    {
        v.velocity.y *= -1.f;
        t.position.y = 0.f;
    }
    else if (t.position.y > Configuration::WINDOW_SIZE.y)
    {
        v.velocity.y *= -1.f;
        t.position.y = Configuration::WINDOW_SIZE.y;
    }
}

void BouncingBallScene::ProcessPhysics(const flecs::iter& it, size_t, Transform& t, Velocity& v, const RigidBody& p)
{
    auto gravitySettings = it.world().get<GravitySettings>();

    // Physics system
    if (gravitySettings.enabled)
    {
        v.velocity.x += gravitySettings.gravity.x * gravitySettings.pixelsPerCentimeter * it.delta_time();
        v.velocity.y += gravitySettings.gravity.y * gravitySettings.pixelsPerCentimeter * it.delta_time();
    }

    v.velocity *= p.damping;
    t.position += v.velocity * it.delta_time();
}

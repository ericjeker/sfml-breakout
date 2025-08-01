// Copyright (c) Eric Jeker 2025.

#include "BouncingBallScene.h"

#ifdef TRACY_ENABLE
#include <tracy/Tracy.hpp>
#endif

#include "Configuration.h"
#include "Modules/BouncingBalls/BouncingBallsModule.h"
#include "Modules/BouncingBalls/Prefabs/Ball.h"
#include "Modules/Physics/Components/GravitySettings.h"
#include "Modules/Physics/PhysicsModule.h"
#include "Modules/Render/Prefabs/Rectangle.h"
#include "Modules/Render/RenderModule.h"
#include "PhysicsConstants.h"
#include "Themes/Nord.h"

#include <random>


void BouncingBallScene::Initialize()
{
    // --- Get the world and import the modules ---
    auto world = GetWorld();

    world.import <Modules::PhysicsModule>();
    world.import <Modules::BouncingBallsModule>();
    world.import <Modules::RenderModule>();

    world.set<GravitySettings>(
        {.gravity = PhysicsConstants::NO_GRAVITY, .pixelsPerCentimeter = PhysicsConstants::PIXELS_PER_CENTIMETER}
    );

    Prefabs::Rectangle::Create(
        world,
        {
            .size = sf::Vector2f{Configuration::WINDOW_SIZE},
            .color = NordTheme::PolarNight4,
            .position = {0.f, 0.f},
        }
    );

    // --- Create LOTTA balls ---
    CreateBalls(world);
}

void BouncingBallScene::HandleEvent(const std::optional<sf::Event>& event)
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

void BouncingBallScene::CreateBalls(const flecs::world& world)
{
    ZoneScopedN("BouncingBallScene::CreateBalls");

    constexpr int BALL_COUNT = 100;

    // Create balls
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> distX(0.f, Configuration::WINDOW_SIZE.x);
    std::uniform_real_distribution<float> distY(0.f, Configuration::WINDOW_SIZE.y);
    std::uniform_real_distribution velX(-50.f, 50.f);
    std::uniform_real_distribution velY(-50.f, 50.f);

    for (int i = 0; i < BALL_COUNT; ++i)
    {
        constexpr float RADIUS = 10.f;

        Prefabs::Ball::Create(
            world,
            {
                .radius = RADIUS,
                .position = {distX(gen), distY(gen)},
                .velocity = {velX(gen) * PhysicsConstants::PIXELS_PER_CENTIMETER, velY(gen) * PhysicsConstants::PIXELS_PER_CENTIMETER},
                .color = NordTheme::Aurora1,
                .friction = 0.f,
                .gravity = PhysicsConstants::NO_GRAVITY,
            }
        );
    }
}

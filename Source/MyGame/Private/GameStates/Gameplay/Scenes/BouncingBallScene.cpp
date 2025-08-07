// Copyright (c) Eric Jeker 2025.

#include "BouncingBallScene.h"

#ifdef TRACY_ENABLE
#include <tracy/Tracy.hpp>
#endif

#include "Core/Configuration.h"
#include "Core/Logger.h"
#include "Core/Modules/Physics/Components/GravitySettings.h"
#include "Core/Modules/Render/Prefabs/Rectangle.h"
#include "Core/Modules/Render/Prefabs/Sprite.h"
#include "Core/PhysicsConstants.h"
#include "Core/Themes/Nord.h"
#include "Modules/BouncingBalls/BouncingBallsModule.h"
#include "Modules/BouncingBalls/Prefabs/Ball.h"

#include <random>


BouncingBallScene::BouncingBallScene(flecs::world& world)
    : Scene(world)
{
}

void BouncingBallScene::Initialize()
{
    Scene::Initialize();

    // --- Get the world and import the modules ---
    auto& world = GetWorld();

    // Clang Format is confused...
    // clang-format off
    world.import<Modules::BouncingBallsModule>().child_of(GetRootEntity());
    // clang-format on

    world.set<GravitySettings>(
        {.gravity = PhysicsConstants::NO_GRAVITY, .pixelsPerCentimeter = PhysicsConstants::PIXELS_PER_CENTIMETER}
    );

    float zOrder = 0.f;

    Prefabs::Rectangle::Create(
        world,
        {
            .size = sf::Vector2f{Configuration::WINDOW_SIZE},
            .color = NordTheme::PolarNight4,
            .position = {0.f, 0.f},
            .zOrder = zOrder++,
        }
    )
        .child_of(GetRootEntity());

    // --- Create LOTTA balls ---
    CreateBalls(world, zOrder);
}

void BouncingBallScene::CreateBalls(const flecs::world& world, float zOrder)
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
                .color = NordTheme::Aurora1,
                .velocity = {velX(gen) * PhysicsConstants::PIXELS_PER_CENTIMETER, velY(gen) * PhysicsConstants::PIXELS_PER_CENTIMETER},
                .zOrder = zOrder++,
                .gravity = PhysicsConstants::NO_GRAVITY,
                .friction = 0.f,
            }
        )
            .child_of(GetRootEntity());
    }
}

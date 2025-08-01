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
#include "Modules/Render/Prefabs/Sprite.h"
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

    int zOrder = 0;

    Prefabs::Rectangle::Create(
        world,
        {
            .size = sf::Vector2f{Configuration::WINDOW_SIZE},
            .color = NordTheme::PolarNight4,
            .position = {0.f, 0.f},
            .zOrder = zOrder++,
        }
    );

    // --- Create LOTTA balls ---
    CreateBalls(world, zOrder);
}

void BouncingBallScene::CreateBalls(const flecs::world& world, int zOrder)
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
                .zOrder = zOrder++,
            }
        );
    }
}

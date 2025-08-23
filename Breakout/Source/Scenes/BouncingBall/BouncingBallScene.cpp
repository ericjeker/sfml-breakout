// Copyright (c) Eric Jeker 2025.

#include "BouncingBallScene.h"

#include "Scenes/BouncingBall/Prefabs/Ball.h"

#include "Core/Configuration.h"
#include "Core/Modules/Physics/Components/Velocity.h"
#include "Core/Modules/Render/Components/Transform.h"
#include "Core/Modules/Render/Prefabs/Rectangle.h"
#include "Core/Modules/Render/Prefabs/Sprite.h"
#include "Core/PhysicsConstants.h"
#include "Core/Themes/Nord.h"

#include <random>
#include <tracy/Tracy.hpp>


BouncingBallScene::BouncingBallScene(flecs::world& world)
    : Scene(world)
{
}

void BouncingBallScene::Initialize()
{
    Scene::Initialize();

    // --- Get the world and import the modules ---
    const auto& world = GetWorld();

    // --- Declare local systems ---
    world.system<Transform, Velocity, const Radius, const ColliderShape>()
        .kind(flecs::OnStore)
        .each(ProcessScreenBounce)
        .child_of(GetRootEntity());

    // --- Create the entities ---
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
                .color = NordTheme::Aurora1,
                .position = {distX(gen), distY(gen)},
                .zOrder = zOrder++,
                .velocity = {velX(gen) * PhysicsConstants::PIXELS_PER_CENTIMETER, velY(gen) * PhysicsConstants::PIXELS_PER_CENTIMETER},
                .gravity = PhysicsConstants::NO_GRAVITY,
                .friction = 0.f,
            }
        )
            .child_of(GetRootEntity());
    }
}


void BouncingBallScene::ProcessScreenBounce(Transform& t, Velocity& v, const Radius& r, const ColliderShape& c)
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
    else if (t.position.y + radius > Configuration::WINDOW_SIZE.y)
    {
        v.velocity.y *= -1.f;
        t.position.y = Configuration::WINDOW_SIZE.y - radius;
    }
}

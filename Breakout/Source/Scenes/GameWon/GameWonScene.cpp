// Copyright (c) Eric Jeker 2025.

#include "GameWonScene.h"

#include "Modules/Breakout/Components/TransitionGameStateIntent.h"
#include "Modules/Breakout/Singletons/GameStateGameWon.h"
#include "Scenes/Gameplay/Components/Intents/RestartGameIntent.h"

#include "Core/Configuration.h"
#include "Core/Modules/Input/Components/Command.h"
#include "Core/Modules/Lifetime/Components/LifetimeOneFrame.h"
#include "Core/Modules/Render/Factories/Rectangle.h"
#include "Core/Modules/UI/Components/KeyPressed.h"
#include "Core/Modules/UI/Components/MouseReleased.h"
#include "Core/Modules/UI/Prefabs/Button.h"
#include "Core/Modules/UI/Prefabs/Text.h"
#include "Core/Modules/Window/Singletons/FrameCount.h"
#include "Core/Themes/Nord.h"
#include "Core/Utils/Logger.h"

GameWonScene::GameWonScene(flecs::world& world)
    : Scene(world)
{
}

void GameWonScene::Initialize()
{
    Scene::Initialize();
    GetRootEntity().set_name("GameWonScene");

    constexpr float CENTER_X = Configuration::RESOLUTION.x / 2;
    constexpr float CENTER_Y = Configuration::RESOLUTION.y / 2;

    const auto& world = GetWorld();

    CreateUISystems(world);

    // --- Create entities ---
    float zOrder = 0.f;

    // --- Overlay ---
    auto backgroundColor = NordTheme::PolarNight1;
    backgroundColor.a = 127;

    Factories::Rectangle::Create(
        world,
        {.size = sf::Vector2f{Configuration::RESOLUTION},
         .color = backgroundColor,
         .position = {0.f, 0.f},
         .scale = {1.f, 1.f},
         .rotation = 0.f}
    )
        .child_of(GetRootEntity());

    // --- Add Game Over Text ---
    Prefabs::Text::Create(
        world,
        {.text = "Game Won",
         .fontAsset = "Orbitron-Bold",
         .fontSize = 60.f,
         .textColor = NordTheme::SnowStorm3,
         .origin = sf::Vector2f{0.5f, 0.5f},
         .position = {CENTER_X, CENTER_Y - 200},
         .zOrder = ++zOrder}
    )
        .child_of(GetRootEntity());

    // --- Add Restart Button ---
    Prefabs::Button::Create(
        world,
        {
            .text = "Restart",
            .fontAsset = "Orbitron-Bold",
            .fontSize = 48.f,
            .textColor = NordTheme::SnowStorm3,
            .backgroundColor = sf::Color::Transparent,
            .position = {CENTER_X, CENTER_Y},
            .zOrder = ++zOrder,
            .onClick =
                [](const flecs::world& stage) {
                    const auto entity = stage.entity().add<LifetimeOneFrame>().add<Command>().add<RestartGameIntent>();
                    LOG_DEBUG(
                        "GameWonScene::RestartButton::onClick -> Add RestartGameIntent, entity: {}, framecount: {}",
                        entity.id(),
                        stage.get<FrameCount>().frameCount
                    );
                },
        }
    )
        .child_of(GetRootEntity());

    // --- Add Exit Button ---
    Prefabs::Button::Create(
        world,
        {.text = "Exit",
         .fontAsset = "Orbitron-Regular",
         .fontSize = 36.f,
         .textColor = NordTheme::SnowStorm3,
         .backgroundColor = sf::Color::Transparent,
         .position = {CENTER_X, CENTER_Y + 100},
         .zOrder = ++zOrder,
         .onClick = [](const flecs::world& stage
                    ) { stage.entity().set<TransitionGameStateIntent>({GameTransitions::OpenMenu}); }}
    ).child_of(GetRootEntity());
}

void GameWonScene::CreateUISystems(const flecs::world& world)
{
    // Query for KeyPressed
    world.system<const KeyPressed>("GameWonScene.ProcessKeyPressed")
        .kind(flecs::PostLoad)
        .write<TransitionGameStateIntent>()
        .with<GameStateGameWon>()
        .singleton()
        .each([](const flecs::entity& e, const KeyPressed& k) {
            LOG_DEBUG("GameWonScene::ProcessKeyPressed");
            if (k.scancode == sf::Keyboard::Scancode::Escape)
            {
                e.world().entity().set<TransitionGameStateIntent>({GameTransitions::OpenMenu});
                e.destruct();
            }
        })
        .child_of(GetRootEntity());
}

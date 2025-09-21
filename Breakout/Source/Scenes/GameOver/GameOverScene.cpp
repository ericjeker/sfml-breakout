// Copyright (c) Eric Jeker 2025.

#include "GameOverScene.h"

#include "Modules/Breakout/Components/Intents/TransitionGameStateIntent.h"
#include "Modules/Breakout/Prefabs/RestartGameIntent.h"
#include "Modules/Breakout/Prefabs/TransitionGameStateIntent.h"
#include "Scenes/Gameplay/GameplayScene.h"

#include "Core/Configuration.h"
#include "Core/GameService.h"
#include "Core/Modules/Event/Components/EventBindings.h"
#include "Core/Modules/Render/Factories/Rectangle.h"
#include "Core/Modules/UI/Components/MouseReleased.h"
#include "Core/Modules/UI/Prefabs/Button.h"
#include "Core/Modules/UI/Prefabs/Text.h"
#include "Core/Themes/Nord.h"

GameOverScene::GameOverScene(flecs::world& world)
    : Scene(world)
{
}

void GameOverScene::Initialize()
{
    Scene::Initialize();
    SetName("GameOverScene");
    GetRootEntity().set_name("GameOverScene");

    constexpr float CENTER_X = Configuration::RESOLUTION.x / 2;
    constexpr float CENTER_Y = Configuration::RESOLUTION.y / 2;

    const auto& world = GetWorld();

    // --- Create Local Systems ---
    CreateEventBindings(world);

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
        {.text = "Game Over",
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
                    stage.entity().is_a<Prefabs::TransitionGameStateIntent>().set<TransitionGameStateIntent>(
                        {GameTransitions::RestartPlaying}
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
         .onClick =
             [](const flecs::world& stage) {
                 stage.entity().is_a<Prefabs::TransitionGameStateIntent>().set<TransitionGameStateIntent>({GameTransitions::OpenMenu});
             }}
    ).child_of(GetRootEntity());
}

void GameOverScene::CreateEventBindings(const flecs::world& world) const
{
    auto restartGame = world.prefab("GameOver::RestartGameIntent").is_a<Prefabs::RestartGameIntent>();
    GetRootEntity().set<EventBindings>({{{InputKey::Keyboard(sf::Keyboard::Key::Escape), restartGame}}});
}

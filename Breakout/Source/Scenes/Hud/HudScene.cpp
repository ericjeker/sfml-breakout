// Copyright (c) Eric Jeker 2025.

#include "HudScene.h"

#include "Modules/Breakout/Tags/LivesWidget.h"
#include "Modules/Breakout/Tags/MultiplierWidget.h"
#include "Modules/Breakout/Tags/ScoreWidget.h"

#include "Core/Configuration.h"
#include "Core/GameStates/GameState.h"
#include "Core/Modules/UI/Prefabs/Text.h"
#include "Core/Themes/Nord.h"

HudScene::HudScene(flecs::world& world)
    : Scene(world)
{
}

void HudScene::Initialize()
{
    Scene::Initialize();
    SetName("HudScene");
    GetRootEntity().set_name("HudScene");

    const auto& world = GetWorld();

    // --- Build entities ---
    float zOrder = 0.f;

    Prefabs::Text::Create(
        world,
        {.text = "MULT 1x",
         .fontAsset = "Orbitron-Bold",
         .fontSize = 40.f,
         .textColor = NordTheme::SnowStorm3,
         .origin = sf::Vector2f{0.f, 0.f},
         .position = {20.f, 20.f},
         .zOrder = ++zOrder}
    )
        .add<MultiplierWidget>()
        .child_of(GetRootEntity());

    Prefabs::Text::Create(
        world,
        {.text = "SCORE 0",
         .fontAsset = "Orbitron-Bold",
         .fontSize = 40.f,
         .textColor = NordTheme::SnowStorm3,
         .origin = sf::Vector2f{.5f, 0.f},
         .position = {Configuration::RESOLUTION.x / 2, 20.f},
         .zOrder = ++zOrder}
    )
        .add<ScoreWidget>()
        .child_of(GetRootEntity());

    Prefabs::Text::Create(
        world,
        {.text = "LIVES 0",
         .fontAsset = "Orbitron-Bold",
         .fontSize = 40.f,
         .textColor = NordTheme::SnowStorm3,
         .origin = sf::Vector2f{1.f, 0.f},
         .position = {Configuration::RESOLUTION.x - 20.f, 20.f},
         .zOrder = ++zOrder}
    )
        .add<LivesWidget>()
        .child_of(GetRootEntity());
}

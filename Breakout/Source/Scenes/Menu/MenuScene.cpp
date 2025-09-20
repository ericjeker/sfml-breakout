// Copyright (c) Eric Jeker 2025.

#include "Scenes/Menu/MenuScene.h"

#include "../../Modules/Breakout/Components/Intents/TransitionGameStateIntent.h"
#include "GameStates/Gameplay/GameplayState.h"
#include "Scenes/Menu/Components/ExitGameIntent.h"
#include "Systems/ProcessExitGameIntent.h"
#include "Systems/ProcessKeyPressed.h"

#include "Core/Configuration.h"
#include "Core/GameService.h"
#include "Core/Modules/Render/Factories/Rectangle.h"
#include "Core/Modules/UI/Prefabs/Button.h"
#include "Core/Modules/UI/Prefabs/Text.h"
#include "Core/Themes/Nord.h"

MenuScene::MenuScene(flecs::world& world)
    : Scene(world)
{
}

void MenuScene::Initialize()
{
    Scene::Initialize();
    GetRootEntity().set_name("MenuScene");

    const auto& world = GetWorld();

    CreateUIEntities(world);

    // --- Declare local systems ---
    Menu::ProcessKeyPressed::Initialize(world, GetRootEntity());
    Menu::ProcessExitGameIntent::Initialize(world, GetRootEntity());
}

void MenuScene::CreateUIEntities(const flecs::world& world)
{
    constexpr float centerX = Configuration::RESOLUTION.x / 2.f;
    constexpr float centerY = Configuration::RESOLUTION.y / 2.f;

    float zOrder = 0;

    // --- Create Background ---
    Factories::Rectangle::Create(
        world,
        {
            .size = sf::Vector2f{Configuration::RESOLUTION},
            .color = NordTheme::PolarNight4,
            .position = {0.f, 0.f},
            .zOrder = zOrder++,
        }
    )
        .set_name("Background")
        .child_of(GetRootEntity());

    // --- Reference Rectangles
    Factories::Rectangle::Create(world, {.size = {10.f, 10.f}, .color = NordTheme::SnowStorm3, .position = {20.f, 20.f}, .zOrder = zOrder++})
        .set_name("ReferenceRect.TopLeft")
        .child_of(GetRootEntity());
    Factories::Rectangle::Create(
        world,
        {.size = {10.f, 10.f},
         .color = NordTheme::SnowStorm3,
         .position = {Configuration::RESOLUTION.x - 30.f, Configuration::RESOLUTION.y - 30.f},
         .zOrder = zOrder++}
    )
        .set_name("ReferenceRect.BottomRight")
        .child_of(GetRootEntity());

    // --- Add Title ---

    Prefabs::Text::Create(
        world,
        {.text = "Main Menu",
         .fontAsset = "Orbitron-Bold",
         .fontSize = 60.f,
         .textColor = NordTheme::SnowStorm3,
         .origin = sf::Vector2f{0.5f, 0.5f},
         .position = {centerX, centerY - 200},
         .zOrder = zOrder++}
    )
        .set_name("Title")
        .child_of(GetRootEntity());

    // --- Add Play Button ---
    Prefabs::Button::Create(
        world,
        {
            .text = "Play",
            .fontAsset = "Orbitron-Bold",
            .fontSize = 48.f,
            .textColor = NordTheme::SnowStorm3,
            .backgroundColor = sf::Color::Transparent,
            .position = {centerX, centerY},
            .zOrder = zOrder++,
            .onClick = [](const flecs::world& stage
                       ) { stage.entity().set<TransitionGameStateIntent>({GameTransitions::StartPlaying}); },
        }
    )
        .set_name("PlayButton")
        .child_of(GetRootEntity());

    // --- Add Exit Button ---
    Prefabs::Button::Create(
        world,
        {
            .text = "Exit",
            .fontAsset = "Orbitron-Regular",
            .fontSize = 36.f,
            .textColor = NordTheme::SnowStorm3,
            .backgroundColor = sf::Color::Transparent,
            .position = {centerX, centerY + 100},
            .zOrder = zOrder++,
            .onClick = [](const flecs::world& stage) { stage.entity().add<ExitGameIntent>(); },
        }
    )
        .set_name("ExitButton")
        .child_of(GetRootEntity());
}

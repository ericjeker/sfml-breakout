// Copyright (c) Eric Jeker 2025.

#include "Scenes/MainMenu/MainMenuScene.h"

#include "GameStates/Gameplay/GameplayState.h"
#include "Scenes/MainMenu/Components/ExitGameIntent.h"
#include "Scenes/MainMenu/Components/StartGameIntent.h"
#include "Systems/ProcessExitGameIntent.h"
#include "Systems/ProcessKeyPressed.h"
#include "Systems/ProcessStartGameIntent.h"

#include "Core/Components/DeferredEvent.h"
#include "Core/Components/WindowResizeIntent.h"
#include "Core/Configuration.h"
#include "Core/GameInstance.h"
#include "Core/Managers/GameService.h"
#include "Core/Managers/GameStateManager.h"
#include "Core/Managers/ResourceManager.h"
#include "Core/Modules/Input/Components/Command.h"
#include "Core/Modules/Lifetime/Components/LifetimeOneFrame.h"
#include "Core/Modules/Render/Factories/Rectangle.h"
#include "Core/Modules/UI/Components/MousePressed.h"
#include "Core/Modules/UI/Prefabs/Button.h"
#include "Core/Modules/UI/Prefabs/Text.h"
#include "Core/Themes/Nord.h"
#include "Core/Utils/Logger.h"

#include <format>

MainMenuScene::MainMenuScene(flecs::world& world)
    : Scene(world)
{
}

void MainMenuScene::Initialize()
{
    LOG_DEBUG("MainMenuScene:Initialize");
    Scene::Initialize();

    const auto& world = GetWorld();

    CreateUIEntities(world);

    // --- Declare local systems ---
    MainMenu::ProcessKeyPressed::Initialize(world, GetRootEntity());
    MainMenu::ProcessExitGameIntent::Initialize(world, GetRootEntity());
    MainMenu::ProcessStartGameIntent::Initialize(world, GetRootEntity());
}

void MainMenuScene::CreateUIEntities(const flecs::world& world)
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
        .child_of(GetRootEntity());

    // --- Reference Rectangles
    Factories::Rectangle::Create(world, {.size = {10.f, 10.f}, .color = NordTheme::SnowStorm3, .position = {20.f, 20.f}, .zOrder = zOrder++})
        .child_of(GetRootEntity());
    Factories::Rectangle::Create(
        world,
        {.size = {10.f, 10.f},
         .color = NordTheme::SnowStorm3,
         .position = {Configuration::RESOLUTION.x - 30.f, Configuration::RESOLUTION.y - 30.f},
         .zOrder = zOrder++}
    )
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
                       ) { stage.entity().add<LifetimeOneFrame>().add<Command>().add<StartGameIntent>(); },
        }
    )
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
            .onClick = [](const flecs::world& stage
                       ) { stage.entity().add<LifetimeOneFrame>().add<Command>().add<ExitGameIntent>(); },
        }
    )
        .child_of(GetRootEntity());
}

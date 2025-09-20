// Copyright (c) Eric Jeker 2025.

#include "Scenes/Pause/PauseScene.h"

#include "../Gameplay/Components/Intents/ResumeGameIntent.h"
#include "Modules/Breakout/Components/TransitionGameStateIntent.h"
#include "Modules/Breakout/Singletons/GameStatePaused.h"
#include "Scenes/Gameplay/GameplayScene.h"

#include "Core/Configuration.h"
#include "Core/GameService.h"
#include "Core/Managers/GameStateManager.h"
#include "Core/Modules/Render/Factories/Rectangle.h"
#include "Core/Modules/UI/Components/KeyPressed.h"
#include "Core/Modules/UI/Components/MouseReleased.h"
#include "Core/Modules/UI/Prefabs/Button.h"
#include "Core/Modules/UI/Prefabs/Text.h"
#include "Core/Modules/Window/Components/DeferredEvent.h"
#include "Core/Scenes/Tags/ScenePaused.h"
#include "Core/Themes/Nord.h"
#include "Core/Utils/Logger.h"

PauseScene::PauseScene(flecs::world& world)
    : Scene(world)
{
}

void PauseScene::Initialize()
{
    Scene::Initialize();
    GetRootEntity().set_name("PauseScene");

    constexpr float CENTER_X = Configuration::RESOLUTION.x / 2;
    constexpr float CENTER_Y = Configuration::RESOLUTION.y / 2;

    const auto& world = GetWorld();

    // --- Create Local Systems ---
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

    // --- Add Pause Text ---
    Prefabs::Text::Create(
        world,
        {.text = "Game Paused",
         .fontAsset = "Orbitron-Bold",
         .fontSize = 60.f,
         .textColor = NordTheme::SnowStorm3,
         .origin = sf::Vector2f{0.5f, 0.5f},
         .position = {CENTER_X, CENTER_Y - 200},
         .zOrder = ++zOrder}
    )
        .child_of(GetRootEntity());

    // --- Add Resume Button ---
    Prefabs::Button::Create(
        world,
        {
            .text = "Resume",
            .fontAsset = "Orbitron-Bold",
            .fontSize = 48.f,
            .textColor = NordTheme::SnowStorm3,
            .backgroundColor = sf::Color::Transparent,
            .position = {CENTER_X, CENTER_Y},
            .zOrder = ++zOrder,
            .onClick =
                [](const flecs::world& stage) {
                    // stage.entity().add<LifetimeOneFrame>().add<Command>().add<ResumeGameIntent>();
                    stage.entity().set<TransitionGameStateIntent>({GameTransitions::ResumePlaying});
                },
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
            .position = {CENTER_X, CENTER_Y + 100},
            .zOrder = ++zOrder,
            .onClick =
                [](const flecs::world& stage) {
                    stage.entity().set<TransitionGameStateIntent>({GameTransitions::OpenMenu});
                },
        }
    )
        .child_of(GetRootEntity());
}

void PauseScene::CreateUISystems(const flecs::world& world)
{
    // Query for KeyPressed
    world.system<const KeyPressed>("ProcessKeyPressed")
        .kind(flecs::PostLoad)
        .write<ResumeGameIntent>()
        .with<GameStatePaused>().singleton()
        .each([rootEntity = GetRootEntity()](const flecs::entity& e, const KeyPressed& k) {
            if (rootEntity.has<ScenePaused>())
            {
                return;
            }

            if (k.scancode == sf::Keyboard::Scancode::Escape)
            {
                e.world().entity().set<TransitionGameStateIntent>({GameTransitions::ResumePlaying});
                LOG_DEBUG("PauseScene::ProcessKeyPressed: Escape -> Add ResumeGameIntent");
            }

            e.destruct();
        })
        .child_of(GetRootEntity());
}

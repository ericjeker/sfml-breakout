// Copyright (c) Eric Jeker 2025.

#include "Scenes/Pause/PauseScene.h"

#include "Components/NavigateToMainMenuIntent.h"
#include "Components/ResumeGameIntent.h"
#include "Core/Configuration.h"
#include "Core/Events/DeferredEvent.h"
#include "Core/GameStates/GameStateManager.h"
#include "Core/Logger.h"
#include "Core/Managers/EventManager.h"
#include "Core/Managers/GameService.h"
#include "Core/Modules/Control/Components/Command.h"
#include "Core/Modules/Lifecycle/Components/LifetimeOneFrame.h"
#include "Core/Modules/Render/Prefabs/Rectangle.h"
#include "Core/Modules/UI/Components/MouseReleased.h"
#include "Core/Modules/UI/Prefabs/Button.h"
#include "Core/Modules/UI/Prefabs/MouseReleasedEvent.h"
#include "Core/Modules/UI/Prefabs/Text.h"
#include "Core/Themes/Nord.h"
#include "GameStates/Gameplay/GameplayState.h"
#include "GameStates/MainMenu/MainMenuState.h"
#include "Prefabs/NavigateToMainMenuIntent.h"
#include "Prefabs/ResumeGameIntent.h"

PauseScene::PauseScene(flecs::world& world)
    : Scene(world)
{
}

void PauseScene::Initialize()
{
    Scene::Initialize();
    LOG_DEBUG("(PauseScene:Initialize)");

    constexpr float CENTER_X = Configuration::WINDOW_SIZE.x / 2;
    constexpr float CENTER_Y = Configuration::WINDOW_SIZE.y / 2;

    auto& world = GetWorld();

    // --- Declare local prefabs ---
    world.prefab<Prefabs::NavigateToMainMenuIntent>()
        .add<LifetimeOneFrame>()
        .add<Command>()
        .add<NavigateToMainMenuIntent>()
        .child_of(GetRootEntity());

    world.prefab<Prefabs::ResumeGameIntent>().add<LifetimeOneFrame>().add<Command>().add<ResumeGameIntent>().child_of(
        GetRootEntity()
    );

    world.system<const NavigateToMainMenuIntent>()
        .each(
            [&](const flecs::iter& it, size_t, const NavigateToMainMenuIntent i)
            {
                it.world().entity().set<DeferredEvent>(
                    {.callback = [&]
                    {
                        GameService::Get<GameStateManager>().ChangeState(std::make_unique<MainMenuState>(world));
                    }}
                );
            }
        )
        .child_of(GetRootEntity());

    world.system<const ResumeGameIntent>()
        .each(
            [](const flecs::iter& it, size_t, const ResumeGameIntent i)
            {
                it.world().entity().set<DeferredEvent>(
                    {.callback =
                         [&]
                     {
                         GameService::Get<SceneManager>().UnloadScene<PauseScene>();
                         GameService::Get<SceneManager>().GetScene<ControllerDemoScene>().Resume();
                     }}
                );
            }
        )
        .child_of(GetRootEntity());


    float zOrder = 0.f;

    // --- Overlay ---
    auto backgroundColor = NordTheme::PolarNight1;
    backgroundColor.a = 127;

    Prefabs::Rectangle::Create(
        world,
        {.size = sf::Vector2f{Configuration::WINDOW_SIZE},
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
            .onClick = [](const flecs::world& stage) { stage.entity().is_a<Prefabs::ResumeGameIntent>(); },
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
            .onClick = [](const flecs::world& stage) { stage.entity().is_a<Prefabs::NavigateToMainMenuIntent>(); },
        }
    )
        .child_of(GetRootEntity());
}

void PauseScene::HandleEvent(const std::optional<sf::Event>& event)
{
    if (!IsLoaded() || IsPaused())
    {
        return;
    }

    if (const auto* mouseReleased = event->getIf<sf::Event::MouseButtonReleased>())
    {
        if (mouseReleased->button != sf::Mouse::Button::Left)
        {
            return;
        }

        GetWorld().entity().is_a<MouseReleasedEvent>().set<MouseReleased>(
            {.position = mouseReleased->position, .button = mouseReleased->button}
        );
    }
}

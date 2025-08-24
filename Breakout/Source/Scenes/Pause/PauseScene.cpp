// Copyright (c) Eric Jeker 2025.

#include "Scenes/Pause/PauseScene.h"

#include "GameStates/Gameplay/GameplayState.h"
#include "Scenes/Gameplay/Components/NavigateToMainMenuIntent.h"
#include "Scenes/Gameplay/Components/ResumeGameIntent.h"
#include "Scenes/Gameplay/GameplayScene.h"

#include "Core/Configuration.h"
#include "Core/Events/DeferredEvent.h"
#include "Core/Managers/GameService.h"
#include "Core/Managers/GameStateManager.h"
#include "Core/Modules/Control/Components/Command.h"
#include "Core/Modules/Lifetime/Components/LifetimeOneFrame.h"
#include "Core/Modules/Render/Prefabs/Rectangle.h"
#include "Core/Modules/UI/Components/KeyPressed.h"
#include "Core/Modules/UI/Components/MouseReleased.h"
#include "Core/Modules/UI/Prefabs/Button.h"
#include "Core/Modules/UI/Prefabs/KeyPressedEvent.h"
#include "Core/Modules/UI/Prefabs/MouseReleasedEvent.h"
#include "Core/Modules/UI/Prefabs/Text.h"
#include "Core/Themes/Nord.h"
#include "Core/Utils/Logger.h"

PauseScene::PauseScene(flecs::world& world)
    : Scene(world)
{
}

void PauseScene::Initialize()
{
    LOG_DEBUG("PauseScene::Initialize");
    Scene::Initialize();

    constexpr float CENTER_X = Configuration::WINDOW_SIZE.x / 2;
    constexpr float CENTER_Y = Configuration::WINDOW_SIZE.y / 2;

    const auto& world = GetWorld();

    // --- Create Local Systems ---
    CreateUISystems(world);

    // --- Create entities ---
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
            .onClick = [](const flecs::world& stage
                       ) { stage.entity().add<LifetimeOneFrame>().add<Command>().add<ResumeGameIntent>(); },
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
            .onClick = [](const flecs::world& stage
                       ) { stage.entity().add<LifetimeOneFrame>().add<Command>().add<NavigateToMainMenuIntent>(); },
        }
    )
        .child_of(GetRootEntity());
}

void PauseScene::HandleEvent(const std::optional<sf::Event>& event)
{
    if (!IsLoaded())
    {
        return;
    }

    if (const auto* mouseReleased = event->getIf<sf::Event::MouseButtonReleased>())
    {
        if (mouseReleased->button != sf::Mouse::Button::Left)
        {
            return;
        }

        // MouseReleasedEvent is treated by the UIInputSystem that will do a hit test on clickable elements
        GetWorld().entity().is_a<Prefabs::MouseReleasedEvent>().set<MouseReleased>(
            {.position = mouseReleased->position, .button = mouseReleased->button}
        );
    }
    else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
    {
        if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
        {
            GetWorld()
                .entity()
                .is_a<Prefabs::KeyPressedEvent>()
                .set<KeyPressed>({
                    .code = keyPressed->code,
                    .scancode = keyPressed->scancode,
                    .alt = keyPressed->alt,
                    .control = keyPressed->control,
                    .shift = keyPressed->shift,
                })
                .child_of(GetRootEntity());
        }
    }
}

void PauseScene::CreateUISystems(const flecs::world& world)
{
    // Query for KeyPressed
    world.system<const KeyPressed>("ProcessKeyPressed")
        .kind(flecs::PostLoad)
        .with(flecs::ChildOf, GetRootEntity())
        .each([](const flecs::entity& e, const KeyPressed& k) {
            if (k.scancode == sf::Keyboard::Scancode::Escape)
            {
                LOG_DEBUG("PauseScene::ProcessKeyPressed: Escape -> Add ResumeGameIntent");
                e.world().entity().add<LifetimeOneFrame>().add<Command>().add<ResumeGameIntent>();
            }

            e.destruct();
        })
        .child_of(GetRootEntity());
}

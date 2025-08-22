// Copyright (c) Eric Jeker 2025.

#include "Scenes/MainMenu/MainMenuScene.h"

#include "Core/Managers/GameStateManager.h"
#include "Core/Utils/Logger.h"
#include "Core/Configuration.h"
#include "Core/Events/DeferredEvent.h"
#include "Core/GameInstance.h"
#include "Core/Managers/EventManager.h"
#include "Core/Managers/GameService.h"
#include "Core/Managers/ResourceManager.h"
#include "Core/Modules/Control/Components/Command.h"
#include "Core/Modules/Lifetime/Components/LifetimeOneFrame.h"
#include "Core/Modules/Render/Prefabs/Rectangle.h"
#include "Core/Modules/UI/Components/KeyPressed.h"
#include "Core/Modules/UI/Components/MousePressed.h"
#include "Core/Modules/UI/Components/MouseReleased.h"
#include "Core/Modules/UI/Prefabs/Button.h"
#include "Core/Modules/UI/Prefabs/KeyPressedEvent.h"
#include "Core/Modules/UI/Prefabs/MouseReleasedEvent.h"
#include "Core/Modules/UI/Prefabs/Text.h"
#include "Core/Themes/Nord.h"
#include "GameStates/Gameplay/GameplayState.h"
#include "Scenes/MainMenu/Components/ExitGameIntent.h"
#include "Scenes/MainMenu/Components/StartGameIntent.h"
#include "Scenes/MainMenu/Prefabs/ExitGameIntent.h"
#include "Scenes/MainMenu/Prefabs/StartGameIntent.h"


MainMenuScene::MainMenuScene(flecs::world& world)
    : Scene(world)
{
}

void MainMenuScene::Initialize()
{
    Scene::Initialize();
    LOG_DEBUG("(MainMenuScene:Initialize)");

    auto& world = GetWorld();

    // --- Declare local prefabs ---
    world.prefab<Prefabs::ExitGameIntent>().add<LifetimeOneFrame>().add<Command>().add<ExitGameIntent>();
    world.prefab<Prefabs::StartGameIntent>().add<LifetimeOneFrame>().add<Command>().add<StartGameIntent>();

    CreateLocalSystems(world);
    CreateUIEntities(world);
}

void MainMenuScene::HandleEvent(const std::optional<sf::Event>& event)
{
    if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
    {
        if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
        {
            GetWorld().entity().is_a<KeyPressedEvent>().set<KeyPressed>({
                .code = keyPressed->code,
                .scancode = keyPressed->scancode,
                .alt = keyPressed->alt,
                .control = keyPressed->control,
                .shift = keyPressed->shift,
            });
        }
    }
    else if (const auto* mouseReleased = event->getIf<sf::Event::MouseButtonReleased>())
    {
        if (mouseReleased->button == sf::Mouse::Button::Left)
        {
            // MouseReleasedEvent is treated by the UIInputSystem that will do a hit test on clickable elements
            GetWorld().entity().is_a<MouseReleasedEvent>().set<MouseReleased>(
                {.position = mouseReleased->position, .button = mouseReleased->button}
            );
        }
    }
}

void MainMenuScene::CreateLocalSystems(flecs::world& world)
{
    // As usual, we process the pressed keys to add the matching intents.
    world.system<const KeyPressed>("ProcessKeyPressed")
        .kind(flecs::PostLoad)
        .each(
            [](const flecs::entity& e, const KeyPressed& k)
            {
                if (k.scancode == sf::Keyboard::Scancode::Escape)
                {
                    e.world().entity().is_a<Prefabs::ExitGameIntent>();
                }

                e.destruct();
            }
        )
        .child_of(GetRootEntity());

    // The created intents are processed here
    world.system<const ExitGameIntent>("ExitGameSystem")
        .each(
            [](const flecs::iter& it, size_t, const ExitGameIntent i)
            {
                it.world().entity().set<DeferredEvent>(
                    {.callback = [&] { GameService::Get<GameInstance>().RequestExit(); }}
                );
            }
        )
        .child_of(GetRootEntity());

    world.system<const StartGameIntent>("StartGameSystem")
        .each(
            [&](const flecs::iter& it, size_t, const StartGameIntent i)
            {
                it.world().entity().set<DeferredEvent>(
                    {.callback = [&]
                     { GameService::Get<GameStateManager>().ChangeState(std::make_unique<GameplayState>(world)); }}
                );
            }
        )
        .child_of(GetRootEntity());
}

void MainMenuScene::CreateUIEntities(const flecs::world& world)
{
    constexpr float centerX = Configuration::WINDOW_SIZE.x / 2;
    constexpr float centerY = Configuration::WINDOW_SIZE.y / 2;

    float zOrder = 0;

    // --- Create Background ---
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
            .onClick = [](const flecs::world& stage) { stage.entity().is_a<Prefabs::StartGameIntent>(); },
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
            .onClick = [](const flecs::world& stage) { stage.entity().is_a<Prefabs::ExitGameIntent>(); },
        }
    )
        .child_of(GetRootEntity());
}

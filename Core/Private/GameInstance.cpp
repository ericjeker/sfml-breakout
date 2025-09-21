// Copyright (c) Eric Jeker 2025.

#include "Core/GameInstance.h"

#include "Core/Configuration.h"
#include "Core/GameService.h"
#include "Core/Managers/GameStateManager.h"
#include "Core/Managers/SceneManager.h"
#include "Core/Modules/Input/Components/Command.h"
#include "Core/Modules/Lifetime/Components/LifetimeOneFrame.h"
#include "Core/Modules/UI/Components/KeyPressed.h"
#include "Core/Modules/UI/Components/MouseReleased.h"
#include "Core/Modules/UI/Prefabs/FocusLostEvent.h"
#include "Core/Modules/UI/Prefabs/KeyPressedEvent.h"
#include "Core/Modules/UI/Prefabs/MouseReleasedEvent.h"
#include "Core/Modules/Window/Components/DeferredEvent.h"
#include "Core/Modules/Window/Components/WindowResizeIntent.h"
#include "Core/Modules/Window/Singletons/FrameCount.h"
#include "Core/Modules/Window/Singletons/WindowSize.h"
#include "Core/Utils/Logger.h"

#include <tracy/Tracy.hpp>

void GameInstance::Initialize()
{
    ZoneScopedN("GameInstance::Initialize");

    const auto& window = GameService::Get<sf::RenderWindow>();

    // --- Set the initial window size used when resizing ---
    GetWorld().set<WindowSize>({.currentSize = window.getSize(), .refSize = Configuration::RESOLUTION});
}

void GameInstance::Run(sf::RenderWindow& renderWindow)
{
    ZoneScopedN("GameInstance::Run");

    // --- Get the only Flecs World ---
    flecs::world& world = GetWorld();

    // --- Game loop ---
    sf::Clock clock;
    while (renderWindow.isOpen() && !ShouldExit())
    {
        static int frameCount = 0;
        world.set<FrameCount>({frameCount++});

        const float deltaTime = clock.restart().asSeconds();

        // --- Event-Based Input System---
        HandleEvents(renderWindow);

        // --- Progressing the world ---
        renderWindow.clear();
        world.progress(deltaTime);
        renderWindow.display();

        // --- Process deferred events at the end of the frame ---
        RunDeferredEvents(world);

        // --- Tracy frame mark so Tracy can properly split each frame ---
        FrameMark;
    }

    // --- Shutdown ---
    if (renderWindow.isOpen())
    {
        renderWindow.close();
    }
}

void GameInstance::HandleEvents(sf::RenderWindow& renderWindow) const
{
    ZoneScopedN("GameInstance::HandleEvents");

    const auto& world = GetWorld();

    while (const auto event = renderWindow.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            renderWindow.close();
        }
        else if (const auto* resized = event->getIf<sf::Event::Resized>())
        {
            // TODO: Might as well move all this jazz in its own function
            assert(world.has<WindowSize>() && "WindowSize singleton does not exist.");
            auto& [size, refSize] = world.get_mut<WindowSize>();

            // Calculate the scale of the new window size
            assert(refSize.x > 0 && refSize.y > 0 && "Reference size must be greater than 0.");
            const sf::Vector2f scale =
                {static_cast<float>(resized->size.x) / static_cast<float>(refSize.x),
                 static_cast<float>(resized->size.y) / static_cast<float>(refSize.y)};
            const float scaleRatio = std::min(scale.x, scale.y);

            // Calculate transformation to center (letterboxing or pillarboxing)
            const sf::Vector2f transformRatio =
                {(static_cast<float>(resized->size.x) - static_cast<float>(refSize.x) * scaleRatio) / 2.f,
                 (static_cast<float>(resized->size.y) - static_cast<float>(refSize.y) * scaleRatio) / 2.f};

            world.entity().add<LifetimeOneFrame>().add<Command>().set<WindowResizeIntent>({
                .newSize = resized->size,
                .oldSize = size,
                .scaleRatio = scaleRatio,
                .transformRatio = transformRatio,
            });

            size = resized->size;
        }
        else if (event->is<sf::Event::FocusLost>())
        {
            // It's up to the game to handle a focus-lost event, we just log it here
            world.entity().is_a<Prefabs::FocusLostEvent>();
        }
        else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
        {
            LOG_DEBUG("GameInstance::HandleEvent -> KeyPressed");
            // We log all the key pressed events, the game will later handle them
            world.entity().is_a<Prefabs::KeyPressedEvent>().set<KeyPressed>({
                .code = keyPressed->code,
                .scancode = keyPressed->scancode,
                .alt = keyPressed->alt,
                .control = keyPressed->control,
                .shift = keyPressed->shift,
            });
        }
        else if (const auto* mouseReleased = event->getIf<sf::Event::MouseButtonReleased>())
        {
            LOG_DEBUG("GameInstance::HandleEvent -> MouseButtonReleased");
            // Same here for the mouse events
            world.entity().is_a<Prefabs::MouseReleasedEvent>().set<MouseReleased>(
                {.position = mouseReleased->position, .button = mouseReleased->button}
            );
        }
    }
}

void GameInstance::RunDeferredEvents(flecs::world& world)
{
    ZoneScopedN("GameInstance::RunDeferredEvents");

    // We will collect all the callbacks and entities to destroy here.
    // Flecs can't destroy an entity while it's iterating as it's in staging.
    std::vector<flecs::entity> toDelete;
    std::vector<std::function<void(flecs::world&)>> callbacks;
    callbacks.reserve(32);
    toDelete.reserve(32);

    // --- Loop the entities ---
    world.each<DeferredEvent>([&](flecs::entity e, DeferredEvent& ev) {
        callbacks.emplace_back(ev.callback);
        toDelete.emplace_back(e);
    });

    // --- Run the callbacks (lambdas) ---
    for (auto& cb : callbacks)
    {
        cb(world);
    }

    // --- Destroy the entities ---
    world.defer_begin();
    for (auto e : toDelete)
    {
        e.destruct();
    }
    world.defer_end();
}

void GameInstance::Shutdown()
{
    ZoneScopedN("GameInstance::Shutdown");
}

void GameInstance::RequestExit()
{
    _shouldExit = true;
}

bool GameInstance::ShouldExit() const
{
    return _shouldExit;
}

flecs::world& GameInstance::GetWorld()
{
    return _world;
}

const flecs::world& GameInstance::GetWorld() const
{
    return _world;
}

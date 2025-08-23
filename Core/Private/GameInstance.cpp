// Copyright (c) Eric Jeker 2025.

#include "Core/GameInstance.h"

#include "Core/Events/DeferredEvent.h"
#include "Core/Managers/GameService.h"
#include "Core/Managers/GameStateManager.h"
#include "Core/Managers/SceneManager.h"
#include "Core/Utils/Logger.h"

#include <tracy/Tracy.hpp>


void GameInstance::Run(sf::RenderWindow& renderWindow) const
{
    LOG_DEBUG("GameInstance::Run: Starting game loop");
    auto& world = GetWorld();

    sf::Clock clock;
    while (renderWindow.isOpen() && !ShouldExit())
    {
        static int frameCount = 0;
        frameCount++;

        const float deltaTime = clock.restart().asSeconds();

        // Event-Based Input System
        HandleEvents(renderWindow);

        // Progresses the world
        renderWindow.clear();
        world.progress(deltaTime);
        renderWindow.display();

        // Process deferred events at the end of the frame
        RunDeferredEvents(world);

        FrameMark;
    }

    LOG_DEBUG("GameInstance::Run: Game loop ended");
    if (renderWindow.isOpen())
    {
        renderWindow.close();
    }
}

void GameInstance::HandleEvents(sf::RenderWindow& renderWindow)
{
    ZoneScoped;

    while (const auto event = renderWindow.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            renderWindow.close();
        }

        // We delegate the event to the game controller
        GameService::Get<GameStateManager>().HandleEvent(event);
        GameService::Get<SceneManager>().HandleEvent(event);
    }
}

void GameInstance::RunDeferredEvents(const flecs::world& world)
{
    ZoneScopedN("GameInstance::RunDeferredEvents");

    // We will collect all the callbacks and entities to destroy here.
    // This is done because Flecs can't destroy an entity while it's iterating.
    std::vector<flecs::entity> toDelete;
    std::vector<std::function<void()>> callbacks;
    callbacks.reserve(32);
    toDelete.reserve(32);

    // Loop the entities
    world.each<DeferredEvent>([&](flecs::entity e, const DeferredEvent& ev) {
        callbacks.emplace_back(ev.callback);
        toDelete.emplace_back(e);
    });

    // Run the callbacks
    for (auto& cb : callbacks)
    {
        cb();
    }

    // Destroy the entities
    world.defer_begin();
    for (auto e : toDelete)
    {
        e.destruct();
    }
    world.defer_end();
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

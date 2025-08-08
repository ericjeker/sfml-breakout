// Copyright (c) Eric Jeker 2025.
#ifdef TRACY_ENABLE
#include <tracy/Tracy.hpp>
#endif

#include "Core/GameInstance.h"
#include "Core/GameStates/GameController.h"
#include "Core/Logger.h"
#include "Core/Managers/EventManager.h"
#include "Core/Managers/GameService.h"
#include "Core/Managers/SceneManager.h"


void GameInstance::Run(sf::RenderWindow& renderWindow) const
{
    LOG_DEBUG("(GameInstance::Run): Starting game loop");

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
        GetWorld().progress(deltaTime);
        renderWindow.display();

        // Process deferred events at the end of the frame
        GameService::Get<EventManager>().ProcessDeferredEvents();

        FrameMark;
    }

    LOG_DEBUG("(GameInstance::Run): Game loop ended");
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
        GameService::Get<GameController>().HandleEvent(event);
        GameService::Get<SceneManager>().HandleEvent(event);
    }
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

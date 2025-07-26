// Copyright (c) Eric Jeker 2025.
#ifdef TRACY_ENABLE
#include <tracy/Tracy.hpp>
#endif

#include "GameInstance.h"
#include "Gameplay/GameController.h"
#include "Logger.h"
#include "Managers/EventManager.h"
#include "Managers/GameService.h"
#include "Managers/SceneManager.h"

GameInstance::GameInstance(flecs::world world)
    : _world(world)
{
}

void GameInstance::Run(sf::RenderWindow& renderWindow)
{
    LOG_DEBUG("(GameInstance::Run): Starting game loop");

    sf::Clock clock;
    while (renderWindow.isOpen() && !ShouldExit())
    {
        static int frameCount = 0;
        frameCount++;

        const float deltaTime = clock.restart().asSeconds();

        // Call Hierarchy: GameInstance -> GameController -> GameState
        //                              -> SceneManager -> Scene -> Systems -> Components
        HandleEvents(renderWindow);
        Update(deltaTime);
        // Render(renderWindow);

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
            RequestExit();
        }

        // We delegate the event to the game controller
        GameService::Get<GameController>().HandleEvent(event);
        GameService::Get<SceneManager>().HandleEvent(event);
    }
}

void GameInstance::Update(const float deltaTime)
{
    ZoneScoped;
    GameService::Get<GameController>().Update(deltaTime);
    GameService::Get<SceneManager>().Update(deltaTime);
}

void GameInstance::Render(sf::RenderWindow& renderWindow)
{
    ZoneScoped;
    renderWindow.clear();
    GameService::Get<SceneManager>().Render(renderWindow);
    renderWindow.display();
}

void GameInstance::RequestExit()
{
    _shouldExit = true;
}

bool GameInstance::ShouldExit() const
{
    return _shouldExit;
}

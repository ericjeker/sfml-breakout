// Copyright (c) Eric Jeker 2025.

#include "GameInstance.h"

#include "Gameplay/GameController.h"
#include "Logger.h"
#include "Managers/SceneManager.h"

GameInstance::GameInstance(std::unique_ptr<GameService> gameService)
    : _gameService(std::move(gameService))
{
    LOG_DEBUG("(GameInstance::GameInstance)");
}

void GameInstance::Run(sf::RenderWindow& renderWindow)
{
    // Start the main loop
    sf::Clock clock;
    LOG_DEBUG("(GameInstance::Run): Starting game loop");
    while (renderWindow.isOpen())
    {
        static int frameCount = 0;
        frameCount++;

        const float deltaTime = clock.restart().asSeconds();

        // Call Hierarchy: GameInstance -> GameController -> GameState
        //                              -> SceneManager -> Scene -> Systems -> Components
        HandleEvents(renderWindow);
        Update(deltaTime);
        Render(renderWindow);
    }
}

void GameInstance::HandleEvents(sf::RenderWindow& renderWindow)
{
    while (const auto event = renderWindow.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            renderWindow.close();
        }

        // We delegate the event to the game controller
        GetGameService().Get<GameController>().HandleEvent(event, renderWindow);
        GetGameService().Get<SceneManager>().HandleEvent(event, renderWindow);
    }
}

void GameInstance::Update(const float deltaTime)
{
    GetGameService().Get<GameController>().Update(deltaTime);
    GetGameService().Get<SceneManager>().Update(deltaTime);
}

void GameInstance::Render(sf::RenderWindow& renderWindow)
{
    renderWindow.clear();
    GetGameService().Get<SceneManager>().Render(renderWindow);
    renderWindow.display();
}

GameService& GameInstance::GetGameService() const
{
    return *_gameService;
}

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

        if (renderWindow.hasFocus())
        {
            Update(deltaTime);
            Render(renderWindow);
        }
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
        else if (event->is<sf::Event::Resized>())
        {
        }
        else if (event->is<sf::Event::FocusLost>())
        {
        }
        else if (event->is<sf::Event::FocusGained>())
        {
        }

        // We delegate the event to the game controller
        _gameService->Get<GameController>()->HandleEvent(event);
        _gameService->Get<SceneManager>()->HandleEvent(event);
    }
}

void GameInstance::Update(const float deltaTime)
{
    _gameService->Get<GameController>()->Update(deltaTime);
    _gameService->Get<SceneManager>()->Update(deltaTime);
}

void GameInstance::Render(sf::RenderWindow& renderWindow)
{
    renderWindow.clear();
    _gameService->Get<SceneManager>()->Render(renderWindow);
    renderWindow.display();
}

GameService* GameInstance::GetGameService() const
{
    return _gameService.get();
}

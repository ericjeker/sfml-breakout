// Copyright (c) Eric Jeker 2025.

#include "Application.h"

#include "Gameplay/GameController.h"
#include "Gameplay/MainMenuState.h"
#include "Logger.h"
#include "Managers/FileManager.h"

#include <SFML/Graphics.hpp>

void Application::Initialization()
{
    LOG_DEBUG("(Application::Initialization)");

    // We load all the resource present in the manifest file
    GetResourceManager().LoadResourcesFromManifest("Assets/Manifest.json");

    // We push the initial state to the state stack
    GetGameController().PushState(std::make_unique<MainMenuState>(GetSceneManager()));
}

void Application::Run(sf::RenderWindow& renderWindow)
{
    // Start the main loop
    sf::Clock clock;
    LOG_DEBUG("(Application::Run): Starting game loop");
    while (renderWindow.isOpen())
    {
        static int frameCount = 0;
        frameCount++;

        const float deltaTime = clock.restart().asSeconds();

        // Call Hierarchy: Application -> GameController -> GameState -> SceneManager -> Scene -> Systems -> Components
        HandleEvents(renderWindow);

        if (renderWindow.hasFocus())
        {
            Update(deltaTime);
            Render(renderWindow);
        }
    }
}

void Application::HandleEvents(sf::RenderWindow& renderWindow)
{
    while (const std::optional event = renderWindow.pollEvent())
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
        GetGameController().HandleEvent(event);
    }
}

void Application::Update(const float deltaTime)
{
    GetGameController().Update(deltaTime);
}

void Application::Render(sf::RenderWindow& renderWindow)
{
    renderWindow.clear();
    GetGameController().Render(renderWindow);
    renderWindow.display();
}

GameController& Application::GetGameController() const
{
    return *_gameController;
}

void Application::SetGameController(std::unique_ptr<GameController> gameController)
{
    _gameController = std::move(gameController);
}

ResourceManager& Application::GetResourceManager() const
{
    return *_resourceManager;
}

void Application::SetResourceManager(std::unique_ptr<ResourceManager> resourceManager)
{
    _resourceManager = std::move(resourceManager);
}

SceneManager& Application::GetSceneManager() const
{
    return *_sceneManager;
}

void Application::SetSceneManager(std::unique_ptr<SceneManager> sceneManager)
{
    _sceneManager = std::move(sceneManager);
}

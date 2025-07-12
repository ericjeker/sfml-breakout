// Copyright (c) Eric Jeker 2025.

#include "Application.h"
#include "ApplicationConfiguration.h"

#include <SFML/Graphics.hpp>

/**
 * Entry point of the application. Initializes the necessary components, configures
 * the rendering window, and starts the main application loop.
 *
 * @return An integer representing the exit status of the application.
 *         Returns 0 if the application exits successfully.
 */
int main()
{
    // Create the window mode
    const sf::VideoMode mode(ApplicationConfiguration::windowSize);

    // Enable anti-aliasing
    sf::ContextSettings settings;
    settings.antiAliasingLevel = ApplicationConfiguration::antiAliasingLevel;

    auto renderWindow = sf::RenderWindow(
        mode,
        ApplicationConfiguration::windowTitle,
        ApplicationConfiguration::windowStyle,
        ApplicationConfiguration::windowState,
        settings
    );

    renderWindow.setFramerateLimit(ApplicationConfiguration::framesPerSecond);
    renderWindow.setVerticalSyncEnabled(ApplicationConfiguration::isVSync);

    // Create the game instance and initialize the different managers
    Application application;
    application.SetResourceManager(std::make_unique<ResourceManager>());
    application.SetSceneManager(std::make_unique<SceneManager>(application.GetResourceManager()));
    application.SetGameController(std::make_unique<GameController>());
    // NetworkManager
    // AudioManager
    // SoundManager
    // EventSystem

    // Initialize and Start the game loop
    application.Initialization();
    application.Run(renderWindow);
}

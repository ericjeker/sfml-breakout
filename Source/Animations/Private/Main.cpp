// Copyright (c) Eric Jeker 2025.

#include "Animations.h"
#include "ApplicationConfiguration.h"
#include "GameInstance.h"
#include "Gameplay/GameController.h"
#include "Managers/GameService.h"
#include "Managers/ResourceManager.h"
#include "Managers/SceneManager.h"

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

    // Initialize the different managers
    auto gameService = std::make_unique<GameService>();
    gameService->Register<ResourceManager>(std::make_unique<ResourceManager>());
    gameService->Register<SceneManager>(std::make_unique<SceneManager>(*gameService->Get<ResourceManager>()));
    gameService->Register<GameController>(std::make_unique<GameController>());
    // NetworkManager
    // AudioManager
    // SoundManager
    // EventSystem

    // Create the game instance
    LOG_DEBUG("(Main::main): Creating GameInstance");
    Animations animations(std::move(gameService));
    animations.Initialize();
    animations.Run(renderWindow);
    animations.Shutdown();
}

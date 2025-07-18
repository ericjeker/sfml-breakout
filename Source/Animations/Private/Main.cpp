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
    const sf::VideoMode mode(ApplicationConfiguration::WINDOW_SIZE);

    // Enable anti-aliasing
    sf::ContextSettings settings;
    settings.antiAliasingLevel = ApplicationConfiguration::ANTI_ALIASING_LEVEL;

    auto renderWindow = sf::RenderWindow(
        mode,
        ApplicationConfiguration::WINDOW_TITLE,
        ApplicationConfiguration::WINDOW_STYLE,
        ApplicationConfiguration::WINDOW_STATE,
        settings
    );

    renderWindow.setFramerateLimit(ApplicationConfiguration::FRAMES_PER_SECOND);
    renderWindow.setVerticalSyncEnabled(ApplicationConfiguration::IS_VSYNC);

    // Initialize the different managers
    auto gameService = std::make_unique<GameService>();
    gameService->Register<sf::RenderWindow>(renderWindow);
    gameService->Register<ResourceManager>(std::make_unique<ResourceManager>());
    gameService->Register<SceneManager>(std::make_unique<SceneManager>(gameService->Get<ResourceManager>()));
    gameService->Register<GameController>(std::make_unique<GameController>());
    gameService->Register<EventManager>(std::make_unique<EventManager>());
    // NetworkManager
    // AudioManager

    // Create the game instance
    LOG_DEBUG("(Main::main): Creating the GameInstance");
    Animations animations(std::move(gameService));
    animations.Initialize();
    animations.Run(renderWindow);
    animations.Shutdown();
    LOG_DEBUG("(Main::main): Bye bye!");

    return 0;
}

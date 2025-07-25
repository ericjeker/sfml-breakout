// Copyright (c) Eric Jeker 2025.

#ifdef TRACY_ENABLE
#include <tracy/Tracy.hpp>
#endif

#include "Configuration.h"
#include "Animations.h"
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
    ZoneScoped;

    // Create the window mode
    const sf::VideoMode mode(Configuration::WINDOW_SIZE);

    // Enable anti-aliasing
    sf::ContextSettings settings;
    settings.antiAliasingLevel = Configuration::ANTI_ALIASING_LEVEL;

    auto renderWindow = sf::RenderWindow(
        mode,
        Configuration::WINDOW_TITLE,
        Configuration::WINDOW_STYLE,
        Configuration::WINDOW_STATE,
        settings
    );

    renderWindow.setFramerateLimit(Configuration::FRAMES_PER_SECOND);
    renderWindow.setVerticalSyncEnabled(Configuration::IS_VSYNC);

    GameService::Initialize();
    // We register the SFML window as a reference, so it's easy to access it
    GameService::Register<sf::RenderWindow>(renderWindow);
    GameService::Register<ResourceManager>(std::make_unique<ResourceManager>());
    GameService::Register<SceneManager>(std::make_unique<SceneManager>());
    GameService::Register<GameController>(std::make_unique<GameController>());
    GameService::Register<EventManager>(std::make_unique<EventManager>());
    // TODO: NetworkManager
    // TODO: AudioManager

    // Create the game instance
    LOG_DEBUG("(Main::main): Creating the GameInstance");
    Animations animations{};

    // Register the game instance as a service
    GameService::Register<GameInstance>(animations);

    animations.Initialize();
    animations.Run(renderWindow);
    animations.Shutdown();
    LOG_DEBUG("(Main::main): Bye bye!");

    return 0;
}

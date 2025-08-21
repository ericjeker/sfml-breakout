// Copyright (c) Eric Jeker 2025.

#include "Breakout.h"
#include "Core/Configuration.h"
#include "Core/GameInstance.h"
#include "Core/GameStates/GameStateManager.h"
#include "Core/Managers/AudioManager.h"
#include "Core/Managers/EventManager.h"
#include "Core/Managers/GameService.h"
#include "Core/Managers/ResourceManager.h"
#include "Core/Managers/SceneManager.h"

#include <SFML/Graphics.hpp>

#include <nlohmann/json.hpp>
#include <tracy/Tracy.hpp>

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

    // --- Create the window mode ---
    const sf::VideoMode mode(Configuration::WINDOW_SIZE);

    // --- Enable anti-aliasing ---
    sf::ContextSettings settings;
    settings.antiAliasingLevel = Configuration::ANTI_ALIASING_LEVEL;

    auto window = sf::RenderWindow(mode, Configuration::WINDOW_TITLE, Configuration::WINDOW_STYLE, Configuration::WINDOW_STATE, settings);

    // --- Configure the window ---
    window.setFramerateLimit(Configuration::FRAMES_PER_SECOND);
    window.setVerticalSyncEnabled(Configuration::IS_VSYNC);
    window.setKeyRepeatEnabled(Configuration::ENABLE_KEY_REPEAT);

    // --- Initialize the Game Service Locator ---
    GameService::Initialize();
    // We register the SFML window as a reference, so it's easy to access it
    GameService::Register<sf::RenderWindow>(window);
    GameService::Register<ResourceManager>(std::make_unique<ResourceManager>());
    GameService::Register<AudioManager>(std::make_unique<AudioManager>(GameService::Get<ResourceManager>()));
    GameService::Register<SceneManager>(std::make_unique<SceneManager>());
    GameService::Register<GameStateManager>(std::make_unique<GameStateManager>());
    // TODO: I don't think we are using the EventManager anymore
    GameService::Register<EventManager>(std::make_unique<EventManager>());
    // TODO: NetworkManager

    // --- Create the game instance ---
    Breakout gameInstance{};

    // --- Register the game instance as a service ---
    GameService::Register<GameInstance>(gameInstance);

    gameInstance.Initialize();
    gameInstance.Run(window);
    gameInstance.Shutdown();

    return 0;
}

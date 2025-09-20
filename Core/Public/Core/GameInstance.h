// Copyright (c) Eric Jeker 2025.

#pragma once

#include <SFML/Graphics/RenderWindow.hpp>

#include <flecs.h>


class GameInstance
{
public:
    GameInstance() = default;
    virtual ~GameInstance() = default;

    virtual void Initialize();
    virtual void Shutdown();

    /**
     * @brief This is the big RUN function that stats everything
     * @param renderWindow
     */
    void Run(sf::RenderWindow& renderWindow);

    /**
     * @brief Translate the SFML events to Flecs entities
     * @param renderWindow
     */
    void HandleEvents(sf::RenderWindow& renderWindow) const;
    static void RunDeferredEvents(flecs::world& world);

    void RequestExit();
    [[nodiscard]] bool ShouldExit() const;

    // The Main And Only God World, AKA The Universe
    [[nodiscard]] flecs::world& GetWorld();
    [[nodiscard]] const flecs::world& GetWorld() const;

private:
    bool _shouldExit = false;

    // The Only World
    flecs::world _world;
};

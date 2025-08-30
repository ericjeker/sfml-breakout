// Copyright (c) Eric Jeker 2025.

#pragma once

#include "Singletons/WindowSize.h"

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
     * This is the big RUN function that stats everything
     * @param renderWindow
     */
    void Run(sf::RenderWindow& renderWindow) const;
    void HandleEvents(sf::RenderWindow& renderWindow) const;
    static void RunDeferredEvents(const flecs::world& world);

    void RequestExit();
    [[nodiscard]] bool ShouldExit() const;

    flecs::world& GetWorld();
    [[nodiscard]] const flecs::world& GetWorld() const;

private:
    bool _shouldExit = false;

    // The Only World
    flecs::world _world;
};

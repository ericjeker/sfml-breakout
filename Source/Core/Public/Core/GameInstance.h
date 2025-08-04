// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef GAMEINSTANCE_H
#define GAMEINSTANCE_H

#include <SFML/Graphics/RenderWindow.hpp>

#include <flecs.h>


class GameInstance
{
public:
    GameInstance() = default;
    virtual ~GameInstance() = default;

    virtual void Initialize() = 0;
    virtual void Shutdown() = 0;

    /**
     * This is the big RUN function that stats everything
     * @param renderWindow
     */
    void Run(sf::RenderWindow& renderWindow) const;

    static void HandleEvents(sf::RenderWindow& renderWindow);
    static void Update(float deltaTime);

    void RequestExit();
    [[nodiscard]] bool ShouldExit() const;

    virtual const flecs::world& GetWorld() const;

private:
    bool _shouldExit = false;
    flecs::world _world;
};


#endif

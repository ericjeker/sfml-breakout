// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef GAMEINSTANCE_H
#define GAMEINSTANCE_H

#include <SFML/Graphics/RenderWindow.hpp>

#include <flecs.h>


class GameInstance
{
public:
    GameInstance(flecs::world world);
    virtual ~GameInstance() = default;

    virtual void Initialize() = 0;
    virtual void Shutdown() = 0;

    void Run(sf::RenderWindow& renderWindow);
    void HandleEvents(sf::RenderWindow& renderWindow);
    void Update(float deltaTime);
    void Render(sf::RenderWindow& renderWindow);

    void RequestExit();
    [[nodiscard]] bool ShouldExit() const;

private:
    bool _shouldExit = false;
    flecs::world _world;
};


#endif

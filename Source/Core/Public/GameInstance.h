// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef GAMEINSTANCE_H
#define GAMEINSTANCE_H

#include <SFML/Graphics/RenderWindow.hpp>


class GameInstance
{
public:
    GameInstance() = default;
    virtual ~GameInstance() = default;

    virtual void Initialize() = 0;
    virtual void Shutdown() = 0;

    void Run(sf::RenderWindow& renderWindow) const;
    static void HandleEvents(sf::RenderWindow& renderWindow);
    static void Update(float deltaTime);

    void RequestExit();
    [[nodiscard]] bool ShouldExit() const;

private:
    bool _shouldExit = false;
};


#endif

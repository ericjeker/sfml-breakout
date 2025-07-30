// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

#include <optional>


class GameState
{
public:
    GameState() = default;
    virtual ~GameState() = default;

    virtual void Enter() = 0;
    virtual void Exit() = 0;

    virtual void Pause();
    virtual void Resume();
    [[nodiscard]] bool IsPaused() const;

    virtual void Update(float deltaTime);
    virtual void HandleEvent(const std::optional<sf::Event>& event) = 0;

private:
    bool _paused = false;
};


#endif

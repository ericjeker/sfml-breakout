// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef GAMEINSTANCE_H
#define GAMEINSTANCE_H

#include "Managers/GameService.h"

#include <SFML/Graphics/RenderWindow.hpp>


class GameInstance
{
public:
    GameInstance() = default;
    explicit GameInstance(std::unique_ptr<GameService> gameService);
    virtual ~GameInstance() = default;

    virtual void Initialize() = 0;
    virtual void Shutdown() = 0;

    void Run(sf::RenderWindow& renderWindow);
    void HandleEvents(sf::RenderWindow& renderWindow);
    void Update(float deltaTime);
    void Render(sf::RenderWindow& renderWindow);

    [[nodiscard]] GameService* GetGameService() const;

private:
    std::unique_ptr<GameService> _gameService;
};


#endif

// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef APPLICATION_H
#define APPLICATION_H

#include "Gameplay/GameController.h"
#include "Managers/ResourceManager.h"
#include "Managers/SceneManager.h"

#include <SFML/Graphics/RenderWindow.hpp>


class Application
{
public:
    void Initialization();
    void Run(sf::RenderWindow& renderWindow);
    void HandleEvents(sf::RenderWindow& renderWindow);
    void Update(float deltaTime);
    void Render(sf::RenderWindow& renderWindow);

    [[nodiscard]] GameController& GetGameController() const;
    void SetGameController(std::unique_ptr<GameController> gameController);
    [[nodiscard]] ResourceManager& GetResourceManager() const;
    void SetResourceManager(std::unique_ptr<ResourceManager> resourceManager);
    [[nodiscard]] SceneManager& GetSceneManager() const;
    void SetSceneManager(std::unique_ptr<SceneManager> sceneManager);

private:
    std::unique_ptr<ResourceManager> _resourceManager;
    std::unique_ptr<SceneManager> _sceneManager;
    std::unique_ptr<GameController> _gameController;
};


#endif

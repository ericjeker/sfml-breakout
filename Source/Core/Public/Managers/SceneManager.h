// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "ResourceManager.h"

#include <SFML/Graphics/RenderWindow.hpp>

#include <memory>
#include <string>
#include <vector>


struct Entity
{

};

class Scene
{
public:
    void Update(float deltaTime);
    void Render(sf::RenderWindow& window);
    void HandleEvent(const sf::Event& event);

private:
    std::string _name;
    std::string _path;
    std::vector<std::unique_ptr<Entity>> _entities;
};

class SceneManager
{
public:
    explicit SceneManager(ResourceManager& resourceManager);
    ~SceneManager() = default;

    void LoadScene(const std::string& scenePath);
    void UnloadScene(const std::string& scenePath);
    void CleanUp();

    // Add scene elements from another manifest file
    void LoadOverlayScene(const std::string& scenePath);
    void UnloadOverlayScene(const std::string& scenePath);

    void Update(float deltaTime);
    void Render(sf::RenderWindow& window);
    void HandleEvent(const sf::Event& event);

private:
    ResourceManager& _resourceManager;
    std::unique_ptr<Scene> _currentScene;
};


#endif

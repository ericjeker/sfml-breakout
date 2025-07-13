// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "ResourceManager.h"
#include "Scenes/Scene.h"

#include <SFML/Graphics/RenderWindow.hpp>

#include <memory>
#include <string>
#include <vector>


enum class LoadMode
{
    Single,
    Additive
};

/**
 * The SceneManager responsibility is to maintain a list of all currently active scenes, regardless of which GameState
 * requested them. It handles the low-level loading, unloading, updating, and rendering of these scenes.
 *
 * Scenes can overlap, and even if the GameState is paused because another state has been pushed at the top of the state
 * stack, all the scenes still need to be rendered in order.
 */
class SceneManager
{
public:
    explicit SceneManager(ResourceManager& resourceManager);
    ~SceneManager() = default;

    void AddScene(std::unique_ptr<Scene> scene);
    void LoadScene(const std::string& scenePath, LoadMode mode);
    void UnloadScene(const std::string& scenePath);
    void CleanUp();

    void Update(float deltaTime);
    void Render(sf::RenderWindow& window);
    void HandleEvent(const std::optional<sf::Event>& event);

private:
    ResourceManager& _resourceManager;
    std::vector<std::unique_ptr<Scene>> _scenes;
};


#endif

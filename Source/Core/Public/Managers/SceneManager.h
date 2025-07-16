// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "ResourceManager.h"
#include "Scenes/Scene.h"

#include <SFML/Graphics/RenderWindow.hpp>

#include <memory>


enum class SceneLoadMode
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

    template <typename T>
    void AddScene(std::unique_ptr<T> scene)
    {
        static_assert(std::is_base_of_v<Scene, T>, "T must inherit from Scene");
        _scenes[std::type_index(typeid(T))] = std::move(scene);
    }

    template <typename T>
    void LoadScene(const SceneLoadMode mode)
    {
        static_assert(std::is_base_of_v<Scene, T>, "T must inherit from Scene");

        if (mode == SceneLoadMode::Single)
        {
            CleanUp();
        }

        auto it = _scenes.find(std::type_index(typeid(T)));
        if (it != _scenes.end() && !it->second->IsLoaded())
        {
            it->second->Initialize();
            it->second->SetLoaded(true);
        }
    }

    template <typename T>
    void UnloadScene()
    {
        static_assert(std::is_base_of_v<Scene, T>, "T must inherit from Scene");
        auto it = _scenes.find(std::type_index(typeid(T)));
        if (it != _scenes.end() && it->second->IsLoaded())
        {
            it->second->Shutdown();
            it->second->SetLoaded(false);
        }
    }

    template <typename T>
    T& GetScene()
    {
        static_assert(std::is_base_of_v<Scene, T>, "T must inherit from Scene");
        auto it = _scenes.find(std::type_index(typeid(T)));
        if (it == _scenes.end())
        {
            throw std::runtime_error("Scene not found");
        }
        return *static_cast<T*>(it->second.get());
    }

    /**
     * Cleans up all currently active scenes by invoking their shutdown routine and marking them as not loaded.
     *
     * This method iterates through all stored scenes, calling their `Shutdown()` method to perform any required
     * resource cleanup or deinitialization. Afterward, it sets the scene's loaded state to false.
     * It is primarily used for unloading all active scenes when transitioning between different configurations
     * or when loading new scenes in `Single` load mode.
     */
    void CleanUp();

    void Update(float deltaTime);
    void Render(sf::RenderWindow& window);
    void HandleEvent(const std::optional<sf::Event>& event, sf::RenderWindow& renderWindow);

private:
    ResourceManager& _resourceManager;
    std::unordered_map<std::type_index, std::unique_ptr<Scene>> _scenes;
};


#endif

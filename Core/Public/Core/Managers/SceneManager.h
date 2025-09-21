// Copyright (c) Eric Jeker 2025.

#pragma once

#include "Core/Managers/ResourceManager.h"
#include "Core/Modules/Scene/Components/SceneDepth.h"
#include "Core/Scenes/Scene.h"

#include <memory>
#include <typeindex>

/**
 * @brief Scenes can be loaded additively or as single scenes
 */
enum class SceneLoadMode
{
    Single,
    Additive
};

/**
 * @brief The SceneManager responsibility is to maintain a list of all currently active scenes, regardless of which
 * GameState requested them.
 *
 * Scenes can overlap, and even if the GameState is paused because another state has been pushed at the top of the state
 * stack, all the scenes still need to be rendered in order.
 */
class SceneManager
{
public:
    SceneManager() = default;
    ~SceneManager() = default;

    template <typename T>
    void AddScene(std::unique_ptr<T> scene)
    {
        static_assert(std::is_base_of_v<Scene, T>, "T must inherit from Scene");

        const std::type_index typeIndex(typeid(T));

        if (_scenes.contains(typeIndex))
        {
            // The scene already exists, we simply return and don't initialize it
            return;
        }

        _sceneOrder.push_back(typeIndex);
        _scenes[typeIndex] = std::move(scene);
    }

    template <typename T>
    void RemoveScene()
    {
        static_assert(std::is_base_of_v<Scene, T>, "T must inherit from Scene");
        const std::type_index typeIndex(typeid(T));

        if (!_scenes.contains(typeIndex))
        {
            return;
        }

        if (_scenes[typeIndex]->IsLoaded())
        {
            _scenes[typeIndex]->Shutdown();
        }

        _scenes.erase(typeIndex);
        std::erase(_sceneOrder, typeIndex);
    }

    template <typename T>
    void LoadScene(const SceneLoadMode mode)
    {
        static_assert(std::is_base_of_v<Scene, T>, "T must inherit from Scene");

        if (mode == SceneLoadMode::Single)
        {
            UnloadAll();
        }

        auto it = _scenes.find(std::type_index(typeid(T)));
        if (it != _scenes.end() && !it->second->IsLoaded())
        {
            it->second->SetLoaded(true);
            it->second->Initialize();
            // Assign the current depth and increment the counter for the next scene
            it->second->GetRootEntity().set<SceneDepth>({_currentSceneDepth++});
        }
    }

    template <typename T>
    void UnloadScene()
    {
        static_assert(std::is_base_of_v<Scene, T>, "T must inherit from Scene");
        auto it = _scenes.find(std::type_index(typeid(T)));
        if (it != _scenes.end() && it->second->IsLoaded())
        {
            it->second->SetLoaded(false);
            it->second->Shutdown();
        }
    }

    template <typename T>
    T& GetScene()
    {
        static_assert(std::is_base_of_v<Scene, T>, "T must inherit from Scene");
        const auto it = _scenes.find(std::type_index(typeid(T)));
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
    void UnloadAll() const;

private:
    std::unordered_map<std::type_index, std::unique_ptr<Scene>> _scenes;
    std::vector<std::type_index> _sceneOrder;
    int _currentSceneDepth{0};
};

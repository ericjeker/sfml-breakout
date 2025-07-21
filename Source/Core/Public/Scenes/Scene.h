// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef SCENE_H
#define SCENE_H

#include "Entity.h"
#include "Managers/EventManager.h"
#include "Managers/ResourceManager.h"
#include "Systems/System.h"

#include <SFML/Graphics/RenderWindow.hpp>

#include <SFML/Window/Event.hpp>

#include <memory>
#include <optional>
#include <string>
#include <vector>


/**
 * A container for all the Entities, lights, cameras, and other elements
 * that make up a level or a screen (like a menu).
 *
 * TODO: Consider CRTP for Scene creation
 *
 * Scene should contain only thing related to a Scene, and not a game state as a single game state
 * can load multiple scenes.
 */
class Scene
{
public:
    Scene(ResourceManager& resourceManager, EventManager& eventManager);
    virtual ~Scene() = default;

    virtual void Initialize();
    virtual void Shutdown();
    virtual void Update(float deltaTime);
    virtual void Render(sf::RenderWindow& window);
    virtual void HandleEvent(const std::optional<sf::Event>& event, sf::RenderWindow& window);

    // -- Scene States --
    [[nodiscard]] bool IsLoaded() const;
    void SetLoaded(bool loaded);

    [[nodiscard]] bool IsPaused() const;
    void Pause();
    void Resume();

    // --- Scene Description ---
    [[nodiscard]] const std::string& GetName() const;
    [[nodiscard]] const std::string& GetPath() const;
    void SetName(const std::string& name);
    void SetPath(const std::string& path);

    // --- Entity Management ------------------------------------------------------------------------------------------

    static int GenerateId();
    void AddEntity(std::unique_ptr<Entity> entity);
    void RemoveEntity(int id);
    [[nodiscard]] Entity* GetEntity(int id) const;

    template <typename T>
    [[nodiscard]] Entity* GetEntityWithComponent()
    {
        const auto it = std::find_if(
            std::begin(_entities),
            std::end(_entities),
            [](const std::unique_ptr<Entity>& entity) { return entity->HasComponent<T>(); }
        );

        return it != std::end(_entities) ? static_cast<Entity*>(it->get()) : nullptr;
    };

    // --- Entity Management : Batch Processing ---

    void ReserveEntities(int count);
    std::vector<std::unique_ptr<Entity>>& GetEntities();

    // TODO: Allow to query only specific entities
    template <typename T>
    std::vector<std::unique_ptr<Entity>>& GetEntitiesWithComponent()
    {
        return _entities;
    }
    // TODO: Batch add and remove
    void AddEntities(std::vector<std::unique_ptr<Entity>>&& entities);
    void RemoveEntities(const std::vector<int>& ids);
    void ClearEntities();

    template <typename T>
    void ClearEntitiesWithComponent()
    {
        _entities.erase(
            std::remove_if(
                std::begin(_entities),
                std::end(_entities),
                [this](const std::unique_ptr<Entity>& entity) { return entity->HasComponent<T>(); }
            ),
            std::end(_entities)
        );
    }

    // --- System Management ---
    void AddSystem(std::unique_ptr<System> system);

    template <typename T>
    [[nodiscard]] T* GetSystem()
    {
        const auto it = std::find_if(
            std::begin(_systems),
            std::end(_systems),
            [](const std::unique_ptr<System>& system) { return dynamic_cast<T*>(system.get()) != nullptr; }
        );

        return it != std::end(_systems) ? static_cast<T*>(it->get()) : nullptr;
    }

    // --- Accessors for child classes ---
    [[nodiscard]] EventManager& GetEventManager() const;
    [[nodiscard]] ResourceManager& GetResourceManager() const;

private:
    std::string _name;
    std::string _path;

    bool _isLoaded = false;
    bool _isPaused = false;

    // TODO: Right now, entities are structures, this is not good for cache performance
    std::vector<std::unique_ptr<Entity>> _entities;

    // TODO: Organize systems by type (Update vs Render)
    // TODO: System should register a component mask and receive only the required entities
    std::vector<std::unique_ptr<System>> _systems;

    EventManager& _eventManager;
    ResourceManager& _resourceManager;
};

#endif

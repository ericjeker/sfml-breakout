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

    // --- Entity Management ---
    static int GenerateId();

    void AddEntity(std::unique_ptr<Entity> entity);
    void RemoveEntity(int id);
    [[nodiscard]] Entity* GetEntity(int id) const;
    std::vector<std::unique_ptr<Entity>>& GetEntities();

    template <typename T>
    [[nodiscard]] Entity* GetEntityWithComponent() {
        const auto it = std::find_if(
            std::begin(_entities),
            std::end(_entities),
            [](const std::unique_ptr<Entity>& entity) { return entity->HasComponent<T>(); }
        );

        return it != std::end(_entities) ? static_cast<Entity*>(it->get()) : nullptr;
    };

    // --- System Management ---
    void AddSystem(std::unique_ptr<System> system);

    // --- Accessors ---
    [[nodiscard]] EventManager& GetEventManager() const;
    [[nodiscard]] ResourceManager& GetResourceManager() const;

private:
    std::string _name;
    std::string _path;

    bool _isLoaded = false;
    bool _isPaused = false;

    std::vector<std::unique_ptr<Entity>> _entities;
    std::vector<std::unique_ptr<System>> _systems;

    EventManager& _eventManager;
    ResourceManager& _resourceManager;
};

#endif

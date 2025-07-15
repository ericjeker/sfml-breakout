// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef SCENE_H
#define SCENE_H

#include "Entity.h"
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
 */
class Scene
{
public:
    Scene() = default;
    virtual ~Scene() = default;

    virtual void Initialize() = 0;
    virtual void Shutdown();
    virtual void Update(float deltaTime);
    virtual void Render(sf::RenderWindow& window);
    virtual void HandleEvent(const std::optional<sf::Event>& event);

    [[nodiscard]] bool IsLoaded() const;
    void SetLoaded(bool loaded);

    [[nodiscard]] bool IsPaused() const;
    void Pause();
    void Resume();

    [[nodiscard]] const std::string& GetName() const;
    [[nodiscard]] const std::string& GetPath() const;
    void SetName(const std::string& name);
    void SetPath(const std::string& path);

    static int GenerateId();

    void AddEntity(std::unique_ptr<Entity> entity);
    void RemoveEntity(int id);
    Entity* GetEntity(int id) const;
    std::vector<std::unique_ptr<Entity>>& GetEntities();

    void AddSystem(std::unique_ptr<System> system);

private:
    std::string _name;
    std::string _path;

    bool _isLoaded = false;
    bool _isPaused = false;

    std::vector<std::unique_ptr<Entity>> _entities;
    std::vector<std::unique_ptr<System>> _systems;
};

#endif

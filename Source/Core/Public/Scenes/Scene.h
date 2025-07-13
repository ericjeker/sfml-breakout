// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef SCENE_H
#define SCENE_H

#include "Entity.h"

#include <SFML/Graphics/RenderWindow.hpp>

#include <SFML/Window/Event.hpp>

#include <memory>
#include <optional>
#include <string>
#include <vector>

/**
 * A container for all the GameObjects, lights, cameras, and other elements that make up a level or a screen (like a menu).
 */
class Scene
{
public:
    Scene() = default;
    virtual ~Scene() = default;

    virtual void Update(float deltaTime) = 0;
    virtual void Render(sf::RenderWindow& window) = 0;
    virtual void HandleEvent(const std::optional<sf::Event>& event) = 0;

    [[nodiscard]] bool IsLoaded() const;
    [[nodiscard]] bool IsPaused() const;
    void Pause();
    void Resume();

private:
    std::string _name;
    std::string _path;

    bool _isLoaded = false;
    bool _isPaused = false;

    std::vector<std::unique_ptr<Entity>> _entities;
};

#endif

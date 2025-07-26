// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef SCENE_H
#define SCENE_H

#include <SFML/Graphics/RenderWindow.hpp>

#include <SFML/Window/Event.hpp>

#include <flecs.h>
#include <optional>
#include <string>


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
    Scene() = default;
    virtual ~Scene() = default;

    virtual void Initialize() = 0;
    virtual void Shutdown();
    virtual void Update(float deltaTime);
    virtual void Render(sf::RenderWindow& window) = 0;
    virtual void HandleEvent(const std::optional<sf::Event>& event);

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

    flecs::world& GetWorld();
    const flecs::world& GetWorld() const;

private:
    std::string _name;
    std::string _path;

    // Flecs entities for singletons
    flecs::world _world{};

    bool _isLoaded = false;
    bool _isPaused = false;
};

#endif

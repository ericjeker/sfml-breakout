// Copyright (c) Eric Jeker 2025.

#pragma once

#include <SFML/Graphics/RenderWindow.hpp>

#include <flecs.h>
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
    explicit Scene(flecs::world& world);
    virtual ~Scene() = default;

    virtual void Initialize();
    /**
     * When the Scene is shutdown, the root entity is deleted, which deletes
     * all the child entities from the world.
     */
    virtual void Shutdown();

    std::string GetName() const;
    void SetName(std::string name);

    // --- Scene States ---
    [[nodiscard]] bool IsLoaded() const;
    void SetLoaded(bool loaded);

    // --- World & Entity Management ---
    [[nodiscard]] flecs::world& GetWorld();
    [[nodiscard]] const flecs::world& GetWorld() const;
    [[nodiscard]] flecs::entity GetRootEntity() const;

private:
    // Flecs entities for singletons
    flecs::world& _world;
    flecs::entity _rootEntity;

    std::string _name = "Scene";
    bool _isLoaded = false;
};

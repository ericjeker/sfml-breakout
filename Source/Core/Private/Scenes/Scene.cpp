// Copyright (c) Eric Jeker 2025.

#ifdef TRACY_ENABLE
#include <tracy/Tracy.hpp>
#endif

#include "Core/Logger.h"
#include "Core/Managers/GameService.h"
#include "Core/Scenes/Scene.h"


Scene::Scene(flecs::world& world)
    : _world(world)
{
}

void Scene::Initialize()
{
    LOG_DEBUG("(Scene:Initialize): Create a the root entity");
    _rootEntity = GetWorld().entity();
}

void Scene::Shutdown()
{
    if (GetRootEntity() == flecs::entity::null() || !GetWorld().exists(GetRootEntity()))
    {
        LOG_DEBUG("(Scene:Shutdown): No root entity");
        return;
    }

    LOG_DEBUG("(Scene:Shutdown): Deferring root entity destruction");
    GetWorld().defer([&] { GetWorld().delete_with(flecs::ChildOf, GetRootEntity()); });
}

void Scene::Update(const float deltaTime)
{
    // Progress of the world by one tick.
    GetLocalWorld().progress(deltaTime);
}

void Scene::HandleEvent(const std::optional<sf::Event>& event)
{
}

bool Scene::IsLoaded() const
{
    return _isLoaded;
}

void Scene::SetLoaded(const bool loaded)
{
    _isLoaded = loaded;
}

bool Scene::IsPaused() const
{
    return _isPaused;
}

void Scene::Pause()
{
    _isPaused = true;
    // GetRootEntity().disable();
    // GetLocalWorld().entity(flecs::OnUpdate).disable();
}

void Scene::Resume()
{
    _isPaused = false;
    // GetRootEntity().enable();
    // GetLocalWorld().entity(flecs::OnUpdate).enable();
}

const std::string& Scene::GetName() const
{
    return _name;
}

const std::string& Scene::GetPath() const
{
    return _path;
}

void Scene::SetName(const std::string& name)
{
    _name = name;
}

void Scene::SetPath(const std::string& path)
{
    _path = path;
}

flecs::world& Scene::GetWorld()
{
    return _world;
}

const flecs::world& Scene::GetWorld() const
{
    return _world;
}

flecs::world& Scene::GetLocalWorld()
{
    return _localWorld;
}

const flecs::world& Scene::GetLocalWorld() const
{
    return _localWorld;
}

flecs::entity& Scene::GetRootEntity()
{
    return _rootEntity;
}

const flecs::entity& Scene::GetRootEntity() const
{
    return _rootEntity;
}

// Copyright (c) Eric Jeker 2025.

#include "Core/Scenes/Scene.h"

#include "Core/Managers/GameService.h"


Scene::Scene(flecs::world& world)
    : _world(world)
{
}

void Scene::Initialize()
{
    _rootEntity = GetWorld().entity();
}

void Scene::Shutdown()
{
    if (GetRootEntity() == flecs::entity::null() || !GetWorld().exists(GetRootEntity()))
    {
        return;
    }

    GetWorld().defer([&] {
        GetWorld().delete_with(flecs::ChildOf, GetRootEntity());
        GetRootEntity().destruct();
    });
}

void Scene::SetName(const std::string name)
{
    _name = name;
}

std::string Scene::GetName() const
{
    return _name;
}

bool Scene::IsLoaded() const
{
    return _isLoaded;
}

void Scene::SetLoaded(const bool loaded)
{
    _isLoaded = loaded;
}

flecs::world& Scene::GetWorld()
{
    return _world;
}

const flecs::world& Scene::GetWorld() const
{
    return _world;
}

flecs::entity Scene::GetRootEntity() const
{
    return _rootEntity;
}

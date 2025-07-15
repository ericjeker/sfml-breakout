// Copyright (c) Eric Jeker 2025.

#include "Scenes/Scene.h"

#include "Logger.h"

void Scene::Shutdown()
{
    LOG_DEBUG("(Scene:Shutdown)");
    _entities.clear();
    _systems.clear();
}

void Scene::Update(const float deltaTime)
{
    if (!IsLoaded() || IsPaused())
    {
        return;
    }

    for (const auto& system : _systems)
    {
        system->Update(GetEntities(), deltaTime);
    }
}

void Scene::Render(sf::RenderWindow& window)
{
    if (!IsLoaded())
    {
        return;
    }

    for (const auto& system : _systems)
    {
        system->Render(GetEntities(), window);
    }
}

void Scene::HandleEvent(const std::optional<sf::Event>& event)
{
    if (!IsLoaded() || IsPaused())
    {
        return;
    }

    for (const auto& system : _systems)
    {
        system->HandleEvent(GetEntities(), event);
    }
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
}

void Scene::Resume()
{
    _isPaused = false;
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

int Scene::GenerateId()
{
    static int id = 0;
    return ++id;
}

void Scene::AddEntity(std::unique_ptr<Entity> entity)
{
    _entities.push_back(std::move(entity));
}

void Scene::RemoveEntity(const int id)
{
    _entities.erase(
        std::remove_if(_entities.begin(), _entities.end(), [&](const auto& entity) { return entity->GetId() == id; })
    );
}

Entity* Scene::GetEntity(const int id) const
{
    for (auto& entity : _entities)
    {
        if (entity->GetId() == id)
        {
            return entity.get();
        }
    }

    return nullptr;
}

std::vector<std::unique_ptr<Entity>>& Scene::GetEntities()
{
    return _entities;
}

void Scene::AddSystem(std::unique_ptr<System> system)
{
    _systems.push_back(std::move(system));
}

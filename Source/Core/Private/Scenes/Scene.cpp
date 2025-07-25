// Copyright (c) Eric Jeker 2025.

#ifdef TRACY_ENABLE
#include <tracy/Tracy.hpp>
#endif

#include "Logger.h"
#include "Scenes/Scene.h"


void Scene::Initialize()
{
}

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
        ZoneScoped;
        ZoneName(std::string("Scene::Update -> " + std::string(typeid(*system).name())).c_str(), strlen(typeid(*system).name()) + 26);
        for (const auto& entity : GetEntities())
        {
            system->Update(entity, deltaTime);
        }
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
        ZoneScoped;
        ZoneName(std::string("Scene::Render -> " + std::string(typeid(*system).name())).c_str(), strlen(typeid(*system).name()) + 26);
        for (const auto& entity : GetEntities())
        {
            system->Render(entity, window);
        }
    }
}

void Scene::HandleEvent(const std::optional<sf::Event>& event, sf::RenderWindow& renderWindow)
{
    if (!IsLoaded() || IsPaused())
    {
        return;
    }

    for (const auto& system : _systems)
    {
        ZoneScoped;
        ZoneName(
            std::string("Scene::HandleEvent -> " + std::string(typeid(*system).name())).c_str(),
            strlen(typeid(*system).name()) + 26
        );
        for (const auto& entity : GetEntities())
        {
            system->HandleEvent(entity, event, renderWindow);
        }
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

void Scene::ReserveEntities(const int count)
{
    _entities.reserve(count);
}

std::vector<std::unique_ptr<Entity>>& Scene::GetEntities()
{
    return _entities;
}

void Scene::AddEntities(std::vector<std::unique_ptr<Entity>>&& entities)
{
}

void Scene::RemoveEntities(const std::vector<int>& ids)
{
}

void Scene::ClearEntities()
{
    _entities.clear();
}

void Scene::AddSystem(std::unique_ptr<System> system)
{
    _systems.push_back(std::move(system));
}

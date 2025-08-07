// Copyright (c) Eric Jeker 2025.
#include "Core/Managers/SceneManager.h"

#include <SFML/Graphics/Font.hpp>

#include <ranges>
#include <tracy/Tracy.hpp>

void SceneManager::UnloadAll()
{
    ZoneScopedN("SceneManager::UnloadAll");
    for (const auto& typeIdx : _sceneOrder)
    {
        const auto& scene = _scenes.at(typeIdx);
        scene->SetLoaded(false);
        scene->Shutdown();
    }
}

void SceneManager::Update(const float deltaTime)
{
    ZoneScopedN("SceneManager::Update");
    for (const auto& typeIdx : _sceneOrder)
    {
        const auto& scene = _scenes.at(typeIdx);
        if (!scene->IsLoaded())
        {
            continue;
        }

        scene->Update(deltaTime);
    }
}

void SceneManager::HandleEvent(const std::optional<sf::Event>& event)
{
    ZoneScopedN("SceneManager::HandleEvent");
    for (const auto& typeIdx : _sceneOrder)
    {
        const auto& scene = _scenes.at(typeIdx);
        if (!scene->IsLoaded() || scene->IsPaused())
        {
            continue;
        }

        scene->HandleEvent(event);
    }
}

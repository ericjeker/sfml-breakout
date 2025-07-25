// Copyright (c) Eric Jeker 2025.
#ifdef TRACY_ENABLE
#include <tracy/Tracy.hpp>
#endif

#include "Managers/SceneManager.h"

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>

#include <ranges>

void SceneManager::CleanUp()
{
    ZoneScopedN("SceneManager::CleanUp");
    for (const auto& scene : _scenes | std::views::values)
    {
        scene->SetLoaded(false);
        scene->Shutdown();
    }
}

void SceneManager::Update(const float deltaTime)
{
    ZoneScopedN("SceneManager::Update");
    for (const auto& scene : _scenes | std::views::values)
    {
        if (!scene->IsLoaded() || scene->IsPaused())
        {
            continue;
        }

        scene->Update(deltaTime);
    }
}

void SceneManager::Render(sf::RenderWindow& window)
{
    ZoneScopedN("SceneManager::Render");
    for (const auto& typeIndex : _sceneOrder)
    {
        if (const auto& scene = _scenes[typeIndex]; scene->IsLoaded())
        {
            scene->Render(window);
        }
    }
}

void SceneManager::HandleEvent(const std::optional<sf::Event>& event, sf::RenderWindow& renderWindow)
{
    ZoneScopedN("SceneManager::HandleEvent");
    for (const auto& scene : _scenes | std::views::values)
    {
        if (!scene->IsLoaded())
        {
            continue;
        }

        scene->HandleEvent(event, renderWindow);
    }
}

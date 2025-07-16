// Copyright (c) Eric Jeker 2025.

#include "Managers/SceneManager.h"

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>

#include <ranges>

SceneManager::SceneManager(ResourceManager& resourceManager)
    : _resourceManager(resourceManager)
{
}

void SceneManager::CleanUp()
{
    for (const auto& scene : _scenes | std::views::values)
    {
        scene->Shutdown();
        scene->SetLoaded(false);
    }
}

void SceneManager::Update(const float deltaTime)
{
    for (const auto& scene : _scenes | std::views::values)
    {
        scene->Update(deltaTime);
    }
}

void SceneManager::Render(sf::RenderWindow& window)
{
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
    for (const auto& scene : _scenes | std::views::values)
    {
        scene->HandleEvent(event, renderWindow);
    }
}

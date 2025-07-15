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
    for (const auto& val : _scenes | std::views::values)
    {
        val->Shutdown();
        val->SetLoaded(false);
    }
}

void SceneManager::Update(float deltaTime)
{
    for (const auto& val : _scenes | std::views::values)
    {
        val->Update(deltaTime);
    }
}

void SceneManager::Render(sf::RenderWindow& window)
{
    for (const auto& val : _scenes | std::views::values)
    {
        val->Render(window);
    }
}

void SceneManager::HandleEvent(const std::optional<sf::Event>& event)
{
    for (const auto& val : _scenes | std::views::values)
    {
        val->HandleEvent(event);
    }
}

// Copyright (c) Eric Jeker 2025.

#include "Managers/SceneManager.h"

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>

SceneManager::SceneManager(ResourceManager& resourceManager)
    : _resourceManager(resourceManager)
{
}

void SceneManager::AddScene(std::unique_ptr<Scene> scene)
{
    _scenes.push_back(std::move(scene));
}

void SceneManager::LoadScene(const std::string& scenePath, const LoadMode mode)
{
    if (mode == LoadMode::Single)
    {
        CleanUp();
    }

    // TODO: Logic to load the scene given the scenePath
    // auto scene = std::make_unique<Scene>();
    // _scenes.push_back(std::move(scene));
}

void SceneManager::UnloadScene(const std::string& scenePath)
{
}

void SceneManager::CleanUp()
{
    for (auto& scene : _scenes)
    {
        // TODO: unload the scene
    }
}

void SceneManager::Update(float deltaTime)
{
    for (const auto& scene : _scenes)
    {
        scene->Update(deltaTime);
    }
}

void SceneManager::Render(sf::RenderWindow& window)
{
    for (const auto& scene : _scenes)
    {
        scene->Render(window);
    }
}

void SceneManager::HandleEvent(const std::optional<sf::Event>& event)
{
    for (const auto& scene : _scenes)
    {
        scene->HandleEvent(event);
    }
}

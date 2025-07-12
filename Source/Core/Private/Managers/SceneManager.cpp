// Copyright (c) Eric Jeker 2025.

#include "Managers/SceneManager.h"

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>

SceneManager::SceneManager(ResourceManager& resourceManager)
    : _resourceManager(resourceManager)
{
}

void SceneManager::Render(sf::RenderWindow& window)
{
    const auto& font = _resourceManager.GetResource<sf::Font>("Orbitron-Regular");
    sf::Text text(*font, "Animations");
    text.setFillColor(sf::Color::White);
    text.setPosition({20, 20});

    window.draw(text);
}

void SceneManager::HandleEvent(const sf::Event& event)
{
    _currentScene.HandleEvent(event);
}

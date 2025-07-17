// Copyright (c) Eric Jeker 2025.

#include "Systems/System.h"

void System::Update(const std::vector<std::unique_ptr<Entity>>& entities, const float deltaTime) {};
void System::Render(const std::vector<std::unique_ptr<Entity>>& entities, sf::RenderWindow& window) {};
void System::HandleEvent(const std::vector<std::unique_ptr<Entity>>& entities, const std::optional<sf::Event>& event, sf::RenderWindow& window) {};

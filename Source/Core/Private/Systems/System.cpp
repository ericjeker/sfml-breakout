// Copyright (c) Eric Jeker 2025.

#include "Systems/System.h"

void System::Update(const std::unique_ptr<Entity>& entity, const float deltaTime) {};
void System::Render(const std::unique_ptr<Entity>& entity, sf::RenderWindow& window) {};
void System::HandleEvent(const std::unique_ptr<Entity>& entity, const std::optional<sf::Event>& event, sf::RenderWindow& window) {};

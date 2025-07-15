// Copyright (c) Eric Jeker 2025.

#include "DrawableRenderer.h"

#include "../Components/Drawable.h"

void DrawableRenderer::Render(const std::vector<std::unique_ptr<Entity>>& entities, sf::RenderWindow& window)
{
    for (const auto& entity : entities)
    {
        if (entity->HasComponent<Drawable>())
        {
            const auto* component = entity->GetComponent<Drawable>();
            window.draw(*component->drawable);
        }
    }
}

void DrawableRenderer::Update(const std::vector<std::unique_ptr<Entity>>& entities, float deltaTime)
{
}

void DrawableRenderer::HandleEvent(const std::vector<std::unique_ptr<Entity>>& entities, const std::optional<sf::Event>& event)
{
}

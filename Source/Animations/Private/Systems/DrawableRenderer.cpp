// Copyright (c) Eric Jeker 2025.

#include "DrawableRenderer.h"

#include "../Components/Drawable.h"
#include "../Components/Transform.h"

#include <SFML/Graphics/Transformable.hpp>

void DrawableRenderer::Render(const std::vector<std::unique_ptr<Entity>>& entities, sf::RenderWindow& window)
{
    for (const auto& entity : entities)
    {
        auto* transform = entity->GetComponent<Transform>();
        auto* drawable = entity->GetComponent<Drawable>();

        if (!transform || !drawable)
        {
            return;
        }

        if (auto* transformable = dynamic_cast<sf::Transformable*>(drawable->drawable.get()))
        {
            transformable->setPosition(transform->position);
            transformable->setRotation(transform->rotation);
            transformable->setScale(transform->scale);
            window.draw(*drawable->drawable);
        }
    }
}

void DrawableRenderer::Update(const std::vector<std::unique_ptr<Entity>>& entities, float deltaTime)
{
}

void DrawableRenderer::HandleEvent(const std::vector<std::unique_ptr<Entity>>& entities, const std::optional<sf::Event>& event)
{
}

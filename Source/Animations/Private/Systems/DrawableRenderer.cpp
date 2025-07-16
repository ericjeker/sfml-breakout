// Copyright (c) Eric Jeker 2025.

#include "DrawableRenderer.h"

#include "../Components/DrawableComponent.h"
#include "../Components/TransformComponent.h"

#include <SFML/Graphics/Transformable.hpp>

void DrawableRenderer::Render(const std::vector<std::unique_ptr<Entity>>& entities, sf::RenderWindow& window)
{
    for (const auto& entity : entities)
    {
        auto* transform = entity->GetComponent<TransformComponent>();
        auto* drawable = entity->GetComponent<DrawableComponent>();

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

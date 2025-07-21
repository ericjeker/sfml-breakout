// Copyright (c) Eric Jeker 2025.

#include "Systems/DrawableRenderer.h"

#include "Components/DrawableComponent.h"
#include "Components/TransformComponent.h"

#include <SFML/Graphics/Transformable.hpp>

void DrawableRenderer::Render(const std::unique_ptr<Entity>& entity, sf::RenderWindow& window)
{
    const auto* transform = entity->GetComponent<TransformComponent>();
    const auto* drawable = entity->GetComponent<DrawableComponent>();

    if (!transform || !drawable || !drawable->isVisible || !drawable->drawable)
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

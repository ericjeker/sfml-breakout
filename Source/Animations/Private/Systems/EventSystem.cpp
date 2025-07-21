// Copyright (c) Eric Jeker 2025.

#include "EventSystem.h"

#include "../Components/BoundingBoxComponent.h"
#include "../Components/EventComponent.h"
#include "Components/DrawableComponent.h"
#include "Components/TransformComponent.h"

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>


void EventSystem::HandleEvent(const std::unique_ptr<Entity>& entity, const std::optional<sf::Event>& event, sf::RenderWindow& window)
{
    if (!event)
    {
        return;
    }

    const auto* eventComponent = entity->GetComponent<EventComponent>();
    auto* boundingBoxComponent = entity->GetComponent<BoundingBoxComponent>();
    if (!eventComponent || !eventComponent->callback || !eventComponent->isActive ||
        !boundingBoxComponent)
    {
        return;
    }

    if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>();
        mousePressed && eventComponent->triggerType == TriggerType::OnMousePressed)
    {
        if (const auto bounds = GetBoundingBoxFromDrawable(entity, boundingBoxComponent);
            bounds.contains(sf::Vector2<float>(mousePressed->position)))
        {
            eventComponent->callback();
        }
    }
}

sf::FloatRect EventSystem::GetBoundingBoxFromDrawable(const std::unique_ptr<Entity>& entity, BoundingBoxComponent* boundingBoxComponent)
{
    sf::FloatRect bounds;

    if (!boundingBoxComponent->boundsNeedsUpdate)
    {
        bounds = boundingBoxComponent->bounds;
    }
    else
    {
        const auto* drawableComponent = entity->GetComponent<DrawableComponent>();

        // Try to get bounds from common SFML drawable types
        if (const auto* sprite = dynamic_cast<sf::Sprite*>(drawableComponent->drawable.get()))
        {
            bounds = sprite->getGlobalBounds();
        }
        else if (const auto* text = dynamic_cast<sf::Text*>(drawableComponent->drawable.get()))
        {
            bounds = text->getGlobalBounds();
        }
        else if (const auto* rectangleShape = dynamic_cast<sf::RectangleShape*>(drawableComponent->drawable.get()))
        {
            bounds = rectangleShape->getGlobalBounds();
        }
        else if (const auto* circleShape = dynamic_cast<sf::CircleShape*>(drawableComponent->drawable.get()))
        {
            bounds = circleShape->getGlobalBounds();
        }

        // Update the bounding box component
        boundingBoxComponent->bounds = bounds;
    }

    return bounds;
}

// Copyright (c) Eric Jeker 2025.

#include "Systems/ScreenBounceSystem.h"

#include "Components/DrawableComponent.h"
#include "Components/PhysicsComponent.h"
#include "Components/TransformComponent.h"

#include <SFML/Graphics/CircleShape.hpp>

ScreenBounceSystem::ScreenBounceSystem(const sf::Vector2u screenSize)
    : _screenSize(screenSize)
{
}
void ScreenBounceSystem::Update(const std::unique_ptr<Entity>& entity, const float deltaTime)
{
    if (!entity->HasComponent<PhysicsComponent>() || !entity->HasComponent<TransformComponent>())
    {
        return;
    }

    const auto* drawable = entity->GetComponent<DrawableComponent>();
    auto* transform = entity->GetComponent<TransformComponent>();
    auto* physics = entity->GetComponent<PhysicsComponent>();
    float width = 0.f;

    if (const auto* shape = dynamic_cast<sf::CircleShape*>(drawable->drawable.get()))
    {
        width = shape->getRadius();
    }

    if (transform->position.x < 0.f + width)
    {
        physics->velocity.x = -physics->velocity.x;
        transform->position.x = 0.f + width;
    }
    else if (transform->position.x > _screenSize.x - width)
    {
        physics->velocity.x = -physics->velocity.x;
        transform->position.x = _screenSize.x - width;
    }

    if (transform->position.y < 0.f + width)
    {
        physics->velocity.y = -physics->velocity.y;
        transform->position.y = 0.f + width;
    }
    else if (transform->position.y > _screenSize.y - width)
    {
        physics->velocity.y = -physics->velocity.y;
        transform->position.y = _screenSize.y - width;
    }
}

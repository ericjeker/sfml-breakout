// Copyright (c) Eric Jeker 2025.

#include "Systems/ScreenBounceSystem.h"

#include "Components/PhysicsComponent.h"
#include "Components/TransformComponent.h"

ScreenBounceSystem::ScreenBounceSystem(const sf::Vector2u screenSize) : _screenSize(screenSize)
{

}
void ScreenBounceSystem::Update(const std::vector<std::unique_ptr<Entity>>& entities, const float deltaTime)
{
    for (const auto& entity : entities)
    {
        if (!entity->HasComponent<PhysicsComponent>() || !entity->HasComponent<TransformComponent>())
        {
            continue;
        }

        const auto* transform = entity->GetComponent<TransformComponent>();
        auto* physics = entity->GetComponent<PhysicsComponent>();

        if (transform->position.x < 0.f || transform->position.x > _screenSize.x)
        {
            physics->velocity.x = -physics->velocity.x;
        }

        if (transform->position.y < 0.f || transform->position.y > _screenSize.y)
        {
            physics->velocity.y = -physics->velocity.y;
        }
    }

}

// Copyright (c) Eric Jeker 2025.

#include "Systems/PhysicsSystem.h"

#include "Components/PhysicsComponent.h"
#include "Components/TransformComponent.h"

PhysicsSystem::PhysicsSystem(const sf::Vector2f gravity, const float pixelsPerMeter)
    : _gravity(gravity)
    , _pixelsPerMeter(pixelsPerMeter)
{
}

void PhysicsSystem::Update(const std::vector<std::unique_ptr<Entity>>& entities, const float deltaTime)
{
    for (const auto& entity : entities)
    {
        if (!entity->HasComponent<PhysicsComponent>() || !entity->HasComponent<TransformComponent>())
        {
            continue;
        }

        auto* physics = entity->GetComponent<PhysicsComponent>();
        auto* transform = entity->GetComponent<TransformComponent>();

        physics->acceleration += _gravity * _pixelsPerMeter * deltaTime;

        physics->velocity += physics->acceleration * physics->friction * deltaTime;
        transform->position += physics->velocity * deltaTime;
        physics->acceleration = sf::Vector2f(0.f, 0.f);
    }
}

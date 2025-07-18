// Copyright (c) Eric Jeker 2025.

#include "Systems/PhysicsSystem.h"

#include "Components/PhysicsComponent.h"
#include "Components/TransformComponent.h"

PhysicsSystem::PhysicsSystem(const sf::Vector2f gravity, const float pixelsPerCentimeters)
    : _gravity(gravity)
    , _pixelsPerCentimeters(pixelsPerCentimeters)
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

        physics->acceleration += _gravity * _pixelsPerCentimeters;

        physics->velocity += physics->acceleration * deltaTime;
        physics->velocity *= physics->friction;

        transform->position += physics->velocity * deltaTime;
        physics->acceleration = sf::Vector2f(0.f, 0.f);
    }
}

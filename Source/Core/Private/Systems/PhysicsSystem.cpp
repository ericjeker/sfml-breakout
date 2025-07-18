// Copyright (c) Eric Jeker 2025.

#include "Systems/PhysicsSystem.h"

#include "../../../Animations/Private/Components/PlayerPossessedComponent.h"
#include "Components/PhysicsComponent.h"
#include "Components/TransformComponent.h"

PhysicsSystem::PhysicsSystem(const sf::Vector2f gravity, const float pixelsPerCentimeters)
    : _gravity(gravity)
    , _pixelsPerCentimeters(pixelsPerCentimeters)
{
}

void PhysicsSystem::Update(const std::unique_ptr<Entity>& entity, const float deltaTime)
{
    if (entity->HasComponent<PlayerPossessedComponent>() || !entity->HasComponent<PhysicsComponent>() ||
        !entity->HasComponent<TransformComponent>())
    {
        return;
    }

    auto* physics = entity->GetComponent<PhysicsComponent>();
    auto* transform = entity->GetComponent<TransformComponent>();

    physics->acceleration += _gravity * _pixelsPerCentimeters;

    physics->velocity += physics->acceleration * deltaTime;
    physics->velocity *= physics->damping;

    transform->position += physics->velocity * deltaTime;
    physics->acceleration = sf::Vector2f(0.f, 0.f);
}

void PhysicsSystem::ToggleGravity()
{
    if (_gravity.x > 0.f || _gravity.y > 0.f)
    {
        _oldGravity = _gravity;
        _gravity *= 0.f;
    }
    else
    {
        _gravity = _oldGravity;
    }
}

// Copyright (c) Eric Jeker 2025.

#include "MovementSystem.h"

#include "../Components/MoveToOrderComponent.h"
#include "../Components/MovementComponent.h"
#include "../Components/SelectedComponent.h"
#include "Components/DrawableComponent.h"
#include "Components/TransformComponent.h"

void MovementSystem::Update(const std::unique_ptr<Entity>& entity, const float deltaTime)
{
    // We want to make sure we are on the PlayerController
    const auto* moveOrder = entity->GetComponent<MoveToOrderComponent>();
    const auto* player = entity->GetComponent<SelectedComponent>();
    const auto* movement = entity->GetComponent<MovementComponent>();
    auto* transform = entity->GetComponent<TransformComponent>();
    auto* drawable = entity->GetComponent<DrawableComponent>();
    if (!player || !moveOrder || !transform || !movement || !drawable || !drawable->isVisible)
    {
        return;
    }

    // We move the entity steadily to the target
    const sf::Vector2f deltaPosition = moveOrder->target - transform->position;
    // TODO: Add a Lerp function somewhere, in the MovementComponent?
    transform->position += deltaPosition * movement->speed * deltaTime;
    drawable->boundsNeedUpdate = true;

    // Test if the player is nearby the target, remove the order component
    if (deltaPosition.length() < 3.f)
    {
        entity->RemoveComponent<MoveToOrderComponent>();
    }
}

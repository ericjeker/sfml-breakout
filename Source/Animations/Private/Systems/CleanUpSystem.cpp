// Copyright (c) Eric Jeker 2025.

#include "CleanUpSystem.h"

#include "../Components/ForGarbageCollectionComponent.h"
#include "Components/PhysicsComponent.h"

void CleanUpSystem::Update(const std::unique_ptr<Entity>& entity, const float deltaTime)
{
    if (entity->HasComponent<PhysicsComponent>())
    {
        if (const auto* physics = entity->GetComponent<PhysicsComponent>();
            physics->velocity.x == 0.f && physics->velocity.y == 0.f)
        {
            entity->RemoveComponent<PhysicsComponent>();
            entity->AddComponent<ForGarbageCollectionComponent>({});
        }
    }
    else if (entity->HasComponent<ForGarbageCollectionComponent>())
    {
        // TODO: I don't have a reference to the scene here...
    }
}

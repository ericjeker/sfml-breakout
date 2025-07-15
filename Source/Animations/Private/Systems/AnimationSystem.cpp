// Copyright (c) Eric Jeker 2025.

#include "AnimationSystem.h"

#include "../Components/Animation.h"
#include "../Components/Transform.h"

void AnimationSystem::Update(const std::vector<std::unique_ptr<Entity>>& entities, const float deltaTime)
{
    // Process all entities with Animation and Transform components
    for (auto& entity : entities)
    {
        auto* animation = entity->GetComponent<Animation>();
        auto* transform = entity->GetComponent<Transform>();

        animation->elapsed += deltaTime;
        float progress = animation->elapsed / animation->duration;

        if (progress >= 1.f)
        {
            progress = 1.f;
            if (animation->loop)
            {
                animation->elapsed = 0.f;
            }
        }

        // Apply animation based on type
        switch (animation->type)
        {
            case Animation::ROTATE:
                transform->rotation = sf::degrees(lerp(animation->startFloat, animation->endFloat, progress));
                break;
            case Animation::SCALE:
                transform->scale = lerp(animation->startValue, animation->endValue, progress);
                break;
            case Animation::MOVE:
                transform->position = lerp(animation->startValue, animation->endValue, progress);
                break;
            // ... other animation types
            default:;
        }
    }
}

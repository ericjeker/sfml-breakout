// Copyright (c) Eric Jeker 2025.

#include "FpsSystem.h"

#include "../Components/FpsComponent.h"
#include "Components/DrawableComponent.h"

#include <SFML/Graphics/Text.hpp>

void FpsSystem::Update(const std::vector<std::unique_ptr<Entity>>& entities, float deltaTime)
{
    for (const auto& entity : entities)
    {
        if (!entity->HasComponent<DrawableComponent>() || !entity->HasComponent<FpsComponent>())
        {
            continue;
        }

        auto *drawable = entity->GetComponent<DrawableComponent>();
        auto *const fps = entity->GetComponent<FpsComponent>();

        fps->timeSinceLastUpdate += deltaTime;
        if (fps->timeSinceLastUpdate < fps->updateEvery)
        {
            continue;
        }
        fps->timeSinceLastUpdate = 0.f;

        fps->fps = 1.f / deltaTime;

        if (auto* text = dynamic_cast<sf::Text*>(drawable->drawable.get()))
        {
            text->setString(std::to_string(fps->fps));
        }
    }
}

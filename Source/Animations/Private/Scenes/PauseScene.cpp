// Copyright (c) Eric Jeker 2025.

#include "PauseScene.h"

#include "../Components/DrawableComponent.h"
#include "../Components/TransformComponent.h"
#include "../Systems/DrawableRenderer.h"
#include "Logger.h"

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>

PauseScene::PauseScene(ResourceManager& resourceManager, EventManager& eventManager)
    : Scene(resourceManager, eventManager)
{
}

void PauseScene::Initialize()
{
    LOG_DEBUG("(PauseScene:Initialize)");

    const auto font = GetResourceManager().GetResource<sf::Font>("Orbitron-Regular");
    auto text = std::make_unique<sf::Text>(*font);
    text->setString("Pause Scene");
    text->setOrigin(text->getLocalBounds().size / 2.f);

    DrawableComponent drawable;
    drawable.drawable = std::move(text);

    TransformComponent transform;
    transform.position = {1920.f / 2.f, 1080.f / 2.f};
    transform.scale = {1.f, 1.f};

    auto entity = std::make_unique<Entity>(GenerateId());
    entity->AddComponent<DrawableComponent>(std::move(drawable));
    entity->AddComponent<TransformComponent>(std::move(transform));

    AddEntity(std::move(entity));

    // -- Add the Systems --
    auto renderer = std::make_unique<DrawableRenderer>();
    AddSystem(std::move(renderer));
}

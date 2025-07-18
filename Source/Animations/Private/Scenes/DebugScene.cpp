// Copyright (c) Eric Jeker 2025.

#include "DebugScene.h"

#include "../Components/FpsComponent.h"
#include "../Systems/FpsSystem.h"
#include "Components/DrawableComponent.h"
#include "Components/TransformComponent.h"
#include "Systems/DrawableRenderer.h"
#include "Themes/Nord.h"

DebugScene::DebugScene(ResourceManager& resourceManager, EventManager& eventManager)
    : Scene(resourceManager, eventManager)
{
}

void DebugScene::Initialize()
{
    const auto font = GetResourceManager().GetResource<sf::Font>("Orbitron-Bold");
    auto fpsText = std::make_unique<sf::Text>(*font, "", 10);
    fpsText->setFillColor(NordTheme::SnowStorm3);

    auto entity = std::make_unique<Entity>(GenerateId());
    entity->AddComponent<DrawableComponent>({.drawable = std::move(fpsText)});
    entity->AddComponent<TransformComponent>({.position = {5.f, 5.f}});
    entity->AddComponent<FpsComponent>({.fps = 0});

    AddEntity(std::move(entity));

    // --- Systems ---
    AddSystem(std::make_unique<FpsSystem>());
    AddSystem(std::make_unique<DrawableRenderer>());
}

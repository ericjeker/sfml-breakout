// Copyright (c) Eric Jeker 2025.

#include "DebugScene.h"

#include "Managers/GameService.h"

#include <Systems/DrawableRenderer.h>
#include <Themes/Nord.h>

#include <Components/DrawableComponent.h>
#include <Components/TransformComponent.h>


void DebugScene::Initialize()
{
    const auto font = GameService::Get<ResourceManager>().GetResource<sf::Font>("Orbitron-Bold");
    auto fpsText = std::make_unique<sf::Text>(*font, "FPS: ", 10);
    fpsText->setFillColor(NordTheme::SnowStorm3);

    auto fpsEntity = std::make_unique<Entity>(_fpsEntityId);
    fpsEntity->AddComponent<DrawableComponent>({.drawable = std::move(fpsText)});
    fpsEntity->AddComponent<TransformComponent>({.position = {5.f, 5.f}});

    AddEntity(std::move(fpsEntity));

    // --- Systems ---
    AddSystem(std::make_unique<DrawableRenderer>());
}

void DebugScene::Update(const float deltaTime)
{
    static float sinceLastUpdate = 0.f;

    sinceLastUpdate += deltaTime;
    if (sinceLastUpdate >= .3f)
    {
        const auto* fpsComponent = GetEntity(_fpsEntityId)->GetComponent<DrawableComponent>();
        if (auto* fpsDrawable = dynamic_cast<sf::Text*>(fpsComponent->drawable.get()))
        {
            fpsDrawable->setString("FPS: " + std::to_string(static_cast<int>(1.f / deltaTime)));
        }

        sinceLastUpdate = 0.f;
    }

    Scene::Update(deltaTime);
}

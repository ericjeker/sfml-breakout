// Copyright (c) Eric Jeker 2025.

#include "DebugScene.h"

#include "../Components/DebugComponent.h"
#include "../Events/BallCountChangedEvent.h"
#include "../Events/BallCountRequestedEvent.h"
#include "../Systems/FpsSystem.h"
#include "Components/DrawableComponent.h"
#include "Components/TransformComponent.h"
#include "Systems/DrawableRenderer.h"
#include "Themes/Nord.h"

#include <cmath>

DebugScene::DebugScene(ResourceManager& resourceManager, EventManager& eventManager)
    : Scene(resourceManager, eventManager)
{
}

void DebugScene::Initialize()
{
    const auto font = GetResourceManager().GetResource<sf::Font>("Orbitron-Bold");
    auto fpsText = std::make_unique<sf::Text>(*font, "FPS: ", 10);
    fpsText->setFillColor(NordTheme::SnowStorm3);

    auto fpsEntity = std::make_unique<Entity>(_fpsEntityId);
    fpsEntity->AddComponent<DrawableComponent>({.drawable = std::move(fpsText)});
    fpsEntity->AddComponent<TransformComponent>({.position = {5.f, 5.f}});

    auto ballCountText = std::make_unique<sf::Text>(*font, "Ball count: ", 10);
    ballCountText->setFillColor(NordTheme::SnowStorm3);

    auto ballCountEntity = std::make_unique<Entity>(_ballCountEntityId);
    ballCountEntity->AddComponent<DrawableComponent>({.drawable = std::move(ballCountText)});
    ballCountEntity->AddComponent<TransformComponent>({.position = {5.f, 20.f}});

    AddEntity(std::move(fpsEntity));
    AddEntity(std::move(ballCountEntity));

    // --- Systems ---
    AddSystem(std::make_unique<DrawableRenderer>());

    // --- Listeners ---
    auto& eventManager = GetEventManager();
    eventManager.Subscribe<BallCountChangedEvent>(
        [this](const BallCountChangedEvent& event, void* sender)
        {
            _ballCount = event.count;
        }
    );

    // To ensure we get the ball count after initialization, we request it; this is because the other scene might
    // have already sent the initial ball count event.
    eventManager.Emit<BallCountRequestedEvent>({}, this);
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

    const auto* ballCountComponent = GetEntity(_ballCountEntityId)->GetComponent<DrawableComponent>();
    if (auto* ballCountDrawable = dynamic_cast<sf::Text*>(ballCountComponent->drawable.get()))
    {
        ballCountDrawable->setString("Ball count: " + std::to_string(_ballCount));
    }

    Scene::Update(deltaTime);
}

// Copyright (c) Eric Jeker 2025.

#include "DebugScene.h"

#include "GameInstance.h"
#include "Managers/GameService.h"
#include "Managers/ResourceManager.h"

#include <Themes/Nord.h>

void DebugScene::Initialize()
{
    Scene::Initialize();

    const auto ecsWorld = GetWorld();

    // --- Resources ---
    const auto font = GameService::Get<ResourceManager>().GetResource<sf::Font>("Orbitron-Bold");
    auto fpsText = std::make_unique<sf::Text>(*font, "FPS: ", 10);
    fpsText->setFillColor(NordTheme::SnowStorm3);

    // --- Entities ---
    ecsWorld.entity("Fps").set<Transform>({.position = {5.f, 5.f}}).set<TextRenderable>({.text = std::move(fpsText)});

    // --- Systems ---
    ecsWorld.system<Transform, TextRenderable>().each(ProcessText);
    ecsWorld.system<TextRenderable>().kind(flecs::OnStore).each(RenderText);
}

void DebugScene::RenderText(const TextRenderable& textRenderable)
{
    auto& window = GameService::Get<sf::RenderWindow>();
    window.draw(*textRenderable.text);
}

void DebugScene::ProcessText(const flecs::iter& it, size_t, const Transform& t, const TextRenderable& textRenderable)
{
    static float sinceLastUpdate = 0.f;
    static int frameCount = 0;
    const float deltaTime = it.world().delta_time();
    sinceLastUpdate += deltaTime;
    frameCount++;

    if (sinceLastUpdate >= .3f)
    {
        const float averageFps = frameCount / sinceLastUpdate;
        textRenderable.text->setString("FPS: " + std::to_string(static_cast<int>(averageFps)));
        sinceLastUpdate = 0.f;
        frameCount = 0;
    }

    textRenderable.text->setPosition(t.position);
}

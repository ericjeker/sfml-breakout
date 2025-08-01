// Copyright (c) Eric Jeker 2025.

#include "DebugScene.h"

#include "Managers/GameService.h"
#include "Modules/Render/Components/TextRenderable.h"
#include "Modules/Render/RenderModule.h"
#include "Modules/UI/Prefabs/Text.h"

#include <Themes/Nord.h>

// Local systems, or I could create a DebugSceneModule
namespace
{

void CalculateFPS(const flecs::iter& it, size_t, const TextRenderable& textRenderable, const FPS& f)
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
}

} // namespace

void DebugScene::Initialize()
{
    Scene::Initialize();

    auto world = GetWorld();

    world.import <Modules::RenderModule>();

    // --- Resources ---
    const flecs::entity& entity = Prefabs::Text::Create(
        world,
        {
            .text = "FPS: ",
            .fontAsset = "Orbitron-Bold",
            .fontSize = 10,
            .textColor = NordTheme::SnowStorm3,
            .origin = {0.f, 0.f},
            .position = {5.f, 5.f},
        }
    );

    // Tag the text so we can easily edit it
    entity.add<FPS>();

    world.system<const TextRenderable, const FPS>().each(CalculateFPS);
}

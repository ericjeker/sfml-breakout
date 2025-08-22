// Copyright (c) Eric Jeker 2025.

#include "Scenes/Debug/DebugScene.h"

#include "Core/Managers/GameService.h"
#include "Core/Modules/Render/Components/TextRenderable.h"
#include "Core/Modules/Render/RenderModule.h"
#include "Core/Modules/UI/Prefabs/Text.h"

#include <Core/Themes/Nord.h>
#include <cmath>

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
        const float averageFps = std::ceil(frameCount / sinceLastUpdate);
        textRenderable.text->setString("FPS: " + std::to_string(static_cast<int>(averageFps)));
        sinceLastUpdate = 0.f;
        frameCount = 0;
    }
}

} // namespace

DebugScene::DebugScene(flecs::world& world)
    : Scene(world)
{
}

void DebugScene::Initialize()
{
    Scene::Initialize();

    const auto& world = GetWorld();

    // --- Systems ---
    world.system<const TextRenderable, const FPS>().each(CalculateFPS).child_of(GetRootEntity());

    // --- Resources ---
    Prefabs::Text::Create(
        world,
        {
            .text = "FPS: ",
            .fontAsset = "Orbitron-Bold",
            .fontSize = 10,
            .textColor = NordTheme::SnowStorm3,
            .origin = {0.f, 0.f},
            .position = {5.f, 5.f},
        }
    )
        .child_of(GetRootEntity())
        .add<FPS>();

}

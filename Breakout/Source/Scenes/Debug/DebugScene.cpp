// Copyright (c) Eric Jeker 2025.

#include "Scenes/Debug/DebugScene.h"

#include "Modules/Debug/Tags/FPSWidget.h"

#include "Core/GameService.h"
#include "Core/Modules/UI/Prefabs/Text.h"
#include "Core/Themes/Nord.h"

namespace
{


} // namespace

DebugScene::DebugScene(flecs::world& world)
    : Scene(world)
{
}

void DebugScene::Initialize()
{
    Scene::Initialize();
    GetRootEntity().set_name("DebugScene");

    const auto& world = GetWorld();

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
        .add<FPSWidget>()
        .child_of(GetRootEntity());
}

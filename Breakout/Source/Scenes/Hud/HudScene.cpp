// Copyright (c) Eric Jeker 2025.

#include "HudScene.h"

#include "GameStates/Gameplay/Components/GameSession.h"
#include "GameStates/Gameplay/Components/Score.h"

#include "Core/Configuration.h"
#include "Core/Modules/Render/Components/TextRenderable.h"
#include "Core/Modules/UI/Prefabs/Text.h"
#include "Core/Themes/Nord.h"

HudScene::HudScene(flecs::world& world)
    : Scene(world)
{
}

void HudScene::Initialize()
{
    Scene::Initialize();

    const auto& world = GetWorld();

    // --- Build entities ---
    float zOrder = 0.f;

    const auto scoreEntity = Prefabs::Text::Create(
                                 world,
                                 {.text = "0",
                                  .fontAsset = "Orbitron-Bold",
                                  .fontSize = 40.f,
                                  .textColor = NordTheme::SnowStorm3,
                                  .origin = sf::Vector2f{1.0f, 0.f},
                                  .position = {Configuration::WINDOW_SIZE.x - 500, 20},
                                  .zOrder = ++zOrder}
    )
                                 .child_of(GetRootEntity());


    // --- Create local systems ---
    world.system<const GameSession, const Score>("DisplayScoreSystem")
        .kind(flecs::PostUpdate)
        .each([scoreEntity](const GameSession& gs, const Score& score) {
            const auto& [text] = scoreEntity.get<TextRenderable>();
            text->setString(std::to_string(score.score));
        })
        .child_of(GetRootEntity());
}

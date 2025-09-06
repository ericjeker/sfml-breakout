// Copyright (c) Eric Jeker 2025.

#include "HudScene.h"

#include "GameStates/Gameplay/Components/Lives.h"
#include "GameStates/Gameplay/Components/Multiplier.h"
#include "GameStates/Gameplay/Components/Score.h"

#include "Core/Configuration.h"
#include "Core/Modules/Render/Components/Origin.h"
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

    const auto multEntity = Prefabs::Text::Create(
                                world,
                                {.text = "MULT 1x",
                                 .fontAsset = "Orbitron-Bold",
                                 .fontSize = 40.f,
                                 .textColor = NordTheme::SnowStorm3,
                                 .origin = sf::Vector2f{0.f, 0.f},
                                 .position = {20.f, 20.f},
                                 .zOrder = ++zOrder}
    )
                                .child_of(GetRootEntity());

    const auto scoreEntity = Prefabs::Text::Create(
                                 world,
                                 {.text = "SCORE 0",
                                  .fontAsset = "Orbitron-Bold",
                                  .fontSize = 40.f,
                                  .textColor = NordTheme::SnowStorm3,
                                  .origin = sf::Vector2f{.5f, 0.f},
                                  .position = {Configuration::RESOLUTION.x / 2, 20.f},
                                  .zOrder = ++zOrder}
    )
                                 .child_of(GetRootEntity());

    const auto livesEntity = Prefabs::Text::Create(
                                 world,
                                 {.text = "LIVES 0",
                                  .fontAsset = "Orbitron-Bold",
                                  .fontSize = 40.f,
                                  .textColor = NordTheme::SnowStorm3,
                                  .origin = sf::Vector2f{1.f, 0.f},
                                  .position = {Configuration::RESOLUTION.x - 20.f, 20.f},
                                  .zOrder = ++zOrder}
    )
                                 .child_of(GetRootEntity());


    // --- Create local systems ---
    world.system<const Score>("DisplayScoreSystem")
        .kind(flecs::PostUpdate)
        .each([scoreEntity](const Score& score) {
            const auto& [text] = scoreEntity.get<TextRenderable>();
            const auto& [origin] = scoreEntity.get<Origin>();
            text->setString("SCORE " + std::to_string(score.score));

            // Update the origin based on the text bounds
            const sf::FloatRect textBounds = text->getLocalBounds();
            text->setOrigin(
                {textBounds.size.x * origin.x + textBounds.position.x,
                 textBounds.size.y * origin.y + textBounds.position.y}
            );
        })
        .child_of(GetRootEntity());

    world.system<const Lives>("DisplayLivesSystem")
        .kind(flecs::PostUpdate)
        .each([livesEntity](const Lives& lives) {
            const auto& [text] = livesEntity.get<TextRenderable>();
            const auto& [origin] = livesEntity.get<Origin>();
            text->setString("LIVES " + std::to_string(lives.lives));

            // Update the origin based on the text bounds
            const sf::FloatRect textBounds = text->getLocalBounds();
            text->setOrigin(
                {textBounds.size.x * origin.x + textBounds.position.x,
                 textBounds.size.y * origin.y + textBounds.position.y}
            );
        })
        .child_of(GetRootEntity());

    world.system<const Multiplier>("DisplayMultiplierSystem")
        .kind(flecs::PostUpdate)
        .each([multEntity](const Multiplier& multiplier) {
            const auto& [text] = multEntity.get<TextRenderable>();
            const auto& [origin] = multEntity.get<Origin>();
            text->setString("MULT " + std::to_string(multiplier.multiplier) + "x");

            // Update the origin based on the text bounds
            const sf::FloatRect textBounds = text->getLocalBounds();
            text->setOrigin(
                {textBounds.size.x * origin.x + textBounds.position.x,
                 textBounds.size.y * origin.y + textBounds.position.y}
            );
        });
}

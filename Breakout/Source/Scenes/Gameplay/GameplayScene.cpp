// Copyright (c) Eric Jeker 2025.

#include "Scenes/Gameplay/GameplayScene.h"

#include "Entities/BallEntity.h"
#include "Entities/BlockEntity.h"
#include "Entities/PaddleEntity.h"
#include "Modules/Breakout/Components/Intents/TransitionGameStateIntent.h"
#include "Modules/Breakout/Components/MoveIntent.h"
#include "Modules/Breakout/Prefabs/LaunchBallIntent.h"
#include "Modules/Breakout/Prefabs/MoveLeftCommand.h"
#include "Modules/Breakout/Prefabs/MoveRightCommand.h"
#include "Modules/Breakout/Prefabs/TransitionGameStateIntent.h"
#include "Modules/Breakout/Singletons/CurrentLevel.h"
#include "Scenes/Debug/DebugScene.h"

#include "Core/Configuration.h"
#include "Core/GameService.h"
#include "Core/Managers/FileManager.h"
#include "Core/Modules/Event/Components/EventBindings.h"
#include "Core/Modules/Input/Singletons/InputBindings.h"
#include "Core/Modules/Render/Factories/Rectangle.h"
#include "Core/Themes/Nord.h"

#include <fstream>
#include <iostream>
#include <map>
#include <nlohmann/json.hpp>
#include <string>


GameplayScene::GameplayScene(flecs::world& world)
    : Scene(world)
{
}

void GameplayScene::Initialize()
{
    Scene::Initialize();
    SetName("GameplayScene");
    GetRootEntity().set_name("GameplayScene");

    const auto& world = GetWorld();

    // TODO: Asserts that necessary module are present in the world

    // --- Create entities ---
    CreateInputBindings(world);
    CreateEventBindings(world);

    float zOrder = 0.f;
    CreateBackground(world, zOrder);
    CreateBlocks(world, zOrder);
    PaddleEntity::Create(world, GetRootEntity(), zOrder);
    BallEntity::Create(world, GetRootEntity(), zOrder);
}

void GameplayScene::CreateInputBindings(const flecs::world& world)
{
    // Create command prefabs
    auto moveLeft = world.prefab("Gameplay::MoveLeftIntent").is_a<Prefabs::MoveLeftCommand>().set<MoveIntent>({{-1.f, 0.f}});
    auto moveRight = world.prefab("Gameplay::MoveRightIntent").is_a<Prefabs::MoveRightCommand>().set<MoveIntent>({{1.f, 0.f}});

    // Bind the left and right movement to A and D
    world.set<InputBindings>(
        {{{InputKey::Keyboard(sf::Keyboard::Key::A), moveLeft}, {InputKey::Keyboard(sf::Keyboard::Key::D), moveRight}}}
    );
}

void GameplayScene::CreateEventBindings(const flecs::world& world) const
{
    auto pauseGame = world.prefab("Gameplay::PauseGameIntent")
                         .is_a<Prefabs::TransitionGameStateIntent>()
                         .set<TransitionGameStateIntent>({GameTransitions::PauseGame});
    auto launchBall = world.prefab("Gameplay::LaunchBallIntent").is_a<Prefabs::LaunchBallIntent>();

    GetRootEntity().set<EventBindings>(
        {{{InputKey::Keyboard(sf::Keyboard::Key::Escape), pauseGame},
          {InputKey::Keyboard(sf::Keyboard::Key::Space), launchBall}}}
    );
}

std::map<char, GameplayScene::BlockDefinition> GameplayScene::LoadBlockDefinitions(const std::string& filename)
{
    std::map<char, BlockDefinition> blockDefinitions;

    const auto data = FileManager::LoadJSON(filename);

    for (const auto& [key, value] : data["definitions"].items())
    {
        char symbol = key[0];
        BlockDefinition def;

        if (blockDefinitions.contains(symbol))
        {
            continue;
        }

        if (value.contains("color"))
        {
            auto colorArray = value["color"];
            def.color = {colorArray[0], colorArray[1], colorArray[2]};
        }

        def.health = value.value("health", 1);
        def.isIndestructible = value.value("indestructible", false);

        blockDefinitions[symbol] = def;
    }

    return blockDefinitions;
}

void GameplayScene::CreateBlocks(const flecs::world& world, float& zOrder) const
{
    // Which level are we playing?
    int& currentLevel = world.get_mut<CurrentLevel>().level;

    // Load the block definition file (json)
    const auto blockDefinitions = LoadBlockDefinitions("Content/Levels/Definitions.json");

    // Load the level definition file (txt)
    std::ifstream f(std::format("Content/Levels/Level_{}.txt", currentLevel));
    if (!f.is_open())
    {
        return;
    }

    std::string line;
    int row = 0;

    while (std::getline(f, line))
    {
        constexpr float BLOCK_SPACING = 10.f;
        constexpr float CENTER_X = Configuration::RESOLUTION.x / 2;
        constexpr sf::Vector2f BLOCK_SIZE = {120.f, 30.f};

        // Calculate the left margin
        const float leftMargin = CENTER_X - (line.size() / 2 * BLOCK_SIZE.x) - (line.size() - 1) / 2 * BLOCK_SPACING;

        for (int col = 0; col < line.size(); ++col)
        {
            constexpr float TOP_MARGIN = 200.f;

            const char symbol = line[col];
            if (symbol == '.' || !blockDefinitions.contains(symbol))
            {
                continue;
            }

            const auto& [color, health, isIndestructible] = blockDefinitions.at(symbol);

            // Calculate the position (block's origin is {0, 0})
            float x = leftMargin + col * (BLOCK_SIZE.x + BLOCK_SPACING);
            float y = TOP_MARGIN + row * (BLOCK_SIZE.y + BLOCK_SPACING);

            BlockEntity::Create(
                world,
                GetRootEntity(),
                {.position = {x, y}, .color = color, .size = BLOCK_SIZE, .zOrder = zOrder, .health = health, .isIndestructible = isIndestructible
                }
            );
        }

        ++row;
    }

    zOrder += 1.f;
}

void GameplayScene::CreateBackground(const flecs::world& world, float& zOrder) const
{
    Factories::Rectangle::Create(
        world,
        {
            .size = sf::Vector2f{Configuration::RESOLUTION},
            .color = NordTheme::PolarNight4,
            .position = {0.f, 0.f},
            .zOrder = zOrder++,
        }
    )
        .child_of(GetRootEntity());
}

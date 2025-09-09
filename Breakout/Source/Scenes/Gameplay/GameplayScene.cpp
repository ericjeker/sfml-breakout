// Copyright (c) Eric Jeker 2025.

#include "Scenes/Gameplay/GameplayScene.h"

#include "Components/MoveIntent.h"
#include "Entities/BallEntity.h"
#include "Entities/BlockEntity.h"
#include "Entities/PaddleEntity.h"
#include "GameStates/Gameplay/Components/CurrentLevel.h"
#include "Prefabs/MoveLeftCommand.h"
#include "Prefabs/MoveRightCommand.h"
#include "Scenes/Debug/DebugScene.h"
#include "Systems/ApplyPaddlePositionToBallSystem.h"
#include "Systems/CheckAllBlocksDestroyedSystem.h"
#include "Systems/CollisionDetectionSystem.h"
#include "Systems/ConstrainPaddleToScreenSystem.h"
#include "Systems/Intents/ProcessContinueGameIntent.h"
#include "Systems/Intents/ProcessGameOverIntent.h"
#include "Systems/Intents/ProcessGameWonIntent.h"
#include "Systems/Intents/ProcessLaunchBallIntent.h"
#include "Systems/Intents/ProcessNavigateToMainMenuIntent.h"
#include "Systems/Intents/ProcessNextLevelIntent.h"
#include "Systems/Intents/ProcessPauseGameIntent.h"
#include "Systems/Intents/ProcessResumeGameIntent.h"
#include "Systems/OutOfBoundsSystem.h"
#include "Systems/PaddleMovementSystem.h"
#include "Systems/ScreenBounceSystem.h"
#include "Systems/UI/ProcessKeyPressed.h"

#include "Core/Components/DeferredEvent.h"
#include "Core/Configuration.h"
#include "Core/Managers/FileManager.h"
#include "Core/Managers/GameService.h"
#include "Core/Managers/GameStateManager.h"
#include "Core/Managers/SceneManager.h"
#include "Core/Modules/Input/Components/Command.h"
#include "Core/Modules/Input/Singletons/InputBindings.h"
#include "Core/Modules/Render/Factories/Rectangle.h"
#include "Core/Modules/UI/Components/KeyPressed.h"
#include "Core/Modules/UI/Prefabs/KeyPressedEvent.h"
#include "Core/Tags/ScenePaused.h"
#include "Core/Themes/Nord.h"
#include "Core/Utils/Logger.h"

#include <fstream>
#include <iostream>
#include <map>
#include <nlohmann/json.hpp>
#include <numbers>
#include <optional>
#include <string>
#include <tracy/Tracy.hpp>


GameplayScene::GameplayScene(flecs::world& world)
    : Scene(world)
{
}

void GameplayScene::Initialize()
{
    LOG_DEBUG("GameplayScene:Initialize");
    Scene::Initialize();

    auto& world = GetWorld();

    // TODO: Asserts that necessary module are present in the world

    // --- Create entities ---
    CreateInputBindings(world);

    float zOrder = 0.f;
    CreateBackground(world, zOrder);
    CreateBlocks(world, zOrder);
    PaddleEntity::Create(world, GetRootEntity(), zOrder);
    BallEntity::Create(world, GetRootEntity(), zOrder);

    // --- Paddle & Ball Control ---
    PaddleMovementSystem::Initialize(world, GetRootEntity());
    ProcessLaunchBallIntent::Initialize(world, GetRootEntity());
    ApplyPaddlePositionToBallSystem::Initialize(world, GetRootEntity());

    // --- Physics, Collision, Constraints ---
    ScreenBounceSystem::Initialize(world, GetRootEntity());
    CollisionDetectionSystem::Initialize(world, GetRootEntity());
    ConstrainPaddleToScreenSystem::Initialize(world, GetRootEntity());

    // --- Game Over / Game Won ---
    // TODO: instead, check if there are no more ball on the screen = GAME OVER or LOSE ONE LIFE
    OutOfBoundsSystem::Initialize(world, GetRootEntity());
    CheckAllBlocksDestroyedSystem::Initialize(world, GetRootEntity());

    // --- UI & Intents ---
    ProcessKeyPressed::Initialize(world, GetRootEntity());
    ProcessNavigateToMainMenuIntent::Initialize(world, GetRootEntity());
    ProcessGameOverIntent::Initialize(world, GetRootEntity());
    ProcessGameWonIntent::Initialize(world, GetRootEntity());
    ProcessNextLevelIntent::Initialize(world, GetRootEntity());
    ProcessContinueGameIntent::Initialize(world, GetRootEntity());
    ProcessResumeGameIntent::Initialize(world, GetRootEntity());
    ProcessPauseGameIntent::Initialize(world, GetRootEntity());
}

void GameplayScene::HandleEvent(const std::optional<sf::Event>& event)
{
    // Only process when active
    if (GetRootEntity().has<ScenePaused>())
    {
        return;
    }

    if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
    {
        // We still filter the scan code as to not populate the ECS with useless entities
        if (keyPressed->scancode == sf::Keyboard::Scancode::Escape || keyPressed->scancode == sf::Keyboard::Scancode::Space)
        {
            // Add a KeyPressed event to this scene, that will later be handled during the update
            GetWorld()
                .entity()
                .is_a<Prefabs::KeyPressedEvent>()
                .set<KeyPressed>({
                    .code = keyPressed->code,
                    .scancode = keyPressed->scancode,
                    .alt = keyPressed->alt,
                    .control = keyPressed->control,
                    .shift = keyPressed->shift,
                })
                .child_of(GetRootEntity());
        }
    }
}

void GameplayScene::CreateInputBindings(const flecs::world& world)
{
    // Create command prefabs
    auto moveLeft = world.prefab<Prefabs::MoveLeftCommand>().add<Command>().set<MoveIntent>({{-1.f, 0.f}}).child_of(GetRootEntity());
    auto moveRight = world.prefab<Prefabs::MoveRightCommand>().add<Command>().set<MoveIntent>({{1.f, 0.f}}).child_of(GetRootEntity());

    // Bind the left and right movement to A and D
    world.set<InputBindings>(
        {{{InputKey::Keyboard(sf::Keyboard::Key::A), moveLeft}, {InputKey::Keyboard(sf::Keyboard::Key::D), moveRight}}}
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
            LOG_ERROR(std::format("GameplayScene::LoadBlockDefinitions -> Duplicate symbol: {}", symbol));
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

void GameplayScene::CreateBlocks(const flecs::world& world, float& zOrder)
{
    // Which level are we playing?
    int& currentLevel = world.get_mut<CurrentLevel>().level;
    LOG_DEBUG("GameplayScene::CreateBlocks -> currentLevel: {}", currentLevel);

    // Load the block definition file (json)
    const auto blockDefinitions = LoadBlockDefinitions("Content/Levels/Definitions.json");

    // Load the level definition file (txt)
    std::ifstream f(std::format("Content/Levels/Level_{}.txt", currentLevel));
    if (!f.is_open())
    {
        LOG_ERROR("GameplayScene::CreateBlocks -> Failed to open level file: Content/Levels/Level_{}.txt", currentLevel);
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

void GameplayScene::CreateBackground(const flecs::world& world, float& zOrder)
{
    // --- Create Background ---
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

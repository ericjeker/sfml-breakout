// Copyright (c) Eric Jeker 2025.

#include "BreakoutModule.h"

#include "Components/Intents/TransitionGameStateIntent.h"
#include "Modules/Breakout/Singletons/Lives.h"
#include "Modules/Breakout/Singletons/Multiplier.h"
#include "Modules/Breakout/Singletons/Score.h"
#include "Singletons/CurrentLevel.h"
#include "Singletons/GameStateGameLost.h"
#include "Singletons/GameStateGameWon.h"
#include "Singletons/GameStateMenu.h"
#include "Singletons/GameStatePaused.h"
#include "Singletons/GameStatePlaying.h"
#include "Singletons/MaxLevel.h"
#include "Systems/ApplyPaddlePositionToBallSystem.h"
#include "Systems/CheckAllBlocksDestroyedSystem.h"
#include "Systems/CollisionDetectionSystem.h"
#include "Systems/ConstrainPaddleToScreenSystem.h"
#include "Systems/Hud/UpdateLivesWidget.h"
#include "Systems/Hud/UpdateMultiplierWidget.h"
#include "Systems/Hud/UpdateScoreWidget.h"
#include "Systems/Intents/ProcessContinueGameIntent.h"
#include "Systems/Intents/ProcessLaunchBallIntent.h"
#include "Systems/Intents/ProcessNextLevelIntent.h"
#include "Systems/Intents/ProcessTransitionGameStateIntent.h"
#include "Systems/OutOfBoundsSystem.h"
#include "Systems/PaddleMovementSystem.h"
#include "Systems/ScreenBounceSystem.h"

namespace Modules
{

BreakoutModule::BreakoutModule(const flecs::world& world)
{
    // --- Register Singletons ---
    world.singleton<GameStatePlaying>();
    world.singleton<GameStatePaused>();
    world.singleton<GameStateMenu>();
    world.singleton<GameStateGameWon>();
    world.singleton<GameStateGameLost>();

    world.singleton<Score>();
    world.singleton<Lives>();
    world.singleton<Multiplier>();
    world.singleton<CurrentLevel>();
    world.singleton<MaxLevel>();

    // --- Register Components ---
    world.component<TransitionGameStateIntent>();

    // --- Initialize Singletons ---
    world.add<GameStateMenu>();

    // --- Register Systems ---
    // --- Paddle & Ball Control ---
    PaddleMovementSystem::Register(world);
    ProcessLaunchBallIntent::Register(world);
    ApplyPaddlePositionToBallSystem::Register(world);

    // --- Physics, Collision, Constraints ---
    ScreenBounceSystem::Register(world);
    CollisionDetectionSystem::Register(world);
    ConstrainPaddleToScreenSystem::Register(world);

    // --- Game Over / Game Won ---
    // TODO: instead, check if there are no more ball on the screen = GAME OVER or LOSE ONE LIFE
    OutOfBoundsSystem::Register(world);
    CheckAllBlocksDestroyedSystem::Register(world);

    // --- UI & Intents ---
    // TODO: ProcessKeyPressed should use bindings
    //Gameplay::ProcessFocusLost::Initialize(world);
    //Gameplay::ProcessKeyPressed::Register(world);
    ProcessNextLevelIntent::Register(world);
    ProcessContinueGameIntent::Register(world);
    ProcessTransitionGameStateIntent::Register(world);

    // --- HUD ---
    UpdateScoreWidget::Register(world);
    UpdateMultiplierWidget::Register(world);
    UpdateLivesWidget::Register(world);

}

} // namespace Modules

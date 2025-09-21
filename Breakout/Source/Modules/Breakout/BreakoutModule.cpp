// Copyright (c) Eric Jeker 2025.

#include "BreakoutModule.h"

#include "Components/Intents/ExitGameIntent.h"
#include "Components/Intents/LaunchBallIntent.h"
#include "Components/Intents/RestartGameIntent.h"
#include "Components/Intents/ResumeGameIntent.h"
#include "Components/Intents/TransitionGameStateIntent.h"
#include "Modules/Breakout/Singletons/Lives.h"
#include "Modules/Breakout/Singletons/Multiplier.h"
#include "Modules/Breakout/Singletons/Score.h"
#include "Prefabs/ExitGameIntent.h"
#include "Prefabs/LaunchBallIntent.h"
#include "Prefabs/RestartGameIntent.h"
#include "Prefabs/ResumeGameIntent.h"
#include "Prefabs/TransitionGameStateIntent.h"
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
#include "Systems/Intents/ProcessExitGameIntent.h"
#include "Systems/Intents/ProcessLaunchBallIntent.h"
#include "Systems/Intents/ProcessNextLevelIntent.h"
#include "Systems/Intents/ProcessTransitionGameStateIntent.h"
#include "Systems/OutOfBoundsSystem.h"
#include "Systems/PaddleMovementSystem.h"
#include "Systems/ScreenBounceSystem.h"

#include "Core/Modules/Input/Components/Command.h"
#include "Core/Modules/Lifetime/Components/LifetimeOneFrame.h"

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

    // --- Register Game Session Singletons ---
    world.singleton<Score>();
    world.singleton<Lives>();
    world.singleton<Multiplier>();
    world.singleton<CurrentLevel>();
    world.singleton<MaxLevel>();

    // --- Initialize Singletons ---
    world.add<GameStateMenu>();

    // --- Register Prefabs ---
    world.prefab<Prefabs::TransitionGameStateIntent>().add<Command>().add<LifetimeOneFrame>().add<TransitionGameStateIntent>();
    world.prefab<Prefabs::LaunchBallIntent>().add<Command>().add<LifetimeOneFrame>().add<LaunchBallIntent>();
    world.prefab<Prefabs::ResumeGameIntent>().add<Command>().add<LifetimeOneFrame>().add<ResumeGameIntent>();
    world.prefab<Prefabs::RestartGameIntent>().add<Command>().add<LifetimeOneFrame>().add<RestartGameIntent>();
    world.prefab<Prefabs::ExitGameIntent>().add<Command>().add<LifetimeOneFrame>().add<ExitGameIntent>();

    // --- Register Systems ---

    // --- Paddle & Ball Control ---
    PaddleMovementSystem::Register(world);
    ApplyPaddlePositionToBallSystem::Register(world);
    ProcessLaunchBallIntent::Register(world);

    // --- Physics, Collision, Constraints ---
    ScreenBounceSystem::Register(world);
    CollisionDetectionSystem::Register(world);
    ConstrainPaddleToScreenSystem::Register(world);

    // --- Game Over / Game Won ---
    OutOfBoundsSystem::Register(world);
    CheckAllBlocksDestroyedSystem::Register(world);

    // --- UI & Intents ---
    ProcessNextLevelIntent::Register(world);
    ProcessContinueGameIntent::Register(world);
    ProcessTransitionGameStateIntent::Register(world);

    // --- HUD ---
    UpdateScoreWidget::Register(world);
    UpdateMultiplierWidget::Register(world);
    UpdateLivesWidget::Register(world);

    // --- Scene & State Transitions ---
    ProcessContinueGameIntent::Register(world);
    ProcessTransitionGameStateIntent::Register(world);
    ProcessExitGameIntent::Register(world);
    ProcessNextLevelIntent::Register(world);
}

} // namespace Modules

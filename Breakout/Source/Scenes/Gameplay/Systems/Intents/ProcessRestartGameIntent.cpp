// Copyright (c) Eric Jeker 2025.

#include "ProcessRestartGameIntent.h"

#include "GameStates/Gameplay/Components/CurrentLevel.h"
#include "GameStates/Gameplay/Components/Lives.h"
#include "GameStates/Gameplay/Components/Multiplier.h"
#include "GameStates/Gameplay/Components/Score.h"
#include "Scenes/Debug/DebugScene.h"
#include "Scenes/Gameplay/Components/RestartGameIntent.h"
#include "Scenes/Gameplay/GameplayScene.h"
#include "Scenes/Gameplay/Tasks/ResumeGame.h"
#include "Scenes/Hud/HudScene.h"

#include "Core/Components/DeferredEvent.h"
#include "Core/Managers/GameService.h"
#include "Core/Managers/SceneManager.h"
#include "Core/Singletons/FrameCount.h"

namespace
{

auto Update()
{
    return [](const flecs::entity& e, const RestartGameIntent&) {
        const auto& world = e.world();

        LOG_DEBUG("ProcessRestartGameIntent::Update -> Add deferred event, framecount: {}", world.get<FrameCount>().frameCount);
        world.entity().set<DeferredEvent>({[](const flecs::world& world) {
            auto& sceneManager = GameService::Get<SceneManager>();
            sceneManager.LoadScene<GameplayScene>(SceneLoadMode::Single);
            sceneManager.LoadScene<DebugScene>(SceneLoadMode::Additive);
            sceneManager.LoadScene<HudScene>(SceneLoadMode::Additive);
        }});

        world.set<Score>({.score = 0});
        world.set<Lives>({3});
        world.set<Multiplier>({1});
        world.set<CurrentLevel>({1});

        e.destruct();
    };
}

} // namespace

void ProcessRestartGameIntent::Initialize(const flecs::world& world, const flecs::entity& rootEntity)
{
    world.system<const RestartGameIntent>("ProcessRestartGameIntent").kind(flecs::PreUpdate).each(Update()).child_of(rootEntity);
}

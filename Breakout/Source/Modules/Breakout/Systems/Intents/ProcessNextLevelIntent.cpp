// Copyright (c) Eric Jeker 2025.

#include "ProcessNextLevelIntent.h"

#include "Modules/Breakout/Components/Intents/NextLevelIntent.h"
#include "Modules/Breakout/Singletons/CurrentLevel.h"
#include "Modules/Breakout/Singletons/Multiplier.h"
#include "Scenes/Debug/DebugScene.h"
#include "Scenes/Gameplay/GameplayScene.h"
#include "Scenes/Hud/HudScene.h"

#include "Core/GameService.h"
#include "Core/Managers/SceneManager.h"
#include "Core/Modules/Window/Components/DeferredEvent.h"
#include "Core/Modules/Window/Singletons/FrameCount.h"

namespace
{

void Update(const flecs::entity& e, const NextLevelIntent& i)
{
    const int frameCount = e.world().get<FrameCount>().frameCount;
    LOG_DEBUG("ProcessNextLevelIntent::Update -> FrameCount: {}", std::to_string(frameCount));

    // We defer the state change to the end of the frame
    e.world().entity().set<DeferredEvent>({.callback = [](const flecs::world& world) {
        const int frameCount = world.get<FrameCount>().frameCount;
        LOG_DEBUG("ProcessNextLevelIntent -> Processing DeferredEvent -> Load Next Level in frame {}", frameCount);

        auto& sceneManager = GameService::Get<SceneManager>();
        sceneManager.LoadScene<GameplayScene>(SceneLoadMode::Single);
        sceneManager.LoadScene<DebugScene>(SceneLoadMode::Additive);
        sceneManager.LoadScene<HudScene>(SceneLoadMode::Additive);
    }});

    // Move to the next level and reset the multiplier
    e.world().get_mut<CurrentLevel>().level++;
    e.world().get_mut<Multiplier>().multiplier = 1;

    e.destruct();
}

} // namespace

void ProcessNextLevelIntent::Register(const flecs::world& world)
{
    world.system<const NextLevelIntent>("ProcessNextLevelIntent").kind(flecs::PreStore).each(Update);
}

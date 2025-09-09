// Copyright (c) Eric Jeker 2025.

#include "ProcessNextLevelIntent.h"

#include "GameStates/Gameplay/Components/CurrentLevel.h"
#include "GameStates/Gameplay/Components/Multiplier.h"
#include "Scenes/Debug/DebugScene.h"
#include "Scenes/Gameplay/Components/NextLevelIntent.h"
#include "Scenes/Gameplay/GameplayScene.h"
#include "Scenes/Hud/HudScene.h"

#include "Core/Components/DeferredEvent.h"
#include "Core/Managers/GameService.h"
#include "Core/Managers/SceneManager.h"

namespace
{

void Update(const flecs::entity& e, const NextLevelIntent& i)
{
    // We defer the state change to the end of the frame
    e.world().entity().set<DeferredEvent>({.callback = [&](const flecs::world& world) {
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

void ProcessNextLevelIntent::Initialize(const flecs::world& world, const flecs::entity& rootEntity)
{
    world.system<const NextLevelIntent>("ProcessNextLevelIntent").kind(flecs::PreUpdate).each(Update).child_of(rootEntity);
}

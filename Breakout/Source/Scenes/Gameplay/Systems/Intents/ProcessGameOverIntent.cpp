// Copyright (c) Eric Jeker 2025.

#include "ProcessGameOverIntent.h"

#include "Scenes/GameOver/GameOverScene.h"
#include "Scenes/Gameplay/Components/GameOverIntent.h"

#include "Core/Components/DeferredEvent.h"
#include "Core/Managers/GameService.h"
#include "Core/Managers/SceneManager.h"

namespace
{

void Update(const flecs::entity& e, const GameOverIntent& g)
{
    e.world().entity().set<DeferredEvent>({[](const flecs::world& world){
        auto& sceneManager = GameService::Get<SceneManager>();
        sceneManager.LoadScene<GameOverScene>(SceneLoadMode::Additive);
    }});

    e.destruct();
}

} // namespace

void ProcessGameOverIntent::Initialize(const flecs::world& world, const flecs::entity& rootEntity)
{
    world.system<const GameOverIntent>("ProcessGameOverIntent").kind(flecs::PreUpdate).each(Update).child_of(rootEntity);
}

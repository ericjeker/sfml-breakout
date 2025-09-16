// Copyright (c) Eric Jeker 2025.

#include "ProcessGameOverIntent.h"

#include "Scenes/GameOver/GameOverScene.h"
#include "Scenes/Gameplay/Components/GameOverIntent.h"
#include "Scenes/Gameplay/Tasks/PauseGame.h"

#include "Core/Components/DeferredEvent.h"
#include "Core/Managers/GameService.h"
#include "Core/Managers/SceneManager.h"

namespace
{

auto Update(const flecs::entity& rootEntity)
{
    return [rootEntity](const flecs::entity& e, const GameOverIntent& g) {
        e.world().entity().set<DeferredEvent>({[](const flecs::world& world) {
            auto& sceneManager = GameService::Get<SceneManager>();
            sceneManager.LoadScene<GameOverScene>(SceneLoadMode::Additive);
        }});

        // Get CheckIfAllBlocksDestroyed and disable it
        if (const auto system = e.world().lookup("CheckAllBlocksDestroyedSystem"); system.is_valid())
        {
            LOG_DEBUG("GameplayScene::ProcessGameWonIntent -> Disable CheckIfAllBlocksDestroyed");
            system.disable();
        }

        PauseGame::Run(e.world(), rootEntity);

        e.destruct();
    };
}

} // namespace

void ProcessGameOverIntent::Initialize(const flecs::world& world, const flecs::entity& rootEntity)
{
    world.system<const GameOverIntent>("ProcessGameOverIntent").kind(flecs::PreUpdate).each(Update(rootEntity)).child_of(rootEntity);
}

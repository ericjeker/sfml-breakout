// Copyright (c) Eric Jeker 2025.

#include "ProcessGameWonIntent.h"

#include "Scenes/GameWon/GameWonScene.h"
#include "Scenes/Gameplay/Components/GameWonIntent.h"
#include "Scenes/Gameplay/Tasks/PauseGame.h"

#include "Core/Components/DeferredEvent.h"
#include "Core/Managers/GameService.h"
#include "Core/Managers/SceneManager.h"

namespace
{

std::function<void(flecs::entity, GameWonIntent)> Update(const flecs::entity& rootEntity)
{
    return [rootEntity](const flecs::entity& e, const GameWonIntent& g) {
        LOG_DEBUG("GameplayScene::ProcessGameWonIntent -> event: {}", e.id());

        e.world().entity().set<DeferredEvent>({[](const flecs::world& world) {
            auto& sceneManager = GameService::Get<SceneManager>();
            sceneManager.LoadScene<GameWonScene>(SceneLoadMode::Additive);
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

void ProcessGameWonIntent::Initialize(const flecs::world& world, const flecs::entity& rootEntity)
{
    world.system<const GameWonIntent>("ProcessGameWonIntent").kind(flecs::PreUpdate).each(Update(rootEntity)).child_of(rootEntity);
}

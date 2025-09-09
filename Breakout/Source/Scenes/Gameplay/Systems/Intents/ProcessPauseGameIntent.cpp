// Copyright (c) Eric Jeker 2025.

#include "ProcessPauseGameIntent.h"

#include "Scenes/Gameplay/Components/PauseGameIntent.h"
#include "Scenes/Gameplay/Tasks/PauseGame.h"
#include "Scenes/Pause/PauseScene.h"

#include "Core/Components/DeferredEvent.h"
#include "Core/Managers/GameService.h"
#include "Core/Managers/SceneManager.h"
#include "Core/Utils/Logger.h"


namespace
{

std::function<void(flecs::entity, PauseGameIntent)> Update(const flecs::entity& rootEntity)
{
    return [rootEntity](const flecs::entity& e, const PauseGameIntent& p) {
        LOG_DEBUG("GameplayScene::PauseGameSystem");

        e.world().entity().set<DeferredEvent>({[](const flecs::world& world) {
            LOG_DEBUG("GameplayScene::PauseGameSystem -> Load Pause Scene");
            auto& sceneManager = GameService::Get<SceneManager>();
            sceneManager.LoadScene<PauseScene>(SceneLoadMode::Additive);
        }});

        PauseGame::Run(e.world(), rootEntity);

        // Destroy the command entity
        LOG_DEBUG("GameplayScene::PauseGameSystem -> Destroy PauseGameIntent");
        e.destruct();
    };
};

} // namespace

void ProcessPauseGameIntent::Initialize(const flecs::world& world, const flecs::entity& rootEntity)
{
    world.system<const PauseGameIntent>("ProcessPauseGameIntent").kind(flecs::PreUpdate).each(Update(rootEntity)).child_of(rootEntity);
}

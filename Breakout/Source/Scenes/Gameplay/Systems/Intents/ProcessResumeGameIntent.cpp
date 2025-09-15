// Copyright (c) Eric Jeker 2025.

#include "ProcessResumeGameIntent.h"

#include "Scenes/BouncingBall/Prefabs/Ball.h"
#include "Scenes/Gameplay/Components/ResumeGameIntent.h"
#include "Scenes/Gameplay/Tasks/ResumeGame.h"
#include "Scenes/Pause/PauseScene.h"

#include "Core/Components/DeferredEvent.h"
#include "Core/Managers/GameService.h"
#include "Core/Managers/SceneManager.h"


namespace
{

auto Update(const flecs::entity& rootEntity)
{
    return [rootEntity](const flecs::entity& e, const ResumeGameIntent& r) {
        LOG_DEBUG("GameplayScene::ResumeGameSystem -> event: {}", e.id());

        e.world().entity().set<DeferredEvent>({[](const flecs::world& world) {
            auto& sceneManager = GameService::Get<SceneManager>();
            sceneManager.UnloadScene<PauseScene>();
        }});

        ResumeGame::Run(e.world(), rootEntity);

        e.destruct();
    };
}

} // namespace

void ProcessResumeGameIntent::Initialize(const flecs::world& world, const flecs::entity& rootEntity)
{
    LOG_DEBUG("ProcessResumeGameIntent::Initialize");
    world.system<const ResumeGameIntent>("ProcessResumeGameIntent").kind(flecs::PreUpdate).each(Update(rootEntity)).child_of(rootEntity);
}

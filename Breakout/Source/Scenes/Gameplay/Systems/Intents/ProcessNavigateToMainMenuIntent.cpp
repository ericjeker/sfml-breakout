// Copyright (c) Eric Jeker 2025.

#include "ProcessNavigateToMainMenuIntent.h"

#include "GameStates/MainMenu/MainMenuState.h"
#include "Scenes/Gameplay/Components/NavigateToMainMenuIntent.h"

#include "Core/Components/DeferredEvent.h"
#include "Core/Managers/GameService.h"
#include "Core/Managers/GameStateManager.h"

namespace
{

void Update(const flecs::entity& e, const NavigateToMainMenuIntent i)
{
    // We defer the state change to the end of the frame
    e.world().entity().set<DeferredEvent>({.callback = [](flecs::world& world) {
        GameService::Get<GameStateManager>().ChangeState(std::make_unique<MainMenuState>(world));
    }});

    e.destruct();
}

} // namespace

void ProcessNavigateToMainMenuIntent::Initialize(const flecs::world& world, const flecs::entity& rootEntity)
{
    world.system<const NavigateToMainMenuIntent>("ProcessNavigateToMainMenuIntent")
        .kind(flecs::PreUpdate)
        .each(Update)
        .child_of(rootEntity);
}

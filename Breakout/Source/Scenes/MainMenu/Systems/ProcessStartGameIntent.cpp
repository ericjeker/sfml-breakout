// Copyright (c) Eric Jeker 2025.

#include "ProcessStartGameIntent.h"

#include "GameStates/Gameplay/GameplayState.h"
#include "Scenes/MainMenu/Components/StartGameIntent.h"

#include "Core/Components/DeferredEvent.h"
#include "Core/Managers/GameService.h"
#include "Core/Managers/GameStateManager.h"

namespace
{

void Update(const flecs::iter& it, size_t, const StartGameIntent i)
{
    it.world().entity().set<DeferredEvent>({.callback = [](flecs::world& world) {
        GameService::Get<GameStateManager>().ChangeState(std::make_unique<GameplayState>(world));
    }});
}

} // namespace

void ProcessStartGameIntent::Initialize(const flecs::world& world, const flecs::entity& rootEntity)
{
    world.system<const StartGameIntent>("ProcessStartGameIntent").each(Update).child_of(rootEntity);
}

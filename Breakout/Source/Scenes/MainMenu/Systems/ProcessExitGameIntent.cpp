// Copyright (c) Eric Jeker 2025.

#include "ProcessExitGameIntent.h"

#include "Scenes/MainMenu/Components/ExitGameIntent.h"

#include "Core/Components/DeferredEvent.h"
#include "Core/GameInstance.h"
#include "Core/Managers/GameService.h"

namespace
{

void Update(const flecs::iter& it, size_t, const ExitGameIntent i)
{
    it.world().entity().set<DeferredEvent>({.callback = [](const flecs::world& world) {
        GameService::Get<GameInstance>().RequestExit();
    }});
}

} // namespace

void ProcessExitGameIntent::Initialize(const flecs::world& world, const flecs::entity& rootEntity)
{
    world.system<const ExitGameIntent>("ProcessExitGameIntent").each(Update).child_of(rootEntity);
}

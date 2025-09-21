// Copyright (c) Eric Jeker 2025.

#include "ProcessExitGameIntent.h"

#include "Modules/Breakout/Components/Intents/ExitGameIntent.h"

#include "Core/GameInstance.h"
#include "Core/GameService.h"
#include "Core/Modules/Window/Components/DeferredEvent.h"

namespace
{
void Update(const flecs::entity& e, const ExitGameIntent)
{
    e.world().entity().set<DeferredEvent>({.callback = [](const flecs::world&) {
        GameService::Get<GameInstance>().RequestExit();
    }});

    e.destruct();
}
} // namespace

void ProcessExitGameIntent::Register(const flecs::world& world)
{
    world.system<const ExitGameIntent>("ProcessExitGameIntent").each(Update);
}
